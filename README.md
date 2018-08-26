# Weather Station
This code developed for education aims, not for commercial using. It's a graduation project GL C/Embedded BaseCamp
### Description
Weather station consist of two temperature and humidity sensors (DHT11 indoor, DHT22 outdoor), one barometer (BMP180 I2C) LCD display (1602A) and micro controller Arduino Uno. Code writing without using Arduino libraries (BARE METAL AVR). Sensors interrogated every 30 seconds. States of display information changes every 30 seconds and with button help. Also weather station show current time (not accurate), time settings by button. A (very) simple weather station written in C for training purposes. The code is written for the purpose of acquaintance with timers, interrupts, buttons, display and different sensors. Seconds are counted in the interrupt timer. The time setting is done using two buttons. Thanks for helping .
### Compilation
Project compiles with Makefile help (make all)
### Prototype Example
![Example](https://github.com/SVS97/Weather-Station/blob/master/Prototype.jpg)

