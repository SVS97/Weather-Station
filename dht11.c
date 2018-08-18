/* Library for termo-sensor DHT 11 */
#include "dht11.h"
#include <avr/interrupt.h>
uint8_t c=0;


void Request11()				/* Microcontroller send start pulse/request */
{
	DHT11_DDR |= (1<<DHT11_BIT);
	DHT11_PORT &= ~(1<<DHT11_BIT);	/* set to low pin */
	_delay_ms(20);				/* wait for 20ms */
	DHT11_PORT |= (1<<DHT11_BIT);	/* set to high pin */
	_delay_us (30);
}

void Response11()				/* receive response from DHT11 */
{
	DHT11_DDR &= ~(1<<DHT11_BIT);
	DHT11_PORT &= ~(1<<DHT11_BIT);
	

	uint16_t wait;
	for (wait = 80; wait > 0 ; wait--) {
		_delay_us(1);
		if (DHT11_PIN & (1 << DHT11_BIT))
			break;
	}

	for (wait = 80; wait > 0 ; wait--) {
		_delay_us(1);
		if ((DHT11_PIN & (1 << DHT11_BIT)) ==0)
			break;
	}
}

uint8_t Receive_data11()			/* receive data */
{	DHT11_DDR &= ~(1<<DHT11_BIT);
	DHT11_PORT &= ~(1<<DHT11_BIT);
	for (int q=0; q<8; q++)
	{
		uint16_t wait;
		for (wait = 55; wait > 0 ; wait--) {
			_delay_us(1);
			if (DHT11_PIN& (1 << DHT11_BIT))
				break;
		}
		if(DHT11_PIN & (1<<DHT11_BIT))
			_delay_us(30);
		if(DHT11_PIN & (1<<DHT11_BIT)){				/* if high pulse is greater than 30ms */
			c = (c<<1)|(0x01);	/* then its logic HIGH */
		}else 
		{
			c = (c<<1);				/* otherwise its logic LOW */
		}
		for (wait = 70; wait > 0 ; wait--) {
			_delay_us(1);
			if ((DHT11_PIN & (1 << DHT11_BIT))==0)
				break;
			}
	}
	return c;
}


int getdht11 (int *temperature, int * humidity)
{
	
	uint8_t I_RH,D_RH,I_Temp,D_Temp,CheckSum;		/* temp					*/
	
	
	cli();
	Request11();						/* send start pulse 			*/
	Response11();						/* receive response 			*/
	I_RH=Receive_data11();					/* store first eight bit in I_RH 	*/
	D_RH=Receive_data11();					/* store next eight bit in D_RH 	*/
	I_Temp=Receive_data11();				/* store next eight bit in I_Temp 	*/
	D_Temp=Receive_data11();				/* store next eight bit in D_Temp 	*/
	CheckSum=Receive_data11();				/* store next eight bit in CheckSum 	*/
	sei();
	*temperature = I_Temp;		/* calculating temperature		*/
	*humidity=  I_RH;			/* calculating humidity			*/
	
	
}