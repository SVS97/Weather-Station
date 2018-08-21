
#ifndef DEFINE_H
#define DEFINE_H

#define  F_CPU 16000000UL /* Quartz Frequency of the MCU */


#endif

/* ================================================ =======================
Type Size in bytes Value range
char 1 (8) from -128 to 127
short 2 (16) from -32768 to 32767
long 4 (32) from -2 147 483 648 to 2 147 483 647
unsigned char 1 (8) from 0 to 255
unsigned short 2 (16) from 0 to 65535
unsigned long 4 (32) from 0 to 4 294 967 295

Type Size in bytes Value range
char 1 (8) from -128 to 127
unsigned char 1 (8) from 0 to 255
char 1 (8) from -128 to 127
int 2 (16) from -32768 to 32767
unsigned int 2 (16) from 0 to 65535
signed int 2 (16) from -32768 to 32767
short int 2 (16) from -32768 to 32767
unsigned short int 2 (16) from 0 to 65535
signed short int 2 (16) from -32768 to 32767
long int 4 (32) from -2147483648 to 2147483647
unsigned long int 4 (32) from 0 to 4294967295
signed long int 4 (32) from -2147483648 to 2147483647
float 4 (32) from 3.4E-38 to 3.4E + 38
double 8 (64) from 1.7E-308 to 1.7E + 308
long double 10 (80) from 3.4Å-4932 to 3.4Å + 4932


volatile - variables with this directive are not affected by the optimizer.
================================================== =======================
<< - bitwise left shift
1 << 6 is 0b01000000, move 1 left to 6th position
5 << 5 is 0b10100000, move 5 to the left to position 5
================================================== =======================
>> - bitwise shift right
255; // 0b11111111
255 >> 1; result 127 or 0b01111111
================================================== =======================
~ - bitwise inversion
94 is 0b01011110
~ 94 will become 161 or 0b10100001
================================================== =======================
| | - bitwise OR
tmp = 155
tmp = tmp | 4; // set the second bit of the tmp variable to one
155 0b10011011
| |
4 0b00000100
= 159 0b10011111
Use decimal numbers to set the bits is quite inconvenient.
It is much more convenient to do this with the help of the shift operation to the left <<.
tmp = tmp | (1 << 4); // set the fourth bit of the tmp variable to one
We read from right to left - move the unit four places to the left,
perform an OR operation between the resulting number and the value of the variable tmp,
assign the result to tmp.
You can set several bits per unit
tmp = tmp | (1 << 7) | (1 << 5) | (1 << 0);
tmp | = (1 << 7) | (1 << 5) | (1 << 0);
set the seventh, fifth and zero bits of the tmp variable to one
================================================== =======================
& is a bitwise AND
tmp = 155 is 0b10011011
tmp = tmp & (~ (1 << 3)); // zero the third bit
1 << 3 0b00001000
~ (1 << 3) 0b11110111
tmp & (~ (1 << 3)) 0b10011011 & 0b11110111
result 0b10010011

tmp = tmp & (~ ((1 << 3) | (1 << 5) | (1 << 6))); // zero the third, fifth and sixth bits
or so
tmp & = (~ ((1 << 3) | (1 << 5) | (1 << 6)))
================================================== =======================
^ -bit exclusive OR
tmp = 155; 0b10011011
tmp = tmp ^ 8; // Invert the fourth bit with tmp
155 0b10011011
^
8 0b00001000
= 147 0b10010011
tmp = tmp ^ (1 << 3); // Invert the third bit with tmp or so
tmp ^ = (1 << 4); // Invert the fourth bit
You can invert several bits at the same time
tmp ^ = ((1 << 4) | (1 << 2) | (1 << 1)); // Invert 4.2 and 1 bits
================================================== =======================
How can I check if a bit is set in a variable? It is necessary to reset all the bits,
except for the check, and then compare the obtained value with zero
if ((tmp & (1 << 2))! = 0) {
	// the block will be executed only if it is installed
	// second bit of the tmp variable
}
and here on the contrary, if the bit is cleared
if ((tmp & (1 << 2)) == 0) {
	// the block will be executed only if it is not installed
	// second bit of the tmp variable
}
================================================== =======================
Operator Meaning
== is equal to
! = is not equal
> more
> = greater than or equal to
<less than
<= less than or equal to
================================================== =======================
* /
/ *
Operation symbol Purpose of operation
() Function call
[] Selecting an element of an array
. Selecting a record item
-> Selecting a record item
!! Logical negation
~ Bitwise negation
- Change of sign
++ Increase by one
- Decrease by one
& Address acquisition
* Address to the address
(type) Conversion type (i.e. (float) a)
sizeof () Determining the size in bytes
* Multiplication
/ Division
% Determination of the remainder from division
+ Addition
- Subtraction
<< Shift left
>> Shift Right
<Less than
<= Less than or equal to
> More than
> = Greater than or equal to
== Equals
! = Not equal to
& Bitwise logical "AND"
^ Bitwise exclude
*/
