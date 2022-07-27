#include		"compiler.h"

#if defined __ICC8051__		// IAR
#include		<intrinsics.h>
#define		NOP			__no_operation()
#else						// KEIL
#include		<intrins.h>
#define		NOP			_nop_()
#endif

//======================================================
//		device   : MC96FD316(32, 28, 24, 20 pin)
//		version  : Ver1.2
//		date     : 2015-10-01.Thu
//		compiler : Keil-C
//======================================================


#ifndef		_MC96FD316_
#define		_MC96FD316_

SFR(P0, 0x80);	// P0 Data Register
	SBIT(P00, 0x80, 0);
	SBIT(P01, 0x80, 1);
	SBIT(P02, 0x80, 2);
	SBIT(P03, 0x80, 3);
	SBIT(P04, 0x80, 4);
	SBIT(P05, 0x80, 5);
	SBIT(P06, 0x80, 6);
	SBIT(P07, 0x80, 7);

SFR(P0IO, 0xA1);	// P0 Direction Register
SFR(P0OD, 0x91);	// P0 Open-drain Selection Register
SFR(P0PU, 0xAC);	// P0 Pull-up Resistor Selection Register

SFR(P1, 0x88);	// P1 Data Register
	SBIT(P10, 0x88, 0);
	SBIT(P11, 0x88, 1);
	SBIT(P12, 0x88, 2);
	SBIT(P13, 0x88, 3);
	SBIT(P14, 0x88, 4);
	SBIT(P15, 0x88, 5);
	SBIT(P16, 0x88, 6);
	SBIT(P17, 0x88, 7);
	
SFR(P1IO, 0xB1);	// P1 Direction Register
SFR(P1OD, 0x92);	// P1 Open-drain Selection Register
SFR(P1PU, 0xAD);	// P1 Pull-up Resistor Selection Register
SFR(P1DB, 0x95);	// P1 Debounce Enable Register

SFR(P2, 0x90);	// P2 Data Register
	SBIT(P20, 0x90, 0);
	SBIT(P21, 0x90, 1);
	SBIT(P22, 0x90, 2);
	SBIT(P23, 0x90, 3);
	SBIT(P24, 0x90, 4);
	SBIT(P25, 0x90, 5);
	SBIT(P26, 0x90, 6);
	SBIT(P27, 0x90, 7);

SFR(P2IO, 0xB9);	// P2 Direction Register
SFR(P2OD, 0x93);	// P2 Open-drain Selection Register
SFR(P2PU, 0xAE);	// P2 Pull-up Resistor Selection Register
SFR(P23DB, 0x96);	// P2/P3 Debounce Enable Register

SFR(P3, 0x98);	// P3 Data Register
	SBIT(P30, 0x98, 0);
	SBIT(P31, 0x98, 1);
	SBIT(P32, 0x98, 2);
	SBIT(P33, 0x98, 3);
	SBIT(P34, 0x98, 4);
	SBIT(P35, 0x98, 5);
	SBIT(P36, 0x98, 6);
	SBIT(P37, 0x98, 7);

SFR(P3IO, 0xC1);	// P3 Direction Register
SFR(P3OD, 0x94);	// P3 Open-drain Selection Register
SFR(P3PU, 0xAF);	// P3 Pull-up Resistor Selection Register

SFR(P0FSR, 0xF1);	//P0 Function Selection Register
SFR(P1FSRL, 0xF2);	//P1 Function Selection Low Register
SFR(P1FSRH, 0xF3);	//P1 Function Selection High Register
SFR(P2FSRL, 0xF4);	//P2 Function Selection Low Register
SFR(P2FSRH, 0xF5);	//P2 Function Selection High Register
SFR(P3FSR, 0xF6);	//P3 Function Selection Register

// Interrupt
SFR(IE, 0xA8);	// Interrupt Enable Register 0
SFR(IE1, 0xA9);	// Interrupt Enable Register 1
SFR(IE2, 0xAA);	// Interrupt Enable Register 2
SFR(IE3, 0xAB);	// Interrupt Enable Register 3
SFR(IP, 0xB8);	// Interrupt Priority Register 0
SFR(IP1, 0xF8);	// Interrupt Priority Register 1
SFR(EIFLAG0, 0xC0);	// External Interrupt Flag 0 Register
SFR(EIFLAG1, 0xB0);	// External Interrupt Flag 1 Register
SFR(EIPOL0L, 0xA4);	// External Interrupt Polarity 0 Low Register
SFR(EIPOL0H, 0xA5);	// External Interrupt Polarity 0 High Register
SFR(EIPOL1, 0xA6);	// External Interrupt Polarity 1 Register

// System Oscillator, Reset
SFR(SCCR, 0x8A);	// System Clock Control Register
SFR(OSCCR, 0xC8);	// Oscillator Control Register

// Basic Interval TImer
SFR(BITCNT, 0x8C);	// Basic Interval Timer Counter Register
SFR(BITCR, 0x8B);	// Basic Interval Timer Control Register

//  Watch Dog Timer
SFRX(WDTC, 0x505E);	// Watch Dog Timer Clear Register
SFRX(WDTSR, 0x505F);	// Watch Dog Timer Status Register
SFR(WDTIDR, 0x8E);	// Watch Dog Timer Identification Register
SFR(WDTCR, 0x8D);	// Watch Dog Timer Control register

// TIMER 0
SFR(T0CR, 0x99);	// Timer 0 Control Register
SFR(T0CNT, 0x9A);	// Timer 0 Counter Register
SFR(T0DR, 0x9B);	// Timer 0 Data register

// TIMER 1
SFR(T1CRL, 0xB2);	// Timer 1 Control Low Register
SFR(T1CRH, 0xB3);	// Timer 1 Control High Register
SFR(T1ADRL, 0xB4);	// Timer 1 A Data Low Register
SFR(T1ADRH, 0xB5);	// Timer 1 A Data High Register
SFR(T1BDRL, 0xB6);	// Timer 1 B Data Low Register
SFR(T1BDRH, 0xB7);	// Timer 1 B Data High Register

// TIMER 2
SFR(T2CRL, 0xBA);	// Timer 2 Control Low Register
SFR(T2CRH, 0xBB);	// Timer 2 Control High Register
SFR(T2ADRL, 0xBC);	// Timer 2 A Data Low Register
SFR(T2ADRH, 0xBD);	// Timer 2 A Data High Register
SFR(T2BDRL, 0xBE);	// Timer 2 B Data Low Register
SFR(T2BDRH, 0xBF);	// Timer 2 B Data High Register

// TIMER 3
SFR(T3CRL, 0xC2);	// Timer 3 Control Low Register
SFR(T3CRH, 0xC3);	// Timer 3 Control High Register
SFR(T3OUTCRL, 0xC4);	// Timer 3 Output Control Low Register
SFR(T3OUTCRH, 0xC5);	// Timer 3 Output Control High Register
SFR(T3PDRL, 0xC6);	// Timer 3 Period Data Low Register
SFR(T3PDRH, 0xC7);	// Timer 3 Period Data High Register
SFR(T3ADRL, 0xCA);	// Timer 3 A Data Low Register
SFR(T3ADRH, 0xCB);	// Timer 3 A Data High Register
SFR(T3BDRL, 0xCC);	// Timer 3 B Data Low Register
SFR(T3BDRH, 0xCD);	// Timer 3 B Data High Register
SFR(T3CDRL, 0xCE);	// Timer 3 C Data Low Register
SFR(T3CDRH, 0xCF);	// Timer 3 C Data High Register
SFR(T3DLY, 0xC9);	// Timer 3 PWM Output Delay Register
SFR(T3INTCR, 0xD1);	// Timer 3 Interrupt Control Register
SFR(T3IFR, 0xD2);	// Timer 3 Interrupt Flag Register
SFR(T3ADTCR, 0xD3);	// Timer 3 A/DC Trigger Control Register
SFR(T3ADTDR, 0xD4);	// Timer 3 A/DC Trigger Generator Data Register
SFR(T3HIZCR, 0xD5);	// Timer 3 High-Impedance Control register

// BUZZER
SFR(BUZDR, 0x8F);	// BUZZER Data Register
SFR(BUZCR, 0x97);	// BUZZER Control Register

// ADC
SFR(ADCCRL, 0x9C);	// A/D Converter Control Low Register
SFR(ADCCRH, 0x9D);	// A/D Converter Control High Register
SFR(ADCDRL, 0x9E);	// A/D Converter Data Low Register
SFR(ADCDRH, 0x9F);	// A/D Converter Data High Register

// Comparator
SFR(CMPCR, 0xEA);	// Comparator Control Register
SFR(CMPPOL, 0xE9);	// Comparator Polarity Register
SFR(CMPFLAG, 0xA0);	// Comparator Interrupt Flag Register
SFR(CMPNFCR, 0xEC);	// Comparator Noise Filter Control Register

// Operational Amplifier
SFR(AMPCR, 0xEB);	// OP-AMP Control Register

// USI (UART + SPI + I2C)
SFR(USICR1, 0xD9);	// USI Control Register 1
SFR(USICR2, 0xDA);	// USI Control Register 2
SFR(USICR3, 0xDB);	// USI Control Register 3
SFR(USICR4, 0xDC);	// USI Control Register 4
SFR(USIST1, 0xE1);	// USI Status Register 1
SFR(USIST2, 0xE2);	// USI Status Register 2
SFR(USIBD, 0xE3);	// USI Baud Rate Generation Register
SFR(USIDR, 0xE5);	// USI Data Register
SFR(USISDHR, 0xE4);	// USI SDA Hold Time Register
SFR(USISCHR, 0xE7);	// USI SCL High Period Register
SFR(USISCLR, 0xE6);	// USI SCL Low Period Register
SFR(USISAR, 0xDD);	// USI Slave Address Register

// Calculator
SFR(CALCR, 0xED);	// Calculator Control Register
SFR(CALPTR, 0xEE);	// Calculator Address Pointer
SFR(CALDR, 0xEF);	// Calculator Data Register

// Power down operation
SFR(PCON, 0x87);	// Power Control Register
SFR(LVICR, 0x86);	// Low Voltage Indicator Control Register
SFR(RSTFR, 0xE8);	// Reset Flag Register
SFR(LVRCR, 0xD8);	// Low Voltage Reset Control Register

// Flash Memory Control
SFR(FSADRH, 0xFA);	// Flash Sector Address High Register
SFR(FSADRM, 0xFB);	// Flash Sector Address Middle Register
SFR(FSADRL, 0xFC);	// Flash Sector Address Low Register
SFR(FIDR, 0xFD);	// Flash Identification Register
SFR(FMCR, 0xFE);	// Flash Mode Control Register

// Flash CRC
SFRX(FCSARH, 0x5050);	// Flash CRC Start Address High Register
SFRX(FCEARH, 0x5051);	// Flash CRC End Address High Register
SFRX(FCSARM, 0x5052);	// Flash CRC Start Address Middle Register
SFRX(FCEARM, 0x5053);	// Flash CRC End Address Middle Register
SFRX(FCSARL, 0x5054);	// Flash CRC Start Address Low Register
SFRX(FCEARL, 0x5055);	// Flash CRC End Address Low Register
SFRX(FCCR, 0x5056);	// Flash CRC Control Register
SFRX(FCDRH, 0x5057);	// Flash CRC Data High Register
SFRX(FCDRL, 0x5058);	// Flash CRC Data Low Register
SFR(FCDIN, 0xDF);	// Flash CRC Data In Register

// Miscellany
SFR(ACC, 0xE0);
SFR(B, 0xF0);
SFR(PSW, 0xD0);
SFR(SP, 0x81);
SFR(DPL, 0x82);
SFR(DPH, 0x83);
SFR(DPL1, 0x84);
SFR(DPH1, 0x85);
SFR(EO, 0xA2);	// EXTENDED OPERATION REGISTER

#endif	//_MC96FD316_

#define		cli()		do{IE &= ~0x80;}while(0)
#define		sei()		do{IE |=  0x80;}while(0)
