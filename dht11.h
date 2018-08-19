/* Library for termo-sensor DHT 11 */
#define F_CPU   16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#define DHT11_BIT 4					/* Number of pin DHT11		*/
#define DHT11_PORT PORTB			/* PORT DHT11				*/
#define DHT11_DDR DDRB				/* PORT DDR DHT11			*/
#define DHT11_PIN PINB				/* Pin DHT11				*/

/* Sending request to sensor */
void Request11();

/* Getting response from sensor */
void Response11();

/*Reading data from sensor*/
uint8_t Receive_data11();

/* Receiving and Calculating temperature and humidity 
 * @ temperature - temperature value transmitted by pointer
 * @ humidity - humidity value transmitted by pointer
*/
void getdht11 (uint16_t *temperature11, uint16_t *humidity11);