#include "global.h"
#include <xc.h>
// FICD  Communicate on PGEC1 and PGED1 & JTAG is disabled
_FICD( ICS_PGD1 & JTAGEN_OFF);
// FPOR    I2C1 mapped to SDA1/SCL1 pins & I2C2 mapped to SDA2/SCL2 pins & WDT Window is 25% of WDT period
_FPOR( ALTI2C1_OFF & ALTI2C2_OFF & WDTWIN_WIN25 ) ;
// FWDT  Watchdog Timer Postscaler bits (1:32,768) & Prescaler bit (1:128) & ..
_FWDT(WDTPOST_PS32768 & WDTPRE_PR128 & PLLKEN_ON & WINDIS_OFF & FWDTEN_OFF);
// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF & PWMLOCK_OFF);
// Enable Clock Switching and Configure Primary Oscillator in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT & IOL1WAY_OFF);
// General Segment Write-Protect bit off  & code-protect off
_FGS(GWRP_OFF & GCP_OFF);


#define		MaxPWMDuty  		2000			//8750
#define		MinPWMDuty			30
#define 	MAX_CURRENT_LMT		3000	
#define 	CNT_Number 			6
#define		SpeedDefine			1			// 1-R1000 2-R200 3-R3000 4-R4000

// #define		SCI_SEND_DAT

void 			StopMotor() ;
void 			WaitingforStop2() ;

// global variables

/*

//unsigned int	CurSpeedL;
//int			CurSpeedH;
unsigned char 	bRenewPWMflag;

unsigned int 	nCntTimerH;
unsigned int 	nCntTimerL;


// for timer check motor block
unsigned char 	Lock_Count;
unsigned char 	Lock_Flag;
unsigned char 	T1IntFlag;

unsigned char 	Run_back_flag ;
unsigned int 	lastSensor1 ;
unsigned int 	lastSensor2 ;





int 			glb_TimeCnt , glb_Position ;
*/
// 控制每一相的电流 先上升（1/3周期） 再下降（1/3周期） 再保持零（1/3周期） 
// 每一相都按这样的电流曲线来执行  
// 遇到堵转时 增大电流值
// 判断正反相
// 不检测位置传感器，遮光片的值

int main()
{		
	// variable define
	int 	rdAD ;
	// long 	T2Ary[CNT_Number+2] ;
	// unsigned int pt2 ; 
	
	// Initial device
	Init_Clock() ;
	DisableInterrupts() ;
	Init_System();		// close all interrupt and clear all interrupt flag
	Init_IO_Port() ;
	Init_QEI() ;
	Init_ADC () ;
	Init_UART() ;
	Init_Timer1 () ;
	Init_Timer23 () ;		// set timer 2 and 3 as an 32-bits timer
	Init_Timer4();
	Init_Timer5();
	Init_PWM();
	AD1CON1bits.ADON = 0;	//disable ADC
	PTCONbits.PTEN = 0 ;	//disable PWM

	// charge capacitor 
	_LEDRED = 1 ;
	_ElecRelay = 0 ;
	Delay_nms(1000);
	_ElecRelay = 1 ;
	Delay_nms(1000);
	_LEDRED = 0 ;

	/////////////////////////////////////////////////
	AD1CON1bits.ADON = 1;	// enable ADC
	EnableInterrupts() ;	// enable Interrupt 
	
	while(1)
	{	
	
		int 	nTmp  , PDC_set1 , PDC_set2 , PDC_set3 ;
		int 	I_CtrlP2A , I_CtrlP2B , I_CtrlP2C ;
		int		CurphA , CurphB , CurphC ;
		int 	phAflg , phBflg ,phCflg ;
		int				pt ;				//, nTurns
		
		int		 		PDC_Set ;
		int 			I_motor ;
		
		unsigned int 	lastSensor , nSensorChange ;
		unsigned char	RunDirection , nRunAllFlg ;
		
		
		//int		CurrentAry[50] = {	153,216,265,305,340,371,399,424,447,468,487,504,520,534,547,558,568,576,583,589,594,597,599,600,
		//							599,597,594,589,583,576,568,558,547,534,520,504,487,468,447,424,399,371,340,305,265,216,153,0,0} ;
		// int 		CurrentAry[110] = {	102,144,177,204,228,250,269,288,305,321,336,350,364,377,390,401,413,424,434,444,454,463,472,481,
										// 489,497,504,511,518,525,531,537,543,548,553,558,562,567,571,574,578,581,584,587,589,591,593,595,
										// 596,597,598,599,599,600,599,599,598,597,596,595,593,591,589,587,584,581,578,574,571,567,562,558,
										// 553,548,543,537,531,525,518,511,504,497,489,481,472,463,454,444,434,424,413,401,390,377,364,350,
										// 336,321,305,288,269,250,228,204,177,144,102,  0,} ;
										
		// int  CurrentAry[110] = {11, 22, 33, 44, 55, 66, 77, 88,100,111,122,133,144,155,166,177,188,200,211,222,233,244,255,266,277,288,300,311,322,
								// 333,344,355,366,377,388,400,411,422,433,444,455,466,477,488,500,511,522,533,544,555,566,577,588,600,600,588,577,566,
								// 555,544,533,522,511,500,488,477,466,455,444,433,422,411,400,388,377,366,355,344,333,322,311,300,288,277,266,255,244,
								// 233,222,211,200,188,177,166,155,144,133,122,111,100, 88, 77, 66, 55, 44, 33, 22, 11,  0,  0} ;
		// 三角形电流曲线		
		int  CurrentAry[112] = {   7, 14, 22, 29, 37, 44, 51, 59, 66, 74, 81, 88, 96,103,111,118,125,133,140,148,155,162,170,177,185,192,200,207,214
								,222,229,237,244,251,259,266,274,281,288,296,303,311,318,325,333,340,348,355,362,370,377,385,392,400,400,392,385,377
								,370,362,355,348,340,333,325,318,311,303,296,288,281,274,266,259,251,244,237,229,222,214,207,200,192,185,177,170,162
								,155,148,140,133,125,118,111,103, 96, 88, 81, 74, 66, 59, 51, 44, 37, 29, 22, 14,  7,  0,  0,  0,  0} ;
								
		// int  CurrentAry[112] = {  0,0,1,3,5,8,12,16,21,26,32,39,46,54,62,71,80,90,99,110,120,131,142,153,165,176,188,199,
								// 211,223,234,246,257,268,279,289,299,309,319,328,337,345,353,360,367,373,378,383,387,391,394,396,398,399,400,
								// 399,398,396,394,391,387,383,378,373,367,360,353,345,337,328,319,309,300,289,279,268,257,246,234,223,211,200,
								// 188,176,165,153,142,131,120,110,99,90,80,71,62,54,46,39,32,26,21,16,12,8,5,3,1,0,0,0,0,0} ;

		// start condition
		StopMotor() ;
		// WaitingforStop2();
		
		// 过压 欠压保护
		while( Voltage_Protect )
		{
			_LEDYLW = 1 ;   Delay_nms(200) ;  _LEDYLW = 0 ; Delay_nms(200) ;
		}
		
		// read AD and ready to run
		rdAD = ADdata_RdDWJ() ;
		while(rdAD > 120){ rdAD = ADdata_RdDWJ() ;	 _LEDRED=1; _LEDYLW=0 ;}		// ready for frist running
		while(rdAD < 240){ rdAD = ADdata_RdDWJ() ;	 _LEDYLW=1; }
		
		// choose direction
		RunDirection = DirectionBit ;
		
		// for(pt2 = 0 ; pt2 < CNT_Number ; pt2 ++ ) T2Ary[pt2] = 5000 ; 		
		// pt2 = 0 ;
		/// ----------------------- Start Motor -------------
		// perpare to start motor
		PDC_Set = 80 ;				//important for inverse running
		PDC1 = PDC_Set;
		PDC2 = PDC_Set;
		PDC3 = PDC_Set;
		
		PDC_set1 = PDC_Set ;
		PDC_set2 = PDC_Set ;
		PDC_set3 = PDC_Set ;	
		
		
		//glb_TimeCnt = 50;
		//glb_Position = 10 ;
		
		phAflg = 1 ;
		phBflg = 2 ;
		phCflg = 3 ;
		
		pt = 0 ;
	//	nTurns = 0 ;
		
		//I_CtrlP2A = 150 ;
		//I_CtrlP2B = 100 ;
		//I_CtrlP2C = 100 ;

		I_CtrlP2A = 250 ;
		I_CtrlP2B = 200 ;
		I_CtrlP2C = 200 ;
		
		I_motor = 300 ;
		
	//	Lock_Flag = 0 ;
	//	Lock_Count = 0 ;
	//	T1IntFlag = 0 ;

		lastSensor = 0 ;
		nRunAllFlg = 0;
		nSensorChange = 0;
		
		// Set_Timer1_Period25_MS ;
		//TMR1 = 0 ;
		
		IOCON1 = 0xC680 ;
		IOCON2 = 0xC680 ;
		IOCON3 = 0xC680 ;	
		
		// Timer1_Start ;
		//Timer23_Start ;
		Timer5_Start ;				// current limit
		Timer4_Start ;				// control circle
		
		IFS1bits.T4IF = 1 ;
		IFS1bits.T5IF = 1 ;
		// IFS1bits.CNIF = 1;
		// IEC1bits.CNIE = 1;			// change phase
		
		PTCONbits.PTEN = 1;			// enalbe PWM
		
		while(1)
		{	
		
			Read_SenSor(nTmp);
			if (nTmp != lastSensor)
			{
				//SCI_Send_Char(nTmp);

				//SCI_Send_Char(pt);
				nSensorChange++;
				nRunAllFlg |= (1 << nTmp);
				lastSensor = nTmp;
			}
			// Timer 4 interrupt
			if(IFS1bits.T4IF)		// check Timer4 interrupt 
			{			

				if( pt >  110 ) 		IOCON1 = 0xC700 ;
				else 				{	IOCON1 = 0xC680 ;		CurphA =  CurrentAry[pt] ;	}
				
				if( RunDirection == 1)
				{
					// phase B
					pt += 54 ;
					if( pt >= 162 ) 		pt -= 162 ;
					if( pt >  110 ) 		IOCON2 = 0xC700 ;
					else 				{	IOCON2 = 0xC680 ;		CurphB =  CurrentAry[pt] ;	}
						
					//phase C
					pt += 54 ;			
					if( pt >= 162 ) 		pt -= 162 ;
					if( pt >  110 ) 	 	IOCON3 = 0xC700 ;
					else 				{	IOCON3 = 0xC680 ;		CurphC =  CurrentAry[pt] ;	}
				}
				else
				{
					// phase C
					pt += 54 ;
					if( pt >= 162 ) 		pt -= 162 ;
					if( pt >  110 ) 		IOCON3 = 0xC700 ;
					else 				{	IOCON3 = 0xC680 ;		CurphC =  CurrentAry[pt] ;	}
						
					//phase B
					pt += 54 ;			
					if( pt >= 162 ) 		pt -= 162 ;
					if( pt >  110 ) 	 	IOCON2 = 0xC700 ;
					else 				{	IOCON2 = 0xC680 ;		CurphB =  CurrentAry[pt] ;	}
				}
				// 总的来说相当于 pt++
				pt += 55 ;
				if(pt >= 162) pt -= 162 ;
				
				
				if(pt==0)
				{
					if( ( nRunAllFlg != 0x7E) && (I_CtrlP2A < 1200) )
					{
						// blocked 电机堵转
						I_CtrlP2A += 80 ;
						I_CtrlP2B += 80 ;
						I_CtrlP2C += 80 ;
					}
					if ( ( nSensorChange > 8 ) && (I_CtrlP2A > 100) )
					{
						I_CtrlP2A -= 15 ;
						I_CtrlP2B -= 15 ;
						I_CtrlP2C -= 15 ;
					}
					nRunAllFlg = 0 ;
					nSensorChange = 0 ;
				}
				
				IFS1bits.T4IF = 0 ;
			}

			// 电流控制  定时器频率 16K
			if(IFS1bits.T5IF)			
			{
				int nLowA , nHighA ;
				int nLowB , nHighB ;
				int nLowC , nHighC ;
				IFS1bits.T5IF = 0 ;
				
				// current  PID 				
				// phase A	// SCI_Send_Int (nTmp) ;
				if( 0xC680 == IOCON1 )
				{
					nTmp = ADReadCurrentA() ;
					
					// SCI_Send_Int (nTmp) ;
					
					if(0 == phAflg) {	nHighA = 0 ; 	nLowA = 0 ;	}
					phAflg ++ ;
					
					I_motor = CurphA ;
					if( nTmp < ( I_motor + I_CtrlP2A ) ) 	{	PDC1 = PDC_set1 ;   	nLowA ++ ; }
					else 					{	PDC1 = (PDC_set1>>2) ;  nHighA ++ ;  }
					
					if(phAflg == 7)
					{
						if	( nHighA <  nLowA )	PDC_set1 += (I_motor>>1) ;
						else 					PDC_set1 -= (I_motor>>1) ;
						phAflg = 0 ;
					}
					if( PDC_set1 < MinPWMDuty )  	PDC_set1 = MinPWMDuty ;
					else if(PDC_set1 > MaxPWMDuty)	PDC_set1 = MaxPWMDuty ;
					
					if(phBflg)
					{
						if( 0xC680 != IOCON2 ) 
						{
							phBflg = 0 ;
							//PDC_set2 = ( I_motor>>1 )  ;
						}
					}
				}			
				
				// phase B
				if( 0xC680 == IOCON2 )
				{
					nTmp = ADReadCurrentB() ;
					
					if(0 == phBflg) {	nHighB = 0 ; 	nLowB = 0 ;	}
					phBflg ++ ;
					
					I_motor = CurphB ;
					if (nTmp < (I_motor + I_CtrlP2B))
					{
						PDC2 = PDC_set2;
						nLowB++;
					}
					else
					{
						PDC2 = (PDC_set2 >> 2);
						nHighB++;
					}

					if(phBflg == 7)
					{
						if	( nHighB <  nLowB )	PDC_set2 += (I_motor>>1) ;
						else 					PDC_set2 -= (I_motor>>1) ;
						phBflg = 0 ;
					}
					
					if( PDC_set2 < MinPWMDuty )  	PDC_set2 = MinPWMDuty ;
					else if(PDC_set2 > MaxPWMDuty)	PDC_set2 = MaxPWMDuty ;
					
					if(phCflg)
					{
						if( 0xC680 != IOCON3 ) 
						{
							phCflg = 0 ;
							//PDC_set3 = ( I_motor>>1 )  ;
						}
					}
				}
				
				// phase C	
				
				if( 0xC680 == IOCON3 )
				{
					nTmp = ADReadCurrentC() ;
					// SCI_Send_Int (nTmp) ;
					// current  PID 
					if(0 == phCflg) {	nHighC = 0 ; 	nLowC = 0 ;	}
					phCflg ++ ;
					
					I_motor = CurphC ;
					if( nTmp < ( I_motor + I_CtrlP2C ) ) 	{	PDC3 = PDC_set3 ;   	nLowC ++ ; }
					else 					{	PDC3 = (PDC_set3>>2) ;  nHighC ++ ;  }
					
					if(phCflg == 7 )
					{
						phCflg = 0 ;
						if	( nHighC <  nLowC )	PDC_set3 += (I_motor>>1) ;
						else 					PDC_set3 -= (I_motor>>1) ;						
					}
					
					if( PDC_set3 < MinPWMDuty )  	PDC_set3 = MinPWMDuty ;
					else if(PDC_set3 > MaxPWMDuty)	PDC_set3 = MaxPWMDuty ;
					
					if(phAflg)
					{
						if( 0xC680 != IOCON1 ) 
						{
							phAflg = 0 ;
							//PDC_set1 = ( I_motor>>1 )  ;
						}
					}
				}
			}

			
			//quit condition check
			rdAD = ADdata_RdDWJ();

			if (rdAD < 160)
				break;
			// check for reverse runing
			if(DirectionBit != RunDirection ) 	{	RunDirection = DirectionBit ;	break;		}
			// check for voltage 
			if( Voltage_Protect ) 	break ;
			// check for motor block
			// if( T1IntFlag )   break ;			
		}
	}
	return 0 ;
};

