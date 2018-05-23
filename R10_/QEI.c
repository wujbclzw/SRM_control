#include <xc.h>

void Init_QEI( void )
{
	RPINR14bits.QEA1R = 24;
	RPINR14bits.QEB1R = 36;	

	RPINR15bits.INDX1R = 20;	
	//	RPINR15bits.HOME1R = 20;	
	
	QEI1CON = 0x8300 ;
	QEI1IOC = 0x0000 ;
	QEI1STAT= 0x0000 ; 	// disable all interrupt
	
	POS1HLD = 0 ;			//position counter
	POS1CNTL = 0 ;

	VEL1CNT = 0;			//velocity counter register
	
	INDX1HLD = 0 ;			
	INDX1CNTL = 0 ;

	QEI1ICH = 0 ;
	QEI1ICL = 0 ;
	
	QEI1LECH = 0 ;
	QEI1LECL = 0 ;
	
	QEI1GECH = 0 ;
	QEI1GECL = 0 ;
	
	INT1TMRH = 0 ;
	INT1TMRL = 0 ;
	
	INT1HLDH = 0 ;
	INT1HLDL = 0;
}
