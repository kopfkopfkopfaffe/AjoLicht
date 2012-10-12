/*
* ajolicht.c
*
*  Created on: May 15, 2011
*      Author: KopfKopfKopfAffe
*


This file contains parts of the code example at http://www.batsocks.co.uk/readme/art_bcm_3.htm
Especially the ISR is pretty much copied from there. So the Credits for that go to Nigel Batten.
He included the following in his file, quote:
  ============================================================================
  Copyright (c) 2008, Nigel Batten.
  Contactable at <firstname>.<lastname>@mail.com

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

	1.	Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
	2.	Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
  ============================================================================

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

For more helpful information on the binary coded modulation, visit the excellent website
        *** http://www.batsocks.co.uk/readme/art_bcm_1.htm ***
*/

//Fuses: CE / D9

/* 
LED	R	G	B 
===========================
1	PD1	PD2	PD3
---------------------------
2	PC5	PC4	PC3
---------------------------
3	PC2	PC1	PB5
---------------------------
4	PB4	PB3	PB2
---------------------------
5	PB1	PB0	PD7
---------------------------
ERR	%	PD6	%
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "includes/ajolicht.h"
#include "includes/utils.h"
#include "includes/uart.h"
#include <util/delay.h>
#include <stdlib.h>
#ifndef F_CPU
#define F_CPU = 16000000UL
#endif
#define UART_BAUD_RATE 38400



uint8_t i = 0;
uint8_t colors[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t input[15]={0, 0,0,0, 0,0,0, 0,0,0, 0,0,0 ,0,0};
uint8_t sliceb[8]={0,0,0,0,0,0,0,0};
uint8_t slicec[8]={0,0,0,0,0,0,0,0};
uint8_t sliced[8]={0,0,0,0,0,0,0,0};
uint8_t gammatable[250];
uint8_t g_bitpos = 0;
char received_char = 0;
char collect = 0;
//ID of microcontroller
char ID = '1'; 
//start symbol of datastream
char start = 'A'; 



// Timer interrupt handler - called once per bit position.
ISR( TIMER2_COMP_vect ){
	// Flicker the error-LED so we can see if controller is running
	PORTD |= (1<<PD6);
	// Increment bitpos and reset at 7
	g_bitpos ++ ;
	g_bitpos &= 7;
	// Set Ports
	PORTB = sliceb[ g_bitpos ] ;
	PORTC = slicec[ g_bitpos ] ;	
	PORTD = sliced[ g_bitpos ] ;
	// now set the delay...
	TCNT2 = 0;
	OCR2 <<= 1 ;
	// reset the compare match value.
	if (g_bitpos == 0) OCR2 = 1 ; 
	// Flicker the error-LED again
	PORTD &= ~(1<<PD6);
}


int main(void) {
	init();
	sei();

	while (1) {
		received_char = uart_getc();
		//check if any char is received to end aquire mode
		if (received_char & UART_NO_DATA) {
			//no data available from UART
		}
		else if ((collect == 0) && (received_char == start)) {
			//start collecting data if ID received
			collect = 1;
		} else if ((collect == 1) && (received_char == ID)){
			collect = 2;
		} else if ((collect > 1) && (received_char > 0)) {
			//if ordered to collect, write bytes into receive buffer
			//and increment collect counter. if 15 bytes collected, reset
			//collect counter to stop collecting. then transfer input array
			//via gamma correction to color array.
			input[collect-2] = received_char;
			collect ++;
			if (collect > 16){
				collect = 0;
				transferarray();
				slicetime();						
			}
		}
		else{
			//collect = 0;
		}
	}
}

void transferarray(){
	//led1
	colors[0] = gammatable[input[0]];
	colors[1] = gammatable[input[1]];
	colors[2] = gammatable[input[2]];
	//led2	
	colors[3] = gammatable[input[3]];
	colors[4] = gammatable[input[4]];
	colors[5] = gammatable[input[5]];
	//led3
	colors[6] = gammatable[input[6]];
	colors[7] = gammatable[input[7]];
	colors[8] = gammatable[input[8]];
	//led4
	colors[9] = gammatable[input[9]];
	colors[10] = gammatable[input[10]];
	colors[11] = gammatable[input[11]];
	//led5
	colors[12] = gammatable[input[12]];
	colors[13] = gammatable[input[13]];
	colors[14] = gammatable[input[14]];
}

void slicetime(){
	uint8_t portbits = 0;
	uint8_t bitvalue ;
	
	for ( uint8_t bitpos = 0 ; bitpos < 8 ; bitpos++ )
	{
		//PORTB
		portbits = 0;
		bitvalue = 1;
		//PB0
		if (colors[13] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PB1
		if (colors[12] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PB2
		if (colors[11] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PB3
		if (colors[10] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PB4
		if (colors[9] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PB5
		if (colors[8] & (1 << bitpos)) portbits |= bitvalue ;

		sliceb[ bitpos ] = portbits ;

		//PORTC
		portbits = 0;
		bitvalue = 2;
		//PC1
		if (colors[7] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PC2
		if (colors[6] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PC3
		if (colors[5] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PC4
		if (colors[4] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PC5
		if (colors[3] & (1 << bitpos)) portbits |= bitvalue ;
		
		slicec[ bitpos ] = portbits ;

		//PORTD
		portbits = 0;
		bitvalue = 4;

		//PD2
		if (colors[1] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PD3
		if (colors[2] & (1 << bitpos)) portbits |= bitvalue ;
		bitvalue = bitvalue << 1 ;
		//PD4
		if (colors[0] & (1 << bitpos)) portbits |= bitvalue ;
		
		bitvalue = 128;
		
		//PD7
		if (colors[14] & (1 << bitpos)) portbits |= bitvalue ;
		
		sliced[ bitpos ] = portbits ;
	}
}

void init(){
    // set the timer to CTC mode.
	TCCR2 |= (1<<WGM21) ; 
	// use clock/32 tickrate
	TCCR2 |= ((1<<CS21)|(1<<CS20)) ; 
	// initial delay
	OCR2 = 1 ; 
	// Enable the Compare Match interrupt
	TIMSK |= (1 << OCIE2) ; 

	// init uart	
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));

	// Set LED-ports to output:
	DDRD |= (1 << PD6); 
	DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);
	DDRC |= (1 << PC5) | (1 << PC4) | (1 << PC3) | (1 << PC2) | (1 << PC1);
	DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD3) | (1 << PD2) | (1 << PD4);
	
	// All LEDs off:
	PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0));
	PORTC &= ~((1 << PC5) | (1 << PC4) | (1 << PC3) | (1 << PC2) | (1 << PC1));
	PORTD &= ~((1 << PD7) | (1 << PD6) | (1 << PD3) | (1 << PD2) | (1 << PD4));

    //Set up gamma table:
	gammatable[0] = 0;
	gammatable[1] = 0;
	gammatable[2] = 0;
	gammatable[3] = 0;
	gammatable[4] = 0;
	gammatable[5] = 1;
	gammatable[6] = 1;
	gammatable[7] = 1;
	gammatable[8] = 2;
	gammatable[9] = 2;
	gammatable[10] = 3;
	gammatable[11] = 3;
	gammatable[12] = 4;
	gammatable[13] = 4;
	gammatable[14] = 5;
	gammatable[15] = 5;
	gammatable[16] = 6;
	gammatable[17] = 7;
	gammatable[18] = 7;
	gammatable[19] = 8;
	gammatable[20] = 9;
	gammatable[21] = 10;
	gammatable[22] = 11;
	gammatable[23] = 12;
	gammatable[24] = 13;
	gammatable[25] = 13;
	gammatable[26] = 14;
	gammatable[27] = 15;
	gammatable[28] = 17;
	gammatable[29] = 18;
	gammatable[30] = 19;
	gammatable[31] = 20;
	gammatable[32] = 21;
	gammatable[33] = 22;
	gammatable[34] = 23;
	gammatable[35] = 25;
	gammatable[36] = 26;
	gammatable[37] = 27;
	gammatable[38] = 29;
	gammatable[39] = 30;
	gammatable[40] = 31;
	gammatable[41] = 33;
	gammatable[42] = 34;
	gammatable[43] = 36;
	gammatable[44] = 37;
	gammatable[45] = 39;
	gammatable[46] = 40;
	gammatable[47] = 42;
	gammatable[48] = 44;
	gammatable[49] = 45;
	gammatable[50] = 47;
	gammatable[51] = 49;
	gammatable[52] = 50;
	gammatable[53] = 52;
	gammatable[54] = 54;
	gammatable[55] = 56;
	gammatable[56] = 58;
	gammatable[57] = 59;
	gammatable[58] = 61;
	gammatable[59] = 63;
	gammatable[60] = 65;
	gammatable[61] = 67;
	gammatable[62] = 69;
	gammatable[63] = 71;
	gammatable[64] = 73;
	gammatable[65] = 75;
	gammatable[66] = 77;
	gammatable[67] = 80;
	gammatable[68] = 82;
	gammatable[69] = 84;
	gammatable[70] = 86;
	gammatable[71] = 88;
	gammatable[72] = 91;
	gammatable[73] = 93;
	gammatable[74] = 95;
	gammatable[75] = 97;
	gammatable[76] = 100;
	gammatable[77] = 102;
	gammatable[78] = 105;
	gammatable[79] = 107;
	gammatable[80] = 109;
	gammatable[81] = 112;
	gammatable[82] = 114;
	gammatable[83] = 117;
	gammatable[84] = 119;
	gammatable[85] = 122;
	gammatable[86] = 125;
	gammatable[87] = 127;
	gammatable[88] = 130;
	gammatable[89] = 133;
	gammatable[90] = 135;
	gammatable[91] = 138;
	gammatable[92] = 141;
	gammatable[93] = 143;
	gammatable[94] = 146;
	gammatable[95] = 149;
	gammatable[96] = 152;
	gammatable[97] = 155;
	gammatable[98] = 158;
	gammatable[99] = 161;
	gammatable[100] = 164;
	gammatable[101] = 166;
	gammatable[102] = 169;
	gammatable[103] = 172;
	gammatable[104] = 175;
	gammatable[105] = 179;
	gammatable[106] = 182;
	gammatable[107] = 185;
	gammatable[108] = 188;
	gammatable[109] = 191;
	gammatable[110] = 194;
	gammatable[111] = 197;
	gammatable[112] = 201;
	gammatable[113] = 204;
	gammatable[114] = 207;
	gammatable[115] = 210;
	gammatable[116] = 214;
	gammatable[117] = 217;
	gammatable[118] = 220;
	gammatable[119] = 224;
	gammatable[120] = 227;
	gammatable[121] = 230;
	gammatable[122] = 234;
	gammatable[123] = 237;
	gammatable[124] = 241;
	gammatable[125] = 244;
	gammatable[126] = 248;
	gammatable[127] = 250;
}

// This is a debug function that sets an indivitual LED to a 
// color encoded by the color channel and a 1-bit brightness.
void setcolor(uint8_t ledno, uint8_t chan, uint8_t color) {

	if ((ledno == 0) && (chan == 0) && (color == 0))
	PORTD &= ~(1 << PD4);
	if ((ledno == 0) && (chan == 0) && (color == 1))
	PORTD |= ~(1 << PD4);
	if ((ledno == 0) && (chan == 1) && (color == 0))
	PORTD &= ~(1 << PD2);
	if ((ledno == 0) && (chan == 1) && (color == 1))
	PORTD |= ~(1 << PD2);
	if ((ledno == 0) && (chan == 2) && (color == 0))
	PORTD &= ~(1 << PD3);
	if ((ledno == 0) && (chan == 2) && (color == 1))
	PORTD |= ~(1 << PD3);

	if ((ledno == 1) && (chan == 0) && (color == 0))
	PORTC &= ~(1 << PC5);
	if ((ledno == 1) && (chan == 0) && (color == 1))
	PORTC |= ~(1 << PC5);
	if ((ledno == 1) && (chan == 1) && (color == 0))
	PORTC &= ~(1 << PC4);
	if ((ledno == 1) && (chan == 1) && (color == 1))
	PORTC |= ~(1 << PC4);
	if ((ledno == 1) && (chan == 2) && (color == 0))
	PORTC &= ~(1 << PC3);
	if ((ledno == 1) && (chan == 2) && (color == 1))
	PORTC |= ~(1 << PC3);

	if ((ledno == 2) && (chan == 0) && (color == 0))
	PORTC &= ~(1 << PC2);
	if ((ledno == 2) && (chan == 0) && (color == 1))
	PORTC |= ~(1 << PC2);
	if ((ledno == 2) && (chan == 1) && (color == 0))
	PORTC &= ~(1 << PC1);
	if ((ledno == 2) && (chan == 1) && (color == 1))
	PORTC |= ~(1 << PC1);
	if ((ledno == 2) && (chan == 2) && (color == 0))
	PORTB &= ~(1 << PB5);
	if ((ledno == 2) && (chan == 2) && (color == 1))
	PORTB |= ~(1 << PB5);

	if ((ledno == 3) && (chan == 0) && (color == 0))
	PORTB &= ~(1 << PB4);
	if ((ledno == 3) && (chan == 0) && (color == 1))
	PORTB |= ~(1 << PB4);
	if ((ledno == 3) && (chan == 1) && (color == 0))
	PORTB &= ~(1 << PB3);
	if ((ledno == 3) && (chan == 1) && (color == 1))
	PORTB |= ~(1 << PB3);
	if ((ledno == 3) && (chan == 2) && (color == 0))
	PORTB &= ~(1 << PB2);
	if ((ledno == 3) && (chan == 2) && (color == 1))
	PORTB |= ~(1 << PB2);

	if ((ledno == 4) && (chan == 0) && (color == 0))
	PORTB &= ~(1 << PB1);
	if ((ledno == 4) && (chan == 0) && (color == 1))
	PORTB |= ~(1 << PB1);
	if ((ledno == 4) && (chan == 1) && (color == 0))
	PORTB &= ~(1 << PB0);
	if ((ledno == 4) && (chan == 1) && (color == 1))
	PORTB |= ~(1 << PB0);
	if ((ledno == 4) && (chan == 2) && (color == 0))
	PORTD &= ~(1 << PD7);
	if ((ledno == 4) && (chan == 2) && (color == 1))
	PORTD |= ~(1 << PD7);
}
