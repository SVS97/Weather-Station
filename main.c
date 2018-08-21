#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include "dht11.h"
#include "dht22.h"
#include "bmp180_lib.h"



volatile uint8_t second;
volatile uint8_t minute;
volatile uint8_t hour;
char *pBuf;									/* Buffer for LCD */
uint16_t temperature11 = 0;					/* Temperature from DHT11	*/
uint16_t humidity11 = 0;					/* Humidity from DHT22		*/
uint16_t temperature22 = 0;					/* Temperature from DHT22	*/
uint16_t humidity22 = 0;					/* Humidity from DHT22		*/

int32_t temperature = 0;					/* Temperature from BMP180	 */
long  pressure = 0;							/* Pressure from BMP180		 */
int16_t BMP085_calibration_int16_t[8];		/* Calibration BMP180		 */
int16_t BMP085_calibration_uint16_t[3];		/* Calibration BMP180		 */
uint8_t error_code=0;						/* Error of BMP180			 */
long alt;									/* Altitude					 */

uint8_t clr = 0;							/*Counter for clear display  */


const enum state {							/* State of indication		*/
	IN_MODE = 0,
	OUT_MODE,
	PRESSURE_MODE,
} state;
volatile enum state STATE = IN_MODE;		/* global state flag		*/
void switch_state(enum state new_state);

/* Timer2 Interrupt (on overflow) */
ISR(TIMER2_OVF_vect, ISR_BLOCK)
{
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20)); /* stop timer */
	/* It's often required to manually reset interrupt flag */
	/* to avoid infinite processing of it.                  */
	/* not on AVRs (unless OCB bit set)                     */
	/* 	TIFR2 &= ~TOV2;                                 */
}

void t1_init()
{
	/* Adjust the divisor (CLK/256)*/
	TCCR1A = 0x00;
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10);
	/* Counting to 62500 (1 sec))*/
	TCNT1 = 0;
	OCR1B = 62500;
	/*Enable timer overflow interrupt*/
	TIMSK1 |= (1 << TOIE1);
	/* set the general interrupt enable bit*/
	sei();
}

/* Interrupt for count seconds */
ISR(TIMER1_OVF_vect)
{
	
	second++;
	if (second >= 60){
		minute++;
		second = 0;
	}
	if (minute >= 60){
		hour++;
		minute = 0;
	}
	if (hour >= 24){
		hour=0;
	}
}


/**
* Enables sleep mode to reduce power consumption.
* ms_val :	sleep time in milliseconds.
*/
void sleep_ms(uint16_t ms_val)
{
	set_sleep_mode(SLEEP_MODE_IDLE);	// Idle Mode, allows timer 1 to work
	cli();		/* Disable interrupts -- as memory barrier */
	sleep_enable();	/* Set SE (sleep enable bit) */
	sei();  	/* Enable interrupts. We want to wake up, don't we? */
	while (ms_val--) {
		sleep_cpu();	/* Put MCU to sleep */
		/* This is executed after wakeup */
		/* We Will Wake Up In ISR(TIMER1_COMPA_vect, ISR_BLOCK) */
	}
	sleep_disable();	/* Disable sleeps for safety */
}

/* Initializing external interrupts */
void int_ini(void)
{
	/* Enable interrupts INT0 on a falling edge */
	EICRA |= (1<<ISC01) | (0 << ISC00) | (1 << ISC11);
	/* allow external interrupts INT0 */
	EIMSK |= (1<<INT0) | (1 << INT1);
}

/* External interrupt for mode*/
ISR(INT0_vect)
{
	/* Change of states		 */
	if ((PIND & (1 << 2)) == (0 << 2)){
		_delay_ms(200);
		if ((PINC & (1 << 2)) == (0 << 2)){
			switch_state(STATE);
		}
	}
}

void switch_state(enum state new_state)
{
	switch (new_state) {
		case IN_MODE:
		STATE = OUT_MODE;
		break;
		case OUT_MODE:
		STATE = PRESSURE_MODE;
		break;
		default:
		STATE = IN_MODE;
	}
		
}

void start_init();				/* Initializing all devices */
void LCD_clock();				/* Displaying hours			*/
void btn_clock();				/* Clock setting by button	*/
void sensors_data();			/* Get data from sensors	*/
void LCD_in();					/* Displaying in data		*/
void LCD_out();					/* Displaying out data		*/
void LCD_bar();					/* Displaying pressure		*/


int main(void)
{
	start_init();
	
	while (1)
	{
		LCD_clock();
		btn_clock();
		sensors_data();
	
		
		/* Displaying different options */
		switch (STATE) {
			case IN_MODE :
				LCD_in();
				break;
			case OUT_MODE:
				LCD_out();
				break;
			case PRESSURE_MODE:
				LCD_bar();
				break;
			default :
				STATE = IN_MODE;
		}
		
	}
}


void start_init()
{
	second = 0;							/** Setting start value for time	*/
	minute = 0;							/*									*/
	hour = 0;							/*									*/
	LCDinit();							/* Initializing LCD Display			*/
	pBuf = BCD_GetPointerBuf();			/* Initializing LCD Buffer 			*/

	t1_init();							/* Timer  for seconds initialization*/

	DDRC |= (0 << 0) | (0 << 1);		/** Setting clock buttons			 */
	PORTC |= (1 << 0) | (1 << 1);		/*									 */
	
	DDRD |= (0 << 2);					/** Setting external interrupts buttons	*/
	PORTD |= (1 << 2);					/*										*/
	int_ini();							/* External interrupt initializing		*/

	/*Initialize BMP085*/
	BMP180_Calibration(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&error_code);
	
	const char PROGMEM text1[] = " Weather";		/** Start text				*/
	const char PROGMEM text2[] = "station";			/*							*/
	uint8_t i = 0;									/* Counter for start text	*/
	
	while(i<13){
		LCDstringXY(text1, 12,0);					/* Start text				*/
		LCDstringXY(text2, 16,1);
		_delay_ms(200);
		LCDscreenl();
		i++;
	}
	sleep_ms(2);
	
	getdht11(&temperature11, &humidity11);				/* Getting information from sensors */
	getdht22(&temperature22, &humidity22);
	bmp180Convert(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&temperature, &pressure,&error_code);
	alt = bmp180CalcAltitude(pressure);
	pressure = pressure / 133.3224 + 120;				/* Translation into mmHg			*/
}

void LCD_clock()
{
	BCD_2(hour);
	LCDstringXY(pBuf, 3, 0);						/* Clocks on display				*/
	BCD_2(minute);
	LCDstringXY(pBuf, 6, 0);
	BCD_2(second);
	LCDstringXY(pBuf, 9, 0);
	LCDdataXY(':',5,0);
	LCDdataXY(':',8,0);
}

void btn_clock()
{
		if ((PINC & (1 << 0)) == (0 << 0)){			/* Setting hours manually			 */
			_delay_ms(100);
			if ((PINC & (1 << 0)) == (0 << 0))
			hour++;
			if (hour >= 24)
			hour = 0;
		}
		
		if ((PINC & (1 << 1)) == (0 << 1)){			/* Setting minutes manually			 */
			_delay_ms(100);
			if ((PINC & (1 << 1)) == (0 << 1))
			minute++;
			if (minute >= 60)
			minute = 0;
		}
}

void sensors_data()
{
		if (second % 30 == 0)
		{
			getdht11(&temperature11, &humidity11);			/* interrogation of sensors every 30 seconds */
			getdht22(&temperature22, &humidity22);
			bmp180Convert(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&temperature, &pressure,&error_code);
			alt = bmp180CalcAltitude(pressure);
			pressure = pressure / 133.3224 + 120;
			sleep_ms(1);
			/* Change of states */
			switch_state(STATE);
		}
};

void LCD_in()
{
	if (clr == 0)
	LCDclear();						/* clear display once				*/
	clr = 1;
	BCD_2(temperature11);
	LCDstringXY(pBuf, 3, 1);		/* Displaying data in the screen	*/
	BCD_2(humidity11);
	LCDstringXY(pBuf, 12, 1);
	LCDstringXY("In",13,0);
	LCDstringXY("T= ",0,1);
	LCDstringXY("C",5,1);
	LCDstringXY("H= ",9,1);
	LCDstringXY("%",14,1);
	
}
void LCD_out()
{
	if (clr == 1)
	LCDclear();
	clr = 2;
	BCD_2(temperature22);
	LCDstringXY(pBuf, 3, 1);
	BCD_2(humidity22);
	LCDstringXY(pBuf, 12, 1);
	LCDstringXY("Out",13,0);
	LCDstringXY("T= ",0,1);
	LCDstringXY("C",5,1);
	LCDstringXY("H= ",9,1);
	LCDstringXY("%",14,1);	
};

void LCD_bar()
{
	if (clr == 2)
	LCDclear();
	clr = 0;
	BCD_3Int(pressure);
	LCDstringXY(pBuf, 3, 1);
	BCD_3Int(alt);
	LCDstringXY(pBuf, 12, 1);
	LCDstringXY("Bar",13,0);
	LCDstringXY("Pr=",0,1);
	LCDstringXY("mm",6,1);
	LCDstringXY("A=",10,1);
	LCDstringXY("m",15,1);
};