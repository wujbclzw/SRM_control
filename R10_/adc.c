#include<xc.h>
#include "delay.h"
void Init_ADC ()
{
	// set port configration
	ANSELA = ANSELB = ANSELC = 0x0000 ;
	ANSELA = 0x0003 ;	// RA0_AN0  RA1_AN1
	ANSELB = 0x0002 ;	// RB1_AN3
	ANSELC = 0x0007 ;	// RC0 RC1 RC2
	
	//configure ADC register
	AD1CON1 = 0x04E0;	// 12-Bit Operation Mode ; Internal counter ends sampling and starts conversion (auto-convert)
	AD1CON2 = 0x0000;	// Always uses channel input selects for Sample MUXA
	AD1CON3 = 0x0F03;	// 10/3 us per AD sample  if 0x0A01  -- 4/3 us per AD sample 
	AD1CON4 = 0x0000;	// DMA will not be used
	AD1CHS0 = 0x0000;	// select AN0 for input
	AD1CHS123 = 0x0000;		// In 12-bit mode (AD21B = 1), unimplemented and is Read as ‘0’ 
	AD1CSSH = 0x0000;
	AD1CSSL = 0x0000;
	// AD1CON1bits.ADON = 1;	//start ADC sample
}


//{0,371,744,1117,1494,1867,2241,2614,2988,3361,3735,4095}
unsigned int ADdata_RdDWJ()
{
	AD1CHS0 =0X0000;		//ch0 - AN0	
	Delay(10) ;
	AD1CHS0 =0X0000;
	IFS0bits.AD1IF = 0 ;
	AD1CON1bits.ASAM = 1 ; 		//AD sample start 
	while(!IFS0bits.AD1IF) { ; }
	AD1CON1bits.ASAM = 0 ; 		//AD sample stop 
	return  ADC1BUF0 ;
}

unsigned int ADReadCurrent0(  )
{

	AD1CHS0 =0X0001;		//ch0 - AN1	
	Delay(10) ;
	AD1CHS0 =0X0001;
	IFS0bits.AD1IF = 0 ;
	AD1CON1bits.ASAM = 1 ; 		//AD sample start 
	while(!IFS0bits.AD1IF) { ; }
	AD1CON1bits.ASAM = 0 ; 		//AD sample stop 
	return  ADC1BUF0 ;
}


// Amplification factor  1 : 0.005237
unsigned int ADReadCurrentA(  )
{
	AD1CHS0 =0X0008;		//ch0 - AN6	
	Delay(10) ;
	AD1CHS0 =0X0008;		//ch0 - AN6	
	IFS0bits.AD1IF = 0 ;
	AD1CON1bits.ASAM = 1 ; 		//AD sample start 
	while(!IFS0bits.AD1IF) { ; }
	AD1CON1bits.ASAM = 0 ; 		//AD sample stop 
	return  ADC1BUF0 ;
}

unsigned int ADReadCurrentB(  )
{
	AD1CHS0 =0X0007;		//ch0 - AN7	
	Delay(10) ;
	AD1CHS0 =0X0007;
	IFS0bits.AD1IF = 0 ;
	AD1CON1bits.ASAM = 1 ; 		//AD sample start 
	while(!IFS0bits.AD1IF) { ; }
	AD1CON1bits.ASAM = 0 ; 		//AD sample stop 
	return  ADC1BUF0 ;
}

unsigned int ADReadCurrentC(  )
{
	AD1CHS0 =0X0006;		//ch0 - AN8
	Delay(10);
	AD1CHS0 =0X0006;
	IFS0bits.AD1IF = 0 ;
	AD1CON1bits.ASAM = 1 ; 		//AD sample start 
	while(!IFS0bits.AD1IF) { ; }
	AD1CON1bits.ASAM = 0 ; 		//AD sample stop 
	return  ADC1BUF0 ;
}
