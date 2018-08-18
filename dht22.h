/* Library for termo-sensor DHT 22 */

#define F_CPU   16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define DHT22_BIT 2				/* Number of pin DHT22		*/
#define DHT22_PORT PORTD			/* PORT DHT22			*/
#define DHT22_DDR DDRD				/* PORT DDR DHT22		*/
#define DHT22_PIN PIND



/*Sending request to sensor*/
void Request22();
/*Getting response from sensor*/
void Response22();
/*Reading data from sensor*/
uint8_t Receive_data22();
/* Receiving and Calculating temperature and humidity 
 * @ temperature - temperature value transmitted by pointer
 * @ humidity - humidity value transmitted by pointer
*/
int getdht22 (int *temperature22, int *humidity22);