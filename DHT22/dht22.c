/* Library for termo-sensor DHT 22 */
#include "dht22.h"
#include <avr/interrupt.h>

uint8_t c22=0;							///< temp 

/**
\brief Micro controller send start pulse/request
*/
void Request22()						
{
	DHT22_DDR |= (1<<DHT22_BIT);
	DHT22_PORT &= ~(1<<DHT22_BIT);				/* set to low pin				*/
	_delay_ms(20);						/* wait for 20ms				*/
	DHT22_PORT |= (1<<DHT22_BIT);				/* set to high pin				*/
	_delay_us (30);						/* wait for 30us				*/
}

/**
\brief Receive response from DHT11
*/
void Response22()						
{
	DHT22_DDR &= ~(1<<DHT22_BIT);
	DHT22_PORT &= ~(1<<DHT22_BIT);
	

	uint16_t wait;
	for (wait = 80; wait > 0 ; wait--) {
		_delay_us(1);
		if (DHT22_PIN & (1 << DHT22_BIT))
		break;
	}

	for (wait = 80; wait > 0 ; wait--) {
		_delay_us(1);
		if ((DHT22_PIN & (1 << DHT22_BIT)) ==0)
		break;
	}
}

/**
\brief Receive data from sensor
*/
uint8_t Receive_data22()					
{	
	DHT22_DDR &= ~(1<<DHT22_BIT);
	DHT22_PORT &= ~(1<<DHT22_BIT);
	for (int q=0; q<8; q++)
	{
		uint16_t wait;
		for (wait = 55; wait > 0 ; wait--) {
			_delay_us(1);
			if (DHT22_PIN & (1 << DHT22_BIT))
			break;
		}
		if(DHT22_PIN & (1<<DHT22_BIT))
		_delay_us(30);
		if(DHT22_PIN & (1<<DHT22_BIT)){			/* if high pulse is greater than 30ms	*/
			c22 = (c22<<1)|(0x01);			/* then its logic HIGH 			*/
		}else
		{
			c22 = (c22<<1);				/* otherwise its logic LOW 		*/
		}
		for (wait = 70; wait > 0 ; wait--) {
			_delay_us(1);
			if ((DHT22_PIN & (1 << DHT22_BIT))==0)
			break;
		}
	}
	return c22;
}

/**
* \brief Receiving and Calculating temperature and humidity
* \param[in] *temperature : temperature value transmitted by pointer
* \param[in] *humidity : humidity value transmitted by pointer
*/
void getdht22 (uint16_t *temperature22, uint16_t * humidity22)
{
	uint8_t I_RH,D_RH,I_Temp,D_Temp;			/* temp					*/
	 
	cli();
	Request22();						/* send start pulse 			*/
	Response22();						/* receive response 			*/
	I_RH=Receive_data22();					/* store first eight bit in I_RH 	*/
	D_RH=Receive_data22();					/* store next eight bit in D_RH 	*/
	I_Temp=Receive_data22();				/* store next eight bit in I_Temp 	*/
	D_Temp=Receive_data22();				/* store next eight bit in D_Temp 	*/
	sei();
	*temperature22 = D_Temp * 0.1 + I_Temp * 25.6;		/* calculating temperature		*/
	*humidity22 =  D_RH * 0.1 + I_RH * 25.6;		/* calculating humidity			*/
}
