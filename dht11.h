/* Library for termo-sensor DHT 11 */
#define F_CPU   16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#define DHT11_BIT 3				/* Number of pin DHT11		*/
#define DHT11_PORT PORTD			/* PORT DHT11			*/
#define DHT11_DDR DDRD				/* PORT DDR DHT11		*/
#define DHT11_PIN PIND

/*Sending request to sensor*/
void Request11();

/*Getting response from sensor*/
void Response11();

/*Reading data from sensor*/
uint8_t Receive_data11();
/* Receiving and Calculating temperature and humidity 
 * @ temperature - temperature value transmitted by pointer
 * @ humidity - humidity value transmitted by pointer
*/
int getdht11 (int *temperature11, int *humidity11);