#include<xc.h>

void Init_UART()
{
	// Config IO port 
    TRISBbits.TRISB2 = 1 ;	//U1Rx  input
	TRISBbits.TRISB3 = 0 ;	//U1TX  output
	// remapping U1RX to RP34   , U1Tx to RP35
	RPINR18bits.U1RXR = 34;
	_RP35R = 1;		// or RPOR0bits.RP35R = 1 ;
	
	_U1RXIP = 4 ;
	_U1TXIP = 4 ;
    IFS0bits.U1TXIF = 0;    // Clear the Transmit Interrupt Flag
    IEC0bits.U1TXIE = 0;    // Enable Transmit Interrupts
    IFS0bits.U1RXIF = 0;    // Clear the Recieve Interrupt Flag
    IEC0bits.U1RXIE = 0;    // Enable Recieve Interrupts
	
	// See section 19.3.1 of datasheet.
    //  U1BRG = (Fcy/(16*BaudRate))-1
    //  U1BRG = (37M/(16*9600))-1
    //  U1BRG = 240
    U1BRG = 18 ;            // 60Mhz osc, 9600 Baud  389    70 - 455
	U1STA = 0x0000 ;
	U1MODE = 0x0000 ;
    U1MODEbits.UARTEN = 1;  // And turn the peripheral on
    U1STAbits.UTXEN = 1;
}

void SCI_Send_Char(unsigned char ch)
{
	//int cnt = 0;
	while(U1STAbits.UTXBF) 
	{
		;
	}
	if(U1STAbits.UTXBF==0)
	{
		U1TXREG = ch;
	}
}


void SCI_Send_Int(unsigned int num)
{
	unsigned char tmp;
	tmp = (unsigned char)num ;
	SCI_Send_Char(tmp );
	tmp = (unsigned char)(num >> 8 ) ;
	SCI_Send_Char(tmp ) ;
}


void SCI_Send_String( char *s)
{
	while(*s != '\0')
	{
		SCI_Send_Char(*s) ;
		s++ ;
	}
}
