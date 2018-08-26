/* Library for termo-sensor DHT 11 */
#include "dht11.h"
#include <avr/interrupt.h>
uint8_t c = 0;	///< Temp

/**
\brief Micro controller send start pulse/request	
*/
void Request11()						
{
	DHT11_DDR |= (1<<DHT11_BIT);
	DHT11_PORT &= ~(1<<DHT11_BIT);				/* Set to low pin 				*/
	_delay_ms(20);						/* Wait for 20ms 				*/
	DHT11_PORT |= (1<<DHT11_BIT);				/* Set to high pin				*/
	_delay_us (30);
}

/**
\brief Receive response from DHT11 	
*/
void Response11()				
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
		if ((DHT11_PIN & (1 << DHT11_BIT)) == 0)
			break;
	}
}

/**
\brief Receive data from sensor
*/
uint8_t Receive_data11()					
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
		if(DHT11_PIN & (1<<DHT11_BIT)){				/* if high pulse is greater than 30ms		*/
			c = (c<<1)|(0x01);				/* then its logic HIGH 				*/
		}else 
		{
			c = (c<<1);					/* otherwise its logic LOW 			*/
		}
		for (wait = 70; wait > 0 ; wait--) {
			_delay_us(1);
			if ((DHT11_PIN & (1 << DHT11_BIT))==0)
				break;
			}
	}
	return c;
}

/**
* \brief Receiving and Calculating temperature and humidity
* \param[in] *temperature : temperature value transmitted by pointer
* \param[in] *humidity : humidity value transmitted by pointer
*/
void getdht11 (uint16_t *temperature, uint16_t * humidity)
{
	uint8_t I_RH,D_RH,I_Temp,D_Temp;				/* temp					*/
	
	cli();
	Request11();							/* send start pulse 			*/
	Response11();							/* receive response 			*/
	I_RH = Receive_data11();					/* store first eight bit in I_RH 	*/
	D_RH = Receive_data11();					/* store next eight bit in D_RH 	*/
	I_Temp = Receive_data11();					/* store next eight bit in I_Temp 	*/
	D_Temp = Receive_data11();					/* store next eight bit in D_Temp 	*/
	sei();
	*temperature = I_Temp;						/* calculating temperature		*/
	*humidity=  I_RH;						/* calculating humidity			*/
}