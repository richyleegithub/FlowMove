//======================================================
// Function and global variables definition
//======================================================

void port_init();             	// initialize ports
void clock_init();            	// initialize operation clock
void ADC_init();              	// initialize A/D convertor
void ADC_start(unsigned char ch);	// start A/D convertor
unsigned int ADC_read();      	// read A/D convertor
void BIT_init();              	// initialize Basic interval timer
void ExINT_init();            	// initialize external interrupt
void OPAMP_init();            	// initialize OPAMP
void Timer0_init();
void Timer1_init();           	// initialize Timer1
void Timer2_init();           	// initialize Timer2
void Timer3_init();           	// initialize Timer3
void UART_init();             	// initialize UART interface
//void UART_write(unsigned char dat);	// write UART
//unsigned char UART_read();    	// read UART
void WDT_init();              	// initialize Watch-dog timer

