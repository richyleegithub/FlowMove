//======================================================
// Main program routine
// - Device name  : MC96FD316
// - Package type : 28TSSOP
//======================================================
// For XDATA variable : V1.041.00 ~
#define		MAIN	1


// Generated    : Wed, Apr 20, 2022 (17:28:48)
#include	"MC96FD316.h"
#include	"func_def.h"
#include 	"drv_led.h"
#include 	"drv_key.h"
#include 	"drv_battery.h"
#include 	"drv_motor.h"
#include 	"drv_adc.h"
#include 	"drv_timer.h"
#include 	"drv_uart.h"
#include 	"sys_manage.h"
#include 	"sys_message.h"
#include 	"drv_charge.h"
#include    "utiles.h"




uint32_t  systme_tick;
void main()
{
	cli();          	// disable INT. during peripheral setting
	port_init();    	// initialize ports
	clock_init();   	// initialize operation clock
	ADC_init();     	// initialize A/D convertor
	ExINT_init();   	// initialize external interrupt
	OPAMP_init();   	// initialize OPAMP
	Timer0_init();  	// initialize Timer0
	//Timer1_init();  	// initialize Timer1
	Timer2_init();  	// initialize Timer2
	Timer3_init();  	// initialize Timer3
	

	UART_init();    	// initialize UART interface


	//WDT_init();     	// initialize Watch-dog timer
	drv_adc_ini();
	drv_led_ini();
	drv_charge_ini();
	drv_battery_ini();
	drv_uart_ini();
	ini_message();
	Init_evTimers();
	sys_process_ini();
	sei();          	// enable INT.
	

	while(1){
		drv_adc();
		drv_comm();
#ifndef 	UART_TX_INT
		drv_uart_tx();
#endif

		
		drv_key();
		drv_led();
		drv_motor();
		Drv_Charge();
		drv_battery();
		drv_time();
	
		sys_process();
		
	}
}

//======================================================
// interrupt routines
//======================================================





void INT_Ext11() interrupt 11
{
	// External interrupt 11
		motor_FG_pluse++;
}

void INT_Timer0(void) interrupt 12
{
	// Timer0 interrupt
	// about every 1ms to generate interrupt.

	 drv_key_tick++;
	 drv_timer_tick++;
	 drv_motor_tick++;
	 drv_battery_tick++;
	 drv_charge_tick++;
	 drv_led_tick++;
	 rx_timeout++;
	 rx_byte_timeout++;
	 adc_timer_tick++;
}
#if 0
//======================================================
// peripheral setting routines
//======================================================

unsigned char UART_read()
{
	unsigned char dat;
	
	while(!(USIST1 & 0x20));	// wait
	dat = USIDR;    	// read
	return	dat;
}

unsigned int ADC_read()
{
	// read A/D convertor
	unsigned int adcVal;
	
	while(!(ADCCRL & 0x10));	// wait ADC busy
	adcVal = ADCDRH << 8;	// read ADC high
	adcVal |= ADCDRL;	// read ADC low
	return	adcVal;
}
#endif


void ADC_init()
{
	// initialize A/D convertor
	ADCCRL = 0x80;  	// setting
	ADCCRH = 0x07;  	// trigger source, alignment, frequency
}
#if 0
void ADC_start(unsigned char ch)
{
	// start A/D convertor
	ADCCRL = (ADCCRL & 0xf0) | (ch & 0x0f);	// select channel
	ADCCRL |= 0x40; 	// start ADC
}
#endif

void ExINT_init()
{
	// initialize external interrupt
	EIPOL0H = 0x00; 	// edge : external INT.6~4
	EIPOL0L = 0x00; 	// edge : external INT.3~0
	EIPOL1 = 0x01;  	// edge : external INT.13~11

	IE1 |= 0x20;    	// Enable Ext.INT.11
}

void OPAMP_init()
{
	// initialize OPAMP
	AMPCR = 0x01;   	// enable OPAMP
}

void Timer0_init()
{
	// initialize Timer0
	// 8bit timer, period = 0.998400mS
	T0CR = 0x0B;    	// timer setting
	T0DR = 0x26;    	// period count

	IE2 |= 0x01;    	// Enable Timer0 interrupt
	T0CR |= 0x80;   	// enable counter
}

void Timer2_init()
{
	// initialize Timer2
	// 16bit PWM, period = 0.050000mS ( 20000.000000Hz )
	//     PWM duty = 50.000000%
	T2CRH = 0x20;   	// PWM setting High, 1 shot mode
	T2CRH |= 0x30;  	// PWM repeat mode
	//T2CRL = 0xC4;   	// PWM setting Low
	T2CRL = 0xC0;   	// PWM setting Low
	T2BDRH = 0x00;  	// duty High
	T2BDRL = 0x00;  	// duty Low
	T2ADRH = 0x03;  	// period count High
	T2ADRL = 0xE7;  	// period count Low
	//T2CRL |= 0x08;	// Disable reload
	//T2CRH |= 0x80;  	// enable counter
}

void Timer3_init()
{
	// initialize Timer3
	// 16bit PWM, period = 1.000000mS ( 1000.000000Hz )
	//     PWM duty A,B,C = 50.000000%, 0.020000%, 99.360000%
	T3ADRH = 0x0;  		// duty High
	T3ADRL = 0x00;  	// duty Low

	T3BDRH = 0x00;  	// duty High
	T3BDRL = 0x00;  	// duty Low
	
	T3CDRH = 0x0;  		// duty High
	T3CDRL = 0x0;  		// duty Low

	//T3DLY = 0xXX; 	// PWM output delay
	T3OUTCRH = 0xC6;	// PWM output control High
	//T3OUTCRL = 0xXX;	// PWM output control Low
	T3PDRH = 0x4E;  	// period count High
	T3PDRL = 0x1F;  	// period count Low
	//T3ADTDR = 0xXX;	// ADC trigger generator
	//T3ADTCR = 0xXX;	// ADC trigger control
	//T3CRL = 0xXX; 	// timer control Low
	T3CRH = 0x3D;   	// timer control High
	T3CRH |= 0x80;  	// enable counter






}

void UART_init()
{
	// initialize UART interface
	// UART : ASync. 9615bps N 8 1
	USICR2 = 0x02;  	// activate USI
	USICR1 = 0x06;  	// Async/Sync, bit count, parity
	USICR2 |= 0xAC; 	// interrupt, speed
	USICR3 = 0x00;  	// stop bit
	USIBD = 0x81;   	// baud rate
	IE1 |= 0x08;    	// enable UART Rx interrupt
#ifdef UART_TX_INT			
	IE1 |= 0x10;		// enable UART Tx interrupt

#endif

}
#if 0
void UART_write(unsigned char dat)
{
	while(!(USIST1 & 0x80));	// wait
	USIDR = dat;    	// write
}
#endif

void WDT_init()
{
	// initialize Watch-dog timer
	WDTCR = 0x95;   	// set
	WDTC = 0x96;    	// WDT clear & run
}

void clock_init()
{
	// internal RC clock (20.000000MHz)
	OSCCR = 0x28;   	// Set Int. OSC
	SCCR = 0x00;    	// Use Int. OSC
}

void port_init()
{
	// initialize ports
	//   2 : P27      out DIR
	//   3 : EINT11   in  FG
	//   4 : PWM2o    out MOTO_PWM
	//   5 : AN6      in  BAT_DETECT
	//   7 : P17      out POWER_CTRL
	//   8 : OPout    out OPOUT
	//   9 : OP-      in  OP-
	//  10 : OP+      in  OP+
	//  11 : P13      out 
	//  12 : P12      out 
	//  13 : T1       in  DC_IN
	//  14 : P10      in  POWR_KEY
	//  16 : P21      out SEG5
	//  17 : P20      out SEG4
	//  18 : P05      out SEG3
	//  19 : P04      out SEG2
	//  20 : P03      out SEG1
	//  21 : PWM3BA   out LED_B
	//  22 : P01      out SEG0
	//  23 : PWM3AA   out LED_R
	//  24 : RxD      in  RXD
	//  25 : TxD      out TXD
	//  26 : P31      out COM1
	//  27 : P30      out COM0
	
	P0   = 0x3A;    	// port initial value
	P0IO = 0xFF;    	// direction
	P0PU = 0x00;    	// pullup
	P0OD = 0x3A;    	// open drain

	P1   = 0x0f;    	// port initial value
	P1IO = 0xCC;    	// direction
	P1PU = 0x02;    	// pullup
	P1OD = 0x0C;    	// open drain
	P1DB = 0x03;    	// debounce : P17~P10




	P2   = 0x03;    	// port initial value

	P2IO = 0xEB;    	// direction
	P2PU = 0x10;    	// pullup
	P2OD = 0x83;    	// open drain
	P23DB = 0x02;   	// bit7~6(debounce clock), bit2~0=P35,P24,23 debounce

	//P2IO = 0xEB;    	// direction
	//P2PU = 0x10;    	// pullup
	//P2OD = 0x8B;    	// open drain
	//P23DB = 0x02;   	// bit7~6(debounce clock), bit2~0=P35,P24,23 debounce


	P3   = 0x03;    	// port initial value
	P3IO = 0xF7;    	// direction
	P3PU = 0x00;    	// pullup
	P3OD = 0x00;    	// open drain


	// Set port functions
	P0FSR = 0x05;   	// P0 selection
	P1FSRH = 0x15;  	// P1 selection High
	P1FSRL = 0x00;  	// P1 selection Low
	P2FSRH = 0x00;  	// P2 selection High
	P2FSRL = 0x60;  	// P2 selection Low
	P3FSR = 0x0C;   	// P3 selection
	

}


