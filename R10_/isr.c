#include<xc.h>
#include"global.h"
/*
unsigned char chr ;
extern unsigned int 	nMAXSensorCnt;	
extern unsigned int 	nSensorCount;

extern unsigned int 	nADStableCnt ;*/
/*
extern int 				nCntTimerH;
extern int 				nCntTimerL;

extern int 				CurSpeedL;
extern int 				CurSpeedH;
extern unsigned char 	bRenewPWMflag;
extern unsigned char	RunDirection ;


extern unsigned char 	Lock_Count;
extern unsigned char 	Lock_Flag;
extern unsigned char	T1IntFlag;
extern int 				PDC_Set ;
extern int 				I_motor ;
extern int 				I_CtrlP2A , I_CtrlP2B , I_CtrlP2C ;
extern unsigned char 	Run_back_flag ;

extern unsigned int 	lastSensor1 ;
extern unsigned int 	lastSensor2 ;

extern int	 glb_TimeCnt , glb_Position ;
*/

void __attribute__((interrupt , no_auto_psv)) _T1Interrupt (void)
{
	// Timer1_Stop  ;
	TMR1 = 0X0000;
	IFS0bits.T1IF = 0;
	
	// glb_TimeCnt ++ ;
	// Timer1_Stop  ;
	// IFS0bits.T1IF=0;

	// Lock_Count++;
	// Lock_Flag = 1;
	// if( Lock_Count < 50 )
	// {
		// I_motor += 50;	
		// if(I_motor > 1000) 	I_motor = 1000 ;
		// else if( I_motor < 50) I_motor = 50 ;
		// PDC1 = PDC_Set;
		// PDC2 = PDC_Set;
		// PDC3 = PDC_Set;	
		// Timer1_Start ;
	// }
	// else
	// {
		// IOCON1 = 0xC700 ;
		// IOCON2 = 0xC700 ;
		// IOCON3 = 0xC700 ;
		// T1IntFlag = 1 ; 
	// }

	
}

void __attribute__ ( (interrupt, no_auto_psv) ) _T3Interrupt( void )
{
	IFS0bits.T3IF = 0;
	T2CONbits.TON = 0;

	_LEDRED = ~_LEDRED ;

	TMR3HLD = 0;
	TMR2 = 0 ;
	T2CONbits.TON = 1;

}


void __attribute__ ( (interrupt, no_auto_psv) ) _T4Interrupt( void )
{
	TMR4 = 0 ;
	T4CONbits.TON = 1;
	IFS1bits.T4IF = 0;
}


void __attribute__((__interrupt__,no_auto_psv)) _CNInterrupt(void)
{
	// Insert ISR Code Here	
	// bRenewPWMflag = 1;
	/*unsigned int sensor ;	
	sensor = (PORTC) & 0x0038;
	sensor >>= 3 ;
	
	if( sensor == lastSensor2  ) 
	{
		glb_Position -- ;
		lastSensor1 = sensor ;
		lastSensor2 = 0 ;
	}
	else
	{
		lastSensor2 = lastSensor1 ;
		lastSensor1 = sensor ;
		glb_Position ++ ;
	}*/
	
	// two phase on
	// I_CtrlP2A = 0 ;
	// I_CtrlP2B = 0 ;
	// I_CtrlP2C = 0 ;
		/*
	if(RunDirection)
	{
		switch(sensor)
		{
			case 1 :
				{
					I_CtrlP2C = ( I_motor >> 1 ) ;
				}
			case 5 :
				{
					// C 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC680 ;
					break;
				}
			case 4 :
				{
					I_CtrlP2A = ( I_motor >> 1 ) ;
				}
			case 6 :
				{
					// A 
					IOCON1 = 0xC680 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC700 ;
					break;
				}
			case 2 :
				{
					I_CtrlP2B = ( I_motor >> 1 ) ;
				}
			case 3 :
				{
					// B
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC680 ;
					IOCON3 = 0xC700 ;
					break;
				}

			default:
				{
					// shut all 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC700 ;
					break;
				}
		}
	}
	else
	{
		switch(sensor)
		{
			case 5 :
				{
					I_CtrlP2A = ( I_motor >> 1 ) ;
				}
			case 1 :
				{
					// A 
					IOCON1 = 0xC680 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC700 ;
					break;
				}
			case 6 :
				{
					I_CtrlP2B = ( I_motor >> 1 ) ;
				}
			case 4 :
				{
					// B
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC680 ;
					IOCON3 = 0xC700 ;
					break;
				}
			case 3 :
				{
					I_CtrlP2C = ( I_motor >> 1 ) ;
				}
			case 2 :
				{
					// C 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC680 ;
					break;
				}

			default:
				{
					// shut all 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC700 ;
					break;
				}
		}
	}
	*/
		
	// three phase on 
	/*
	if(RunDirection)
	{
		switch(sensor)
		{
			case 1 :
				{
					// BC 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC680 ;
					IOCON3 = 0xC680 ;
					break;
				}
			case 3 :
				{
					// B 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC680 ;
					IOCON3 = 0xC700 ;
					//I_CtrlP2B = ( I_motor>>1 ) ;
					break;
				}
			case 2 :
				{
					// AB 
					IOCON1 = 0xC680 ;
					IOCON2 = 0xC680 ;
					IOCON3 = 0xC700 ;
					break;
				}
			case 6 :
				{
					// A
					IOCON1 = 0xC680 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC700 ;
					//I_CtrlP2A = ( I_motor>>1 ) ;
					break;
				}
			case 4 :
				{
					// AC 
					IOCON1 = 0xC680 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC680 ;
					break;
				}
			case 5 :
				{
					// C
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC680 ;
					//I_CtrlP2C = ( I_motor>>1 ) ;
					break;
				}

			default:
				{
					// shut all 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC700 ;
					break;
				}
		}
	}
	else
	{
		switch(sensor)
		{
			case 1 :
				{
					// A 
					IOCON1 = 0xC680 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC700 ;
					break;
				}
			case 3 :
				{
					// AC 
					IOCON1 = 0xC680 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC680 ;
					break;
				}
			case 2 :
				{
					// C 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC680 ;
					break;
				}
			case 6 :
				{
					// BC
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC680 ;
					IOCON3 = 0xC680 ;
					break;
				}
			case 4 :
				{
					// B 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC680 ;
					IOCON3 = 0xC700 ;
					break;
				}
			case 5 :
				{
					// AB
					IOCON1 = 0xC680 ;
					IOCON2 = 0xC680 ;
					IOCON3 = 0xC700 ;
					break;
				}

			default:
				{
					// shut all 
					IOCON1 = 0xC700 ;
					IOCON2 = 0xC700 ;
					IOCON3 = 0xC700 ;
					break;
				}
		}
	}*/
	
	// nCntTimerL = TMR2 ;
	// nCntTimerH = TMR3HLD;
	// TMR3HLD = 0 ;
	// TMR2 = 0 ;
	// bRenewPWMflag = 1; 
	
	IFS1bits.CNIF = 0;
}



void __attribute__ ( (interrupt, no_auto_psv) ) _U1RXInterrupt( void )
{
	// chr = U1RXREG;
	IFS0bits.U1RXIF = 0;
}


void __attribute__ ( (interrupt, no_auto_psv) ) _U1TXInterrupt( void )
{
	IFS0bits.U1TXIF = 0;
}
