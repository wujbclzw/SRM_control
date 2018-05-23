#include <xc.h>
// Initial function for PWM

void Init_PWM(void)
{

	PTCON	= 0x0000;				//1:1fractional frequency,T=0.1us,free running model,                                 
	PTCON2 	= 0x0000 ;				// divide-by-2 
	PTPER 	= 8750;                	//16K - 4375	8K-8750  
	CHOP 	= 0x0000;				//disable chop
	MDC 	= 0x0000;
	
	PWMCON1	= 0x0081;				// 
	PWMCON2	= 0x0081;
	PWMCON3	= 0x0081;
	

	PDC1	= 200;
	PDC2	= 200;
	PDC3	= 200;
	
	PHASE1 	= 200 ;
	PHASE2 	= 200 ;
	PHASE3 	= 200 ;	
	
	// pwm dead-time register and alternate dead-time register
	DTR1 	= 0x2000;  
	DTR2 	= 0x2000;
	DTR3 	= 0x2000;	
	ALTDTR1 = 0x2000;
	ALTDTR2 = 0x2000;
	ALTDTR3 = 0x2000;

	// trigger control register
	TRGCON1	= 0x0000;
	TRGCON2	= 0x0000;	
	TRGCON3	= 0x0000;
	
	// PWM I/O control register
	IOCON1 	= 0xC400 ;			// if 0xC400  PWM1H-PWM  PWM1L-io-low  
	IOCON2 	= 0xC400 ;			// if 0xC940  PWM1H-PWM  PWM1L-io-low  
	IOCON3 	= 0xC400 ;			// if 0xC940  PWM1H-PWM  PWM1L-io-low  
	
	// triggle for ADC operation
	TRIG1 	= 0x0000 ;
	TRIG2 	= 0x0000 ;
	TRIG3 	= 0x0000 ;	
	
	// fault current-limit control register
	FCLCON1	= 0x0003;	//fault input disable
	FCLCON2	= 0x0003;
	FCLCON3	= 0x0003;
	
	// leading=edge blanking control register
	LEBCON1	= 0x0000;
	LEBCON2	= 0x0000;
	LEBCON3	= 0x0000;
	LEBDLY1 = 0x0000;
	LEBDLY2 = 0x0000;
	LEBDLY3 = 0x0000;
	
	// AUXCON auxiliary control register
	AUXCON1	= 0x0000;
	AUXCON2	= 0x0000;
	AUXCON3	= 0x0000;
	
	// PWM interrupt setting
	IEC5bits.PWM1IE 	= 0 ;
	IEC5bits.PWM2IE 	= 0 ;
	IEC6bits.PWM3IE 	= 0 ;
	
	IFS5bits.PWM1IF 	= 0 ;
	IFS5bits.PWM2IF 	= 0 ;
	IFS6bits.PWM3IF 	= 0 ;
	
	//setting for IO port 
	// pwm1/2/3|L/H output open-drain off
	TRISB	&= 0x03FF	; 
	ODCB	&= 0x03FF	;
}

//uint PWMdat1[6] = {0x24,0x21,0x09,0x18,0x12,0x06} ;
//uint PWMdat2[4] = {0xc300,0xf000,0x3c00,0x0f00} ;
/*
   void PWM_test()
   {
// diable PWM
PTCONbits.PTEN=0;
// period
PTPER = 20;
// 
PDC1 = 10;
PDC2 = 20 ;
PDC3 = 30 ;

OVDCON=0X0000;
PTCONbits.PTEN=1;

// start timer 5 for output
Timer5_Start ;
}*/
