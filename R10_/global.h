#ifndef __GLOBAL_H__
    #define __GLOBAL_H__


// #include <stdint.h>
// #include <math.h>
// #include <dsp.h>
// #include <stdio.h>
// #include <float.h>
// #include <libpic30.h>

// #if __XC16_VERSION == 1011
// #warning "XC16 v1.11 detected. It is recommended that a newer version of XC16 be used."
// #endif


#include "io.h"
#include "adc.h"
#include "uart.h"
#include "delay.h"
#include "Timer.h"
#include "QEI.h"
#include "sys.h"
#include "pwm.h"


// ------ data type define
#define	uint	unsigned int
#define uint16	unsigned int 	
#define uchar	unsigned char 	


// led define
#define LED_RED		0
#define LED_YLW		1
#define LED_OUT		2
  
#define _LEDRED 	PORTAbits.RA10
#define _LEDYLW 	PORTAbits.RA7
#define _LEDOUT 	PORTBbits.RB5

// relay 
#define _ElecRelay	PORTAbits.RA8 
// steering direction
#define DirectionBit	PORTCbits.RC7
// 过压欠压
#define Voltage_Protect		PORTCbits.RC8




// Tiemr define 
//Timer define
#define Timer1_Start T1CONbits.TON = 1
#define Timer2_Start T2CONbits.TON = 1
#define Timer23_Start T2CONbits.TON = 1
#define Timer3_Start T3CONbits.TON = 1
#define Timer4_Start T4CONbits.TON = 1
#define Timer5_Start T5CONbits.TON = 1

#define Timer1_Stop  T1CONbits.TON = 0
#define Timer2_Stop  T2CONbits.TON = 0
#define Timer23_Stop  T2CONbits.TON = 0
#define Timer3_Stop  T3CONbits.TON = 0
#define Timer4_Stop  T4CONbits.TON = 0
#define Timer5_Stop  T5CONbits.TON = 0


// read sensor 

#define Read_SenSor(x)  { x = (PORTC) & 0x0038 ;	x >>= 3; 	}



#endif