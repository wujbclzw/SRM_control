#include <xc.h>


void EnableInterrupts(void)
{
	/* Enable level 1-7 interrupts */
	/* No restoring of previous CPU IPL state performed here */
	INTCON2bits.GIE = 1;
	return;
}
void DisableInterrupts(void)
{
	/* Disable level 1-7 interrupts */
	/* No saving of current CPU IPL setting performed here */
	INTCON2bits.GIE = 0;
	return;
}
void InitInterrupts(void)
{
	/* Interrupt nesting enabled here */
	INTCON1bits.NSTDIS = 1;
	
	/* Set Timer1 interrupt priority to 2 */
	IPC0bits.T1IP = 2; 
	/* Set Timer2 interrupt priority to 5 */
	IPC1bits.T2IP = 3;
	/* Set Timer3 interrupt priority to 6 (level 7 is highest) */
	IPC2bits.T3IP = 4;
	/* Set Timer4 interrupt priority to 3 */
	IPC6bits.T4IP = 3; 
	
	/* Set Change Notice interrupt priority to 4 */
	IPC4bits.CNIP = 7;
	
	/* Reset Timer1 interrupt flag */
	IFS0bits.T1IF = 0;
	/* Reset Timer2 interrupt flag */
	IFS0bits.T2IF = 0;
	/* Reset Timer3 interrupt flag */
	IFS0bits.T3IF = 0;
	/* Reset Timer4 interrupt flag */
	IFS1bits.T4IF = 0;
	/* Reset change notice interrupt flag */
	IFS1bits.CNIF = 0;
	
	/* Enable Timer1 interrupt */
	IEC0bits.T1IE = 0;
	/* Enable Timer2 interrupt (PWM time base) */
	IEC0bits.T2IE = 0;
	/* Enable Timer3 interrupt */
	IEC0bits.T3IE = 0;
	/* Enable Timer4 interrupt (replacement for Timer2 */
	IEC1bits.T4IE = 0;
	/* Enable CN interrupts */
	IEC1bits.CNIE = 0;
	
	return;
}

void Init_System( void )
{
	//
	SR 		= 0x0000 ;
	CORCON 	= 0x0000;
	INTCON1 = 0x0000;
	INTCON2 = 0x0000;	//disable all Interrupt
	INTCON3 = 0x0000;
	INTCON4 = 0x0000;
	INTTREG = 0x0000;
	//disable all interrupt
	IEC0 = 0x0000 ;
	IEC1 = 0x0000 ;
	IEC2 = 0x0000 ;
	IEC3 = 0x0000 ;
	IEC4 = 0x0000 ;
	IEC5 = 0x0000 ;
	IEC6 = 0x0000 ;
	IEC8 = 0x0000 ;
	IEC9 = 0x0000 ;	

	IFS0 = 0x0000 ;
	IFS1 = 0x0000 ;
	IFS2 = 0x0000 ;
	IFS3 = 0x0000 ;
	IFS4 = 0x0000 ;
	IFS5 = 0x0000 ;
	IFS6 = 0x0000 ;
	IFS8 = 0x0000 ;
	IFS9 = 0x0000 ;	

	//setting the interrupt using in program
	InitInterrupts();	
}


// set systme clock
void Init_Clock( void )
{

	PLLFBD = 54;                        // M = 60
	CLKDIVbits.PLLPRE = 0;              // N1 = 2
	CLKDIVbits.PLLPOST = 0;             // N2 = 2
	OSCTUN = 0;
	RCONbits.SWDTEN = 0;

	// Clock switch to incorporate PLL
	__builtin_write_OSCCONH( 0x03 );    // Initiate Clock Switch to

	// External oscillator with PLL (NOSC=0b011)
	__builtin_write_OSCCONL( OSCCON || 0x01 );  // Start clock switching
	while( OSCCONbits.COSC != 0b011 );

	// Wait for Clock switch to occur
	while( OSCCONbits.LOCK != 1 )
	{ };
}
