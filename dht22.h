/* Library for termo-sensor DHT 22 */

#define F_CPU   16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define DHT22_BIT 3				/* Number of pin DHT22		*/
#define DHT22_PORT PORTB			/* PORT DHT22			*/
#define DHT22_DDR DDRB				/* PORT DDR DHT22		*/
#define DHT22_PIN PINB



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
void getdht22 (uint16_t *temperature22, uint16_t *humidity22);