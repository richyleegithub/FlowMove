#include	"MC96FD316.h"
#include 	"types.h"
#include	"func_def.h"
#include 	"drv_adc.h"
#define 	SAMPLE_COUNT		16
#define		ADC_SAMPLE_TICK		1
uint16_t 	idata adc_sum;
uint16_t 	idata adc_battery;
uint16_t 	idata adc_motor;
uint8_t	  idata	adc_state;
uint8_t   idata 	adc_count;
uint8_t    	adc_timer_tick;

/******************************************************************************************
*  FUNCTION NAME: drv_adc_ini
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
void drv_adc_ini(void){

		adc_count=0;
		adc_sum=0;
		adc_state=ADC_START;
}
/******************************************************************************************
*  FUNCTION NAME: drv_read_adc_battery
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
uint16_t drv_read_adc_battery(void){

		return adc_battery;

}
/******************************************************************************************
*  FUNCTION NAME: drv_read_adc_current
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
uint16_t drv_read_adc_currrent(void){

		return adc_motor;

}


/******************************************************************************************
*  FUNCTION NAME: drv_adc
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
void drv_adc(void){
uint16_t adcVal;
	if(adc_timer_tick<ADC_SAMPLE_TICK) return;
	adc_timer_tick=0;


	switch(adc_state){
	case ADC_START:
		ADCCRL = (ADCCRL & 0xf0) | (BATTERY_CHANNEL & 0x0f);	
		ADCCRL |= 0x40; 	// start ADC
		adc_count=0;
		adc_sum=0;
		adc_state=ADC_SAMPLE_BATTERY;
		break;
		
	case ADC_SAMPLE_BATTERY:
		if((ADCCRL & 0x10)){					// wait ADC busy
			adcVal = ADCDRH << 8;				// read ADC high
			adcVal |= ADCDRL;					// read ADC low
			adc_sum+=adcVal;
			adc_count++;
			if(adc_count>=SAMPLE_COUNT){
				adc_battery=(adc_sum>>4);		// div 16
				adc_sum=0;
				adc_count=0;
				ADCCRL = (ADCCRL & 0xf0) | (CURRENT_CHANNEL & 0x0f);
				adc_state=ADC_SAMPLE_MOTOR;
			}
			ADCCRL |= 0x40; 	// start ADC 
		}
		
		break;

	case ADC_SAMPLE_MOTOR:
	
		if((ADCCRL & 0x10)){					// wait ADC busy
			adcVal = ADCDRH << 8;				// read ADC high
			adcVal |= ADCDRL;					// read ADC low
			adc_sum+=adcVal;
			adc_count++;
			if(adc_count>=SAMPLE_COUNT){
				adc_motor=(adc_sum>>4);		// div 16
				adc_sum=0;
				adc_count=0;
				ADCCRL = (ADCCRL & 0xf0) | (BATTERY_CHANNEL & 0x0f);
				adc_state=ADC_SAMPLE_BATTERY;
			}
			ADCCRL |= 0x40; 	// start ADC 
		}
		break;
	default:
	break;

	}
}




