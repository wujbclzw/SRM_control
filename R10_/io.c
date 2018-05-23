#include "global.h"
#include <xc.h>


void Init_IO_Port ()
{
	TRISA = 0x0001 ;	// output, except AN0/1 -- RA0/1 ( RA1 do not use now. )
	PORTA = 0x0000 ;	// output low LED red yellow and Relay
	ODCA  = 0xFA7F ;	// open-drain    A7 A8 A10

	TRISB = 0x0002 ;	// output , except AN3 -- RB1
	PORTB = 0x0000 ;	// output low 
	ODCB  = 0xFFFF ;	// open-drain !!LED out B5

	TRISC = 0x01BF ;	// output  , except AN6/7/8 -- RC0/1/2  and sensor A/B/C - RC3/4/5    
						// RC7 - input for steering direction 
						// RC8 - input for voltage detect
	PORTC = 0x0000 ;	// output low 
	ODCC  = 0xFFFF ;	// open-drain 	
	
	// Analog Select  (set in file "ADC.c")
	ANSELA = 0x0000;
	ANSELB = 0x0000;
	ANSELC = 0x0000;
	// input change notification Interrupt
	CNENA = 0x0000;
	CNENB = 0x0000;
	CNENC = 0x0000;	
	CNPUA = 0x0000;
	CNPUB = 0x0000;
	CNPUC = 0x0000;
	CNPDA = 0x0000;
	CNPDB = 0x0000;
	CNPDC = 0x0000;
	
	// set IO port interrupt enable bits
	CNPUCbits.CNPUC3 = 1;
	CNPUCbits.CNPUC4 = 1;
	CNPUCbits.CNPUC5 = 1;
	
	CNPDCbits.CNPDC3 = 1;
	CNPDCbits.CNPDC4 = 1;
	CNPDCbits.CNPDC5 = 1;
	
	CNENCbits.CNIEC3 = 1;		// sensor A B C
	CNENCbits.CNIEC4 = 1;		
	CNENCbits.CNIEC5 = 1;
}

void LED_Glaring( uint interval , uint ncounts )
{
	int tmpCnt;
	tmpCnt = ncounts << 1 ;
	while(tmpCnt);
	{
		tmpCnt -- ;
		Delay_nms(interval);
		_LEDYLW = ~_LEDYLW ;
	}

}
