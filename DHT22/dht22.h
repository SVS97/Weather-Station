/* Library for termo-sensor DHT 22 */

#define F_CPU   16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define DHT22_BIT 3					///< Number of pin DHT22		
#define DHT22_PORT PORTB				///< PORT DHT22			
#define DHT22_DDR DDRB					///< DDR DHT22			
#define DHT22_PIN PINB					///< Pin DHT22		
	

/**
* \brief Receiving and Calculating temperature and humidity
* \param[in] *temperature : temperature value transmitted by pointer
* \param[in] *humidity : humidity value transmitted by pointer
*/
void getdht22 (uint16_t *temperature22, uint16_t *humidity22);