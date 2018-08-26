
/* Type of MCU:			Atmega16, Flash 16 Kbytes, SRAM 1 Kbytes, EEPROM 512 bytes.
 * Description:         	A trimmed library for working with LCD displays based on HD44780
				controllers. This library operates on a 4-bit and 8-bit connection scheme
				LCD, supports only display data*/

#include <inttypes.h>

//---------------------------------------------------------------------------------------------
//If you want to use an 8-bit connection scheme, then uncomment #define LCD_8BIT
//#define LCD_8BIT
//---------------------------------------------------------------------------------------------

#ifndef LCD
#define LCD

#if 1

/* Specify the port to which the LCD display DB0 ... DB7 terminals are connected	*/
#define DPIN  PIND
#define DDDR  DDRD
#define DPORT PORTD	

/* In figures, we indicate the pin numbers m connected to the display 			*/					
/* PIN	MCU	LCD   */
#define DB0	0// DB0
#define DB1	1// DB1
#define DB2	2// DB2	
#define DB3	3// DB3		
#define DB4	7// DB4  
#define DB5	6// DB5
#define DB6	5// DB6
#define DB7	4// DB7 

/* Specify the port to which the display pins E, RS, R / W are connected 		*/
#define CDDR  DDRB
#define CPORT PORTB

/* Specify the pin numbers of the MK to which the display is connected 			*/
#define E	0	// E	
#define RW	2   	/* R/W		R / W = 1 is read from the LCD, R / W = 0 is written in the LCD		*/
#define RS	1 	/* RS		RS = 0 send the command to the LCD, RS = 1 send the data to the LCD	*/

#endif
//----------------------------------Settings complete---------------------------------
#if 1
/* Custom functions, which used in the program 	*/
                   
void LCDGotoXY(uint8_t,uint8_t);						/* Set cursor to X, Y position 							*/
void LCDdata(uint8_t);								/* Display 1 character 								*/
void LCDdataXY(uint8_t,uint8_t,uint8_t);					/* Display 1 character in the display in the X, Y position 			*/
void LCDsendString(char*);							/* Display the string								*/
void LCDstringXY(char*,uint8_t,uint8_t);					/* Display the string at position x, y						*/
void LCDstring_of_sramXY(uint8_t*,uint8_t,uint8_t);				/* Display the string at position x, y from RAM					*/
void LCDstring_of_flashXY(const uint8_t*,uint8_t, uint8_t);			/* Display the string to the position x, y from the flash 			*/
void LCDinit(void);								/* Initializing the LCD 							*/
void LCDblank(void);								/* Make invisible information on the display 					*/
void LCDnblank(void);								/* Make visible information in the display + turn off the visible cursors	*/
void LCDclear(void);								/* Clear the display from info + cursor to position 0,0				*/
void LCDcursor_bl(void);							/* Enable the flashing cursor							*/
void LCDcursor_on(void);							/* Enable underline cursor 							*/
void LCDcursor_vi(void);							/* Enable both cursors								*/
void LCDcursorOFF(void);							/* Disable the cursor (any)							*/
void LCDacr(void);								/* AC address counter will always be shifted to n + 1				*/
void LCDacl(void);								/* AC address counter will always be shifted to n-1				*/
void LCDcursorl(void);								/* Move the cursor to the left by 1 character					*/
void LCDcursorr(void);								/* Move the cursor to the right by 1 character					*/
void LCDcursorln(uint8_t);							/* Move the cursor to the left with n characters				*/
void LCDcursorrn(uint8_t);							/* Move the cursor to the right by n characters					*/
void LCDscreenl(void);								/* Move the screen to the left by 1 character					*/
void LCDscreenr(void);								/* Move the screen to the right by 1 character					*/
void LCDscreenln(uint8_t);							/* Move the screen to the left by n characters					*/
void LCDscreenrn(uint8_t);							/* Move the screen to the right by n characters					*/
void LCDscreenL(void);								/* With each new character, the screen will shift to the left			*/
void LCDscreenR(void);								/* With each new character, the screen will shift to the right 			*/
void LCDresshift(void);								/* Cursor to position 0.0 + reset all shifts, the image remains			*/


/* Two-line display */
#define LINE0 0x00
#define LINE1 0x40

#endif

/* LIBRARY FOR TRANSFER OF BINARY NUMBERS TO SYMBOLS AND CONCLUSION OF THEM ON THE LCD */
#if 1
/* Here we redefine the function to output the symbol to the screen or to the terminal
if the output is not used leave an empty macro */

#define BCD_SendData(data) /* LCD_WriteData(data) */

/* Display zero in the senior digits or not. if commented, then
zero will not be displayed */

#define MIRROR_NULL

/* Use the buffer or not. if commented, write to the buffer
will not be produced */

#define BCD_USE_BUF

/* Translate the numbers into symbols or leave them in binary-decimal form. if
comment, then the numbers will be stored in binary-decimal form, if
leave it in character with the trailing zero character */

#define BCD_SYM

/*************************** Custom Functions ***********************/

/* Take the pointer to the buffer. If the buffer is not used,
a value of zero will be returned */

char* BCD_GetPointerBuf(void);

/* Convert 8-bit binary numbers. Optimized functions for 1, 2 and 3-ex
bit decimal numbers. If the functions BCD_1 (uint8_t value) transfer 2 or 3-eh
bit decimal number, it will not work correctly. The same applies
and to two other functions */

void BCD_1(uint8_t value);
void BCD_2(uint8_t value);
void BCD_3(uint8_t value);

/* Convert 16-bit binary numbers. Optimized functions for 3, 4 and 5-and
bit decimal numbers. If the functions BCD_3Int (uint16_t value) pass 4 or 5
bit decimal number, it will not work correctly. The same applies
and to two other functions */

void BCD_3Int(uint16_t value);
void BCD_4Int(uint16_t value);
void BCD_5Int(uint16_t value);

/* Functions for converting 8, 16 and 32 bit binary numbers. All functions
correctly work with their data types */

void BCD_Uchar(uint8_t value);
void BCD_Uint(uint16_t value);
void BCD_Ulong(uint32_t value);

#endif

#endif

//Configuring 
/*
Configuring the configuration includes the following steps.

1. Connecting an external library to output data.
If this functionality is not used, this line can be commented out.

// Connect library for an output on lcd
#include "lcd_lib.h"

2. Override the output function. If this functionality is not used, the macro should be left blank.

// the output is not used leave the macro empty
#define BCD_SendData (data)

// the output is used, we redefine the function
#define BCD_SendData (data) LCD_WriteData (data)

3. Displays the zero in the upper digits. If you comment out the setting, zero will not be output,
The space symbol will be saved instead. If left, zero will be displayed.

// with this setting, BCD_Uchar (3) will display on screen 003
#define MIRROR_NULL

// if comment BCD_Uchar (3) will display on screen 3
// # define MIRROR_NULL

4.Using the buffer.
Depending on the library settings, the number conversion functions can store the result in the buffer
in the form of a string, which you can then transfer to some output function. If this setting
comment, then the buffer will not be written.

#define BCD_BUF_USE

5. The desired final result. The library allows you to convert binary numbers to binary-decimal form
or in character. If this setting is commented out, the binary-decimal representation is used, if not, then symbolic.

#define BCD_SYM

Description of functions

Common functions for converting 8, 16 and 32 bit binary numbers.

void BCD_Uchar (uint8_t value) - converts numbers from 0 to 255
void BCD_Uint (uint16_t value) - converts numbers from 0 to 65535
void BCD_Ulong (uint32_t value) - converts numbers from 0 to 4294967295

Example

#define F_CPU 9600000UL
...
uint8_t count = 120;
uint16_t adc_value = 1020;
...
BCD_Uchar (count);
BCD_Uint (adc_value);
BCD_Ulong (F_CPU);

Functions for converting decimal numbers of a given digit.

void BCD_1 (uint8_t value) - converts numbers from 0 to 9
void BCD_2 (uint8_t value) - converts numbers from 0 to 99
void BCD_3 (uint8_t value) - converts numbers from 0 to 255
void BCD_3Int (uint16_t value) - converts numbers from 0 to 999
void BCD_4Int (uint16_t value) - converts numbers from 0 to 9999
void BCD_5Int (uint16_t value) - converts numbers from 0 to 65535

Example

#define DATA 23
...
// variable that is always <10
uint8_t index = 0;

// counter up to 999
uint8_t count;
...
BCD_1 (index);
BCD_2 (DATA);
BCD_3Int (count);

// this is wrong
BCD_1 (DATA);
BCD_3 (count);

Function to get a pointer to the buffer in which the result is stored. If the buffer is not used, the function returns a null pointer.

uint8_t * BCD_GetPointerBuf (void);

Example
// define a variable pointer to the buffer
uint8_t * pBuf;

// initialize this variable
pBuf = BCD_GetPointerBuf ();

// convert the number, and then display the line on the screen
BCD_3Int (counter);
LCD_SendStr (pBuf); */

//Examples of using commands
/*
LCDcommand (0b00101000); // Turn on the 4-bit transmit / receive interface and select 5x8 pixels.
LCDcommand (0b00000001); // Cleaning the screen
LCDcommand (0b00000010); // Set cursor to position 0.0 + reset shifts
LCDcommand (0b00000110); // On. counter of address counter, image movement.
LCDcommand (0b00001100); // Turn on the display + without displaying the cursor.

LCDdata ('A');			// Print the A
LCDGotoXY (6.1);		// Move the cursor to the seventh position the second line.
LCDdata ('B');			// And output the character B
LCDdataXY ('T', 0,0);   //The symbol T was taken to position 0,0
LCDdata ('y');
LCDdata ('to');
LCDdata ('a');
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LCDGotoXY (1,1); // Move the cursor to the position 1.1.
LCDsendString; // Output Goggles
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LCDstring ("Pretty Eyes", 0.1); // Output the string to the position 0,1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
First, declare in an ounce uint8_t text_1 [] = "Goggles_3";
LCDstring_of_sram (text_1,0,0); and now output the line to the position 0,0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
First, we'll declare in the flush const uint8_t PROGMEM text_1 [] = "Bug-eyed_1";
LCDstring_of_flash (text_1,0,0); and now output the line to the position 0,0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~							 						 
*/

/*
The display is initialized in this way.
1. Turn on the power.
2.After VCC> = 4.5V wait at least 15 msec.
3. Sending the command 0x30 - 0b110000
4.We wait at least 4.1 msec
5.Send the command 0x30
6.We are not less than 100 usec
Sending the command 0x30
The above operations are initializing for the LCD
and are able to bring the display into operation from any state.
8.Next work with the display in the normal mode.

In the initial state, E = 0, R / W = 0, RS is an arbitrary, DB0 ... DB7 high impedance (HI).
Such a state should signal E = 0, R / W = 0 should be maintained all the time in between
data exchange operations with the MC.

The display is configured like this:

int main (void)
{
	init (); // Initialize the MK.
	LCDinit (); // Initialize the LCD, this function first, then everything else.

	while (1) // The main program loop.
	{

	}
}

Map of display symbols.
LCDGotoXY (3,1);
The number 3 means the fourth position in any line. If 0, then this is the first position in any line.
the number 1 means the lower line and 0 the upper.
| 0,0 | 1,0 | 2,0 | 3,0 | 4,0 | 5,0 | 6,0 | 7,0 | - First line
| 0,1 | 1,1 | 2,1 | 3,1 | 4,1 | 5,1 | 6,1 | 7,1 | - second line

100
1 I / D S
I / D - address counter offset, 0-reduction 1-increase
S - screen content shift 0 content is not shifted, 1 is shifted,
if I / D - 0, then to the right, if 1 then to the left.
LCDcommand (100) - counter n-1, the screen does not shift. The symbols will be displayed <-
LCDcommand (110) - counter n + 1, the screen does not shift. Characters will be displayed ->
LCDcommand (101) - counter n-1, the image is shifted to the right with each new symbol
LCDcommand (111) - counter n + 1, the image is shifted to the left with each new symbol

1 0 0 0 0
1 S / C R / L - -
S / C (screen / cursor) - 0 the cursor is shifted, 1 the screen is shifted.
R / L (right / left) - 0 shift left, 1 shift right. for one command for 1 shift
*/