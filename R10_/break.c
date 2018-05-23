#include "global.h"
#include <xc.h>

void StopMotor()
{
	// OVDCON=0x0000;
	IEC1bits.CNIE = 0;	
	PTCONbits.PTEN = 0;
	IOCON1 = 0xC700 ;
	IOCON2 = 0xC700 ;
	IOCON3 = 0xC700 ;
	Timer1_Stop;
	Timer2_Stop;
	Timer3_Stop;
	Timer4_Stop;
	Timer5_Stop;
	TMR1 = 0X0000;
	TMR2 = 0X0000;
	TMR3 = 0X0000;
	TMR4 = 0X0000;
	TMR5 = 0X0000;
	_LEDYLW = 0;
	_LEDRED = 0;
}


void WaitingforStop2()
{
	unsigned int rdSensor, rdSensor2 , lastsensor , PDC_Set;
	unsigned char run_count , stable_flag ;
	unsigned int tmp ;	
	unsigned int rdCurI=0;
	unsigned int cmpCnt ;

	Read_SenSor(lastsensor) ;
	run_count	= 0;
	stable_flag	= 1;
	
	Init_Timer4() ;			//
	TMR4 = 0 ;
	Timer4_Start ;
	
	
	// ------------------------ count the sensor number in 40ms -------------------------
	while(1)
	{	
		do
		{
			Read_SenSor(rdSensor) ;
			if( TMR4 >22000 ) break;
		}
		while( rdSensor == lastsensor );
		if( TMR4 >22000 ) break; 			
		run_count++;
		lastsensor = rdSensor;
		if( run_count > 20 ) run_count = 20;
	}
	
	Timer4_Stop ;
	if(run_count>2) stable_flag = 0;

	
	PDC_Set = 300;
	lastsensor = 0 ;
	PDC1=PDC_Set;
	PDC2=PDC_Set;
	PDC3=PDC_Set;
	
	
	// Init_Timer4() ;
	cmpCnt = 6835 ;   // 100 rmp	   orgin setting 12000 about 57 RPM
	if( 0 == stable_flag )      //stable_flag==0
	{
		// LED2 = 1 ;	
		rdSensor = 0;
		lastsensor = 0;
		PTCONbits.PTEN = 1;			//enalbe PWM
		IOCON1 = 0xC680 ;				//phase A 
		PDC_Set = 800;
		PDC1 = PDC_Set;
		
		//  --------------------------- break motor by phase A -----------------------------
		Init_Timer4() ;
		Init_Timer5() ;
		TMR4 = 0 ;
		Timer4_Start ;
		Timer5_Start ;			//10us tiemr period , deal by check bit value		
		while(1)
		{
			do
			{
				Read_SenSor(rdSensor) ;
				
				if(IFS1bits.T5IF)
				{
					IFS1bits.T5IF = 0 ;
					TMR5 = 0 ;				
					rdCurI = ADReadCurrentA(); 
					if( rdCurI < 600 )   		//	about 3.2A in ideal;  max current reach 8.9A in experiment
					{
						IOCON1 = 0xC680 ;
						_LEDRED = 1 ;
					}
					else
					{
						IOCON1 = 0xC780 ;		// follow current
						_LEDRED = 0 ;
					}
				}
				if(TMR4 > cmpCnt ) break ;
			}while( rdSensor == lastsensor) ;		//hall1.byt == lastsensor
			Delay(8);
			Read_SenSor(rdSensor2) ;			
			
			if( rdSensor2 == rdSensor )		//hall1.byt == hall2.byt
			{	
				tmp = TMR4 ;
				TMR4 = 0;
				if( tmp > cmpCnt ) break ;
				
				//if(tmp < 100)		PDC_Set = 700;
				//else if(tmp < 138)	PDC_Set = 700; 		//  >5000 rpm
				// else if(tmp < 342) 	PDC_Set = 700;         //  >2000 rpm 300
				// else if(tmp < 456) 	PDC_Set  = 00;       	//  >1500 rpm 270
				// else if(tmp < 547) 	PDC_Set  = 00;			//	>1250 rpm 240
				// else if(tmp < 683) 	PDC_Set  = 800;			//	>1000  rpm  180
				// else PDC_Set = 700;
				PDC1	= 700;
				lastsensor = rdSensor ;	// presave current sensor read
			}
		}	
		Timer4_Stop ;
		// Timer5_Stop ;
		IOCON1 = 0xC700;
		
		//------------------------------- break by two phase -----------------------------------
		PDC1=400;
		PDC2=400;
		PDC3=400;
		
		IOCON1 = 0xC680 ;
		IOCON2 = 0xC680 ;
		// 
		Init_Timer4();
		T4CON = 0x0030 ;	// 256/35us
		TMR4=0x0000;
		Timer4_Start ;	
		
		while(1)
		{
			// control current by timer 5 period 10us
			if(IFS1bits.T5IF)
			{
				IFS1bits.T5IF = 0 ;
				TMR5 = 0 ;	
				
				rdCurI = ADReadCurrentA();
				if( rdCurI > 600 )   		//	about 2A
				{
					IOCON1 = 0xC680 ;
				}
				else
				{
					IOCON1 = 0xC780 ;		// follow current
				}
			
				rdCurI = ADReadCurrentB();				
				if( rdCurI > 600 )   		//	about 2A
				{
					IOCON1 = 0xC680 ;
				}
				else
				{
					IOCON1 = 0xC780 ;		// follow current
				}
			}
			if(TMR4 > 27344 ) break ;		//hold for 0.2s
		}
		Timer4_Stop ;
		Timer5_Stop ;
	}
	StopMotor() ;
}