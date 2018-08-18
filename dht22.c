/* Library for termo-sensor DHT 22 */
#include "dht22.h"
#include <avr/interrupt.h>

uint8_t c22=0;						/* temp */


void Request22()						/* Sending start pulse/request	*/
{
	DHT22_DDR |= (1<<DHT22_BIT);
	DHT22_PORT &= ~(1<<DHT22_BIT);				/* set to low pin		*/
	_delay_ms(20);						/* wait for 20ms		*/
	DHT22_PORT |= (1<<DHT22_BIT);				/* set to high pin		*/
	_delay_us (30);						/* wait for 30us		*/
}

void Response22()						/* Receive response from DHT22	*/
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

uint8_t Receive_data22()					/* Receive data 		*/
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


int getdht22 (int *temperature22, int * humidity22)
{
	
	uint8_t I_RH,D_RH,I_Temp,D_Temp,CheckSum;		/* temp					*/
	
 
	cli();
	Request22();						/* send start pulse 			*/
	Response22();						/* receive response 			*/
	I_RH=Receive_data22();					/* store first eight bit in I_RH 	*/
	D_RH=Receive_data22();					/* store next eight bit in D_RH 	*/
	I_Temp=Receive_data22();				/* store next eight bit in I_Temp 	*/
	D_Temp=Receive_data22();				/* store next eight bit in D_Temp 	*/
	CheckSum=Receive_data22();				/* store next eight bit in CheckSum 	*/
	sei();
	*temperature22 = D_Temp * 0.1 + I_Temp * 25.6;		/* calculating temperature		*/
	*humidity22 =  D_RH * 0.1 + I_RH * 25.6;			/* calculating humidity			*/
	
	
}
