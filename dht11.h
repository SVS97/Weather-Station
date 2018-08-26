/* Library for termo-sensor DHT 11 */
#define F_CPU   16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#define DHT11_BIT 4				///< Number of pin DHT11
#define DHT11_PORT PORTB			///< PORT DHT11				
#define DHT11_DDR DDRB				///< PORT DDR DHT11			
#define DHT11_PIN PINB				///< Pin DHT11				




/**
* \brief Receiving and Calculating temperature and humidity
* \param[in] *temperature : temperature value transmitted by pointer
* \param[in] *humidity : humidity value transmitted by pointer
*/
void getdht11 (uint16_t *temperature11, uint16_t *humidity11);