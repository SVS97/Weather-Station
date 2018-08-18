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
uint8_t *pBuf;										
uint8_t I_RH,D_RH,I_Temp,D_Temp,CheckSum;
uint16_t temperature11 = 0;
uint16_t humidity11 = 0;
uint16_t temperature22 = 0;
uint16_t humidity22 = 0;

int32_t temperature = 0;
long  pressure = 0;
int16_t BMP085_calibration_int16_t[8];
int16_t BMP085_calibration_uint16_t[3];
uint8_t error_code=0;
long alt;

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
 * ms_val :	sleep time in miliseconds.
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


int main(void)
{
	second = 0;
	minute = 0;							/** Setting start value for time and alarm	*/
	hour = 0;
	LCDinit();
	pBuf = BCD_GetPointerBuf();

	t1_init();							/* Timer  for seconds initialization		*/

	DDRC |= (0 << 0) | (0 << 1);				/** Setting clock buttons			 */
	PORTC |= (1 << 0) | (1 << 1);

	// Initialize TWI
	i2cSetBitrate(1000);
	// Initialize BMP085
	BMP180_Calibration(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&error_code);
	
	
	
    while (1) 
    {
		BCD_3(hour);
		LCDstring_of_sramXY(pBuf, 0, 0);
		BCD_3(minute);
		LCDstring_of_sramXY(pBuf, 3, 0);
		BCD_3(second);
		LCDstring_of_sramXY(pBuf, 6, 0);
		LCDdataXY(':',3,0);
		LCDdataXY(':',6,0);
		
		if ((PINC & (1 << 0)) == (0 << 0)){			/* Setting hours manually			 */
			_delay_ms(1000);
			if ((PINC & (1 << 0)) == (0 << 0))
			hour++;
			if (hour >= 24)
			hour = 0;
		}
		
		if ((PINC & (1 << 1)) == (0 << 1)){			/* Setting minutes manually			 */
			_delay_ms(1000);
			if ((PINC & (1 << 1)) == (0 << 1))
			minute++;
			if (minute >= 60)
			minute = 0;
			
		}
		if (second % 10 == 0)
		{
			getdht11(&temperature11, &humidity11);
			getdht22(&temperature22, &humidity22);
			BCD_3Int(temperature11);
			LCDstring_of_sramXY(pBuf, 10, 0);
			BCD_3Int(humidity11);
			LCDstring_of_sramXY(pBuf, 13, 0);
			BCD_3(temperature22);
			LCDstring_of_sramXY(pBuf, 0, 1);
			BCD_3(humidity22);
			LCDstring_of_sramXY(pBuf, 3, 1);
			//LCDstringXY("T1=",6,1);
			//LCDstringXY("H1=",0,1);
			bmp180Convert(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&temperature, &pressure,&error_code);
			alt = bmp180CalcAltitude(pressure);
			pressure = pressure / 133.3224 + 120;
			BCD_3Int(pressure);
			LCDstring_of_sramXY(pBuf, 7, 1);
			BCD_3Int(alt);
			LCDstring_of_sramXY(pBuf, 11, 1);
			sleep_ms(1);
			
		}		
	
		
    }
}

