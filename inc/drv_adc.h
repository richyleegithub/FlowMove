
#ifndef DRV_ADC_H
#define DRV_ADC_H


#define BATTERY_CHANNEL			6
#define CURRENT_CHANNEL			2
enum{
 	ADC_START,
	ADC_SAMPLE_BATTERY,
	ADC_SAMPLE_MOTOR,
};
////////////////////////////////////////////////////
extern uint16_t idata	adc_battery;
extern uint16_t idata	adc_motor;

extern uint8_t data   	adc_timer_tick;

/////////////////////////////////////////////////////

void drv_adc_ini(void);
uint16_t drv_read_adc_battery(void);
uint16_t drv_read_adc_currrent(void);
void drv_adc(void);





#endif
