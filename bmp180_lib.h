//#################################################################################
//	Library to use BMP180 with ATMEL Atmega family
//#################################################################################

#ifndef _ATMEGA_BMP180_LIB_H_
#define _ATMEGA_BMP180_LIB_H_

#include <avr/io.h>
#include <util/delay.h> 
#include <stdio.h>
#include <util/twi.h>
#include <math.h>

#define F_CPU 16000000UL

//----------------------------------------
#define OSS 3								///< Accuracy mode
//----------------------------------------

#define BMP180_R 0xEF						///< Read from sensor
#define BMP180_W 0xEE						///< Write to sensor

/* Calibration sensor */
/**
* \brief Calibration sensor
* \param[in] BMP180_calibration_int16_t : Calibration coefficients for pressure
* \param[in] BMP180_calibration_uint16_t : Calibration coefficients for temperature
* \param[in] *errorcode : Code of error transmitted by pointer
*/
void BMP180_Calibration(int16_t BMP180_calibration_int16_t[],int16_t BMP180_calibration_uint16_t[], uint8_t* errorcode);
uint16_t bmp180ReadShort(uint8_t address, uint8_t* errorcode);
int32_t bmp180ReadTemp(uint8_t* error_code);
int32_t bmp180ReadPressure(uint8_t* errorcode);

/**
* \brief Convert and displaying values from BMP180
* \param[in] BMP180_calibration_int16_t : Calibration coefficients for pressure
* \param[in] BMP180_calibration_uint16_t : Calibration coefficients for temperature
* \param[in] *errorcode : Code of error transmitted by pointer
* \param[in] *temperature : temperature from DHT180 transmitted by pounter
* \param[in] *pressure : pressure transmitted by pointer
*/
void bmp180Convert(int16_t BMP180_calibration_int16_t[],int16_t BMP180_calibration_uint16_t[],int32_t* temperature, int32_t* pressure, uint8_t* error_code);
/**
* \brief Calculating altitude (Height above sea level)
* \param[in] *pressure : pressure transmitted by pointer
* \param[out] altitude : calculating altitude
*/
int32_t bmp180CalcAltitude(int32_t pressure);
#endif
