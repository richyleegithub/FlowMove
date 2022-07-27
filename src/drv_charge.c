
#include	"MC96FD316.h"
#include 	"types.h"
#include 	"sys_message.h"
#include 	"drv_charge.h"

#define DRV_CHARGE_TICK			50
#define DRV_CHARGE_DEBOUCE		(1500/DRV_CHARGE_TICK)

#define DC_PORT					P1
#define DC_PIN					0X02

uint8_t   drv_charge_tick;

CHARGE_CTRL_t xdata chargeCtrl;
/******************************************************************************************
*  FUNCTION NAME: ReadChargeState
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

BOOL ReadChargeState(void){
	return (chargeCtrl.state==ST_DC_IN);
	
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

void  Drv_Charge(void){
	if(drv_charge_tick<DRV_CHARGE_TICK) return;
	drv_charge_tick++;
	switch(chargeCtrl.state){
	case 	ST_CHARGE_IDLE:
		chargeCtrl.debounce=0;	
		if((DC_PORT&DC_PIN)==0){
			
			chargeCtrl.state=ST_DC_IN_DEBOUNCE;
		}else{
			chargeCtrl.state=ST_DC_OUT_DEBOUNCE;
		}
		break;
	case 	ST_DC_OUT_DEBOUNCE:
		if((DC_PORT&DC_PIN)==0){
			chargeCtrl.debounce=0;
			chargeCtrl.state=ST_DC_IN_DEBOUNCE;
		}
		else
		{
			chargeCtrl.debounce++;
			if(chargeCtrl.debounce>=DRV_CHARGE_DEBOUCE){
				chargeCtrl.debounce=0;
				put_message(MSG_DISCHARGE_IND, 0);
				chargeCtrl.state=ST_DC_OUT;
			}
		}
		break;
	case ST_DC_OUT:
		if((DC_PORT&DC_PIN)==0){
			chargeCtrl.debounce=0;
			chargeCtrl.state=ST_DC_IN_DEBOUNCE;
		}
		break;
	case ST_DC_IN_DEBOUNCE:
		if((DC_PORT&DC_PIN)==0){
			chargeCtrl.debounce++;
			if(chargeCtrl.debounce>=DRV_CHARGE_DEBOUCE){
				chargeCtrl.debounce=0;
				put_message(MSG_CHARGE_IND, 1);
				chargeCtrl.state=ST_DC_IN;
			}
		}
		else
		{
			chargeCtrl.debounce=0;
			chargeCtrl.state=ST_DC_OUT_DEBOUNCE;
		}
		break;
	case ST_DC_IN:
		if((DC_PORT&DC_PIN)==DC_PIN){
			chargeCtrl.debounce=0;
			chargeCtrl.state=ST_DC_OUT_DEBOUNCE;
		}
		break;
	default:
		break;
	}

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
void drv_charge_ini(void){

		drv_charge_tick=0;
		chargeCtrl.debounce=0;	
		chargeCtrl.state=ST_CHARGE_IDLE;
}

