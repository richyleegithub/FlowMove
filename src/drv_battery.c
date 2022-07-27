
#include	"MC96FD316.h"
#include 	"types.h"
#include 	"drv_charge.h"
#include  	"sys_message.h"
#include 	"drv_adc.h"
#include 	"drv_battery.h"


#define DRV_BATTERY_TICK				 	200	//ms
#define BATTERY_FIRST_DEBOUNCE_TIME			(1000/DRV_BATTERY_TICK)
#define BATTERY_CHK_DEBOUCNE_TIME			(20000/DRV_BATTERY_TICK)
#define BATTERY_CHK_DEBOUNCE_CHARGE_TIME	(1000/DRV_BATTERY_TICK)

uint8_t drv_battery_tick;
BATTERY_CTRL_t xdata BatteryCtrl;

/******************************************************************************************
*  FUNCTION NAME: drv_battery
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

uint8_t ReadBatteryLevel(BOOL charge){
	uint8_t battery_level;

	uint16_t battery_adc;	


	battery_adc=drv_read_adc_battery();

#if 1
	if(charge){
 		if(battery_adc>=CHARGE_VOLT_24V5){
			battery_level=BAT_FULL;
 		}else if(battery_adc>=CHARGE_VOLT_22V0){
			battery_level=BAT_LEVEL5;
			
		}else if(battery_adc>=CHARGE_VOLT_21V0){
			battery_level=BAT_LEVEL4;

		}else if(battery_adc>=CHARGE_VOLT_20V0){	
			battery_level=BAT_LEVEL3;

		}else if(battery_adc>=CHARGE_VOLT_19V0){	
			battery_level=BAT_LEVEL2; //

 		}else if(battery_adc>=CHARGE_VOLT_18V0){						
			battery_level=BAT_LEVEL1; //18V  LED LIGHT
 		}else if(battery_adc>=CHARGE_VOLT_17V0){
			battery_level=BAT_LOW; // FLASH ALL LED
		}else{
			battery_level=BAT_NONE;//POWER OFF,
		}
	}
	else
#endif

	{

		if(battery_adc>=VOLT_24V5){
			battery_level=BAT_FULL;
		}else if(battery_adc>=VOLT_22V0){
			battery_level=BAT_LEVEL5;
		}else if(battery_adc>=VOLT_21V0){
			battery_level=BAT_LEVEL4;
		}else if(battery_adc>=VOLT_20V0){ 
			battery_level=BAT_LEVEL3;
		}else if(battery_adc>=VOLT_19V0){ 
			battery_level=BAT_LEVEL2;
		}else if(battery_adc>=VOLT_18V0){ 	
			battery_level=BAT_LEVEL1;
		}else if(battery_adc>=VOLT_17V0){ 	
			battery_level=BAT_LOW; // FLASH ALL LED
		}else{
			battery_level=BAT_NONE;//POWER OFF,
		}
	}
	
	return battery_level;
}
/******************************************************************************************
*  FUNCTION NAME: drv_battery
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


void  drv_battery(void){
uint8_t level;
BOOL  chargeing;

		if(drv_battery_tick<DRV_BATTERY_TICK) return;
		drv_battery_tick=0;
		
		switch(BatteryCtrl.state){
		case ST_BATTERY_POWER_ON:
			BatteryCtrl.debouceTime=0;
			BatteryCtrl.state=ST_BATTERY_IDLE;
			break;
		case ST_BATTERY_IDLE:
			level=ReadBatteryLevel(0);
			if(BatteryCtrl.batery_level!=level){
				BatteryCtrl.batery_level=level;
				BatteryCtrl.debouceTime=0;
			}
			else
			{
				BatteryCtrl.debouceTime++;
				if(BatteryCtrl.debouceTime>=BATTERY_FIRST_DEBOUNCE_TIME){
					BatteryCtrl.batery_level=level;
					put_message(MSG_BATTERY_CHANGE_IND, BatteryCtrl.batery_level);
					BatteryCtrl.state=ST_BATTERY_CHK;
				}
			}
			break;
		case ST_BATTERY_CHK:
			chargeing=ReadChargeState();
			level=ReadBatteryLevel(chargeing);
			if(chargeing){
				if(level>BatteryCtrl.batery_level){
					BatteryCtrl.debouceTime++;
					if(BatteryCtrl.debouceTime>BATTERY_CHK_DEBOUNCE_CHARGE_TIME){
						BatteryCtrl.batery_level=level;
						put_message(MSG_BATTERY_CHANGE_IND, level);
					}
				}else{
					BatteryCtrl.debouceTime=0;
				}
			}
			else
			{
				if(level<BatteryCtrl.batery_level){
					BatteryCtrl.debouceTime++;
					if(BatteryCtrl.debouceTime>BATTERY_CHK_DEBOUCNE_TIME){
						BatteryCtrl.batery_level=level;
						put_message(MSG_BATTERY_CHANGE_IND, level);
					}
				}else{
					BatteryCtrl.debouceTime=0;
				}
			}
			break;
		default:
		break;
		}
}
/******************************************************************************************
*  FUNCTION NAME: drv_battery_ini
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


void  drv_battery_ini(void){
	BatteryCtrl.debouceTime=0;
	BatteryCtrl.state=ST_BATTERY_POWER_ON;

}

