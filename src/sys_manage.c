#include "MC96FD316.h"
#include "types.h"
#include "sys_message.h"
#include "sys_manage.h"
#include "drv_led.h"
#include "drv_battery.h"
#include "drv_motor.h"
#include "drv_timer.h"
#include "utiles.h"
#include "drv_uart.h"

#define 	BLE_MODULE			1

#define 	POWER_CTRL_PIN 		P17

#define     TEST_RUNING_MIN		10


extern void drv_set_led_b_duty(uint16_t duty);
extern void drv_set_led_r_duty(uint16_t duty);


#define   	set_sys_state(m) do{sys_state=m;}while(0)
//#define 	power_off_req(m)	do{ power_off_req=m;} while(0)
#define 	disable_power()		do{POWER_CTRL_PIN=0;}while(0)
#define 	enable_power()		do{POWER_CTRL_PIN=1;}while(0)

uint8_t 	sys_state;
//uint8_t   	power_off_req;
uint8_t		runing_mode;
uint8_t		mmi_battery_level;
uint8_t		mmi_battery_charge_disposition;
uint8_t		knock_flash_count;
uint8_t 	runing_time_count;
uint8_t 	mmi_pressure;
uint8_t		key_count;
uint8_t		none_power_count;

BOOL     	low_power_led_flash;
BOOL		none_power_led_flash;
BOOL        knock_led_flash;
BOOL 		msg_processed;
BOOL		mmi_charge;
BOOL 		ischarging_full;
BOOL		ble_connect_flag;

uint8_t 	act_msg;
uint16_t 	act_info;



/*=========================================================
* FUNCTION NAME: common_ev_Process
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void common_ev_Process(void){

		switch(act_msg){


	 ///////////////////////////////////////////////////////////////		
 	   case MSG_BATTERY_CHANGE_IND:
 		  
 		   mmi_battery_level=act_info&0xff;
 		   drv_led_set_bat_level(mmi_battery_level);
 		   drv_led_set_battery_light(mmi_battery_level);
 		   if(mmi_battery_level==BAT_LOW){
 			   low_power_led_flash=TRUE;
 			   Start_evTimer(TIMEOUT_100MS(5), MSG_LOW_POWER_LED_FLASH_TIMEOUT);
 		   }
		   else if(mmi_battery_level==BAT_NONE){
 			   drv_motor_stop();
			   none_power_led_flash=TRUE;
		   	   none_power_count=0;
			   drv_led_set_battery_light(BAT_LEVEL1);
			   Stop_evTimer(MSG_LOW_POWER_LED_FLASH_TIMEOUT);
			   Start_evTimer(TIMEOUT_10MS(50),MSG_POWER_NONE_FLASH_TIMEOUT);
///			   Start_evTimer(TIMEOUT_SECOND(5),MSG_POWER_NONE_TIMEOUT);
			   set_sys_state(SYS_POWER_NONE);
 		   }
 		   break;


	   case MSG_LOW_POWER_LED_FLASH_TIMEOUT:
		   if(low_power_led_flash==TRUE){
			   low_power_led_flash=FALSE;
		   		drv_led_set_battery_light_off();
		   }else{
			   low_power_led_flash=TRUE;
			   drv_led_set_battery_light(BAT_FULL);
		   }
		   Start_evTimer(TIMEOUT_100MS(5), MSG_LOW_POWER_LED_FLASH_TIMEOUT);
		   break;
		/////////////////////////////////////////////////////////////
			
		case MSG_BLE_SET_FLOWTIME:
			if(ble_connect_flag==TRUE){
				drv_motor_change_flow_time(act_info&0x0f);
			}
			break;
		case MSG_BLE_SET_SPEED:
			if(ble_connect_flag==TRUE){
				drv_motor_change_direct_speed(act_info);
			}
			break;
#if BLE_MODULE			
		case   MSG_BLE_NOTICE_TIMEOUT:
			SendQueueSysRuningParam(runing_mode,mmi_pressure,mmi_battery_level);	
			Start_evTimer(TIMEOUT_SECOND(1), MSG_BLE_NOTICE_TIMEOUT);
			break;
#endif
		case	MSG_KEY_LONG_PRESS:
			disable_power();
			drv_motor_stop();
			drv_led_set_mode_light_off();
			drv_led_set_current_light_reset();
			drv_led_set_battery_light_off();
			set_sys_state(SYS_POWER_KEY_REL_WAIT);
			break;
		case MSG_BLE_CONNECT_IND:
			ble_connect_flag=TRUE;
			drv_led_set_mode_connect();
			break;
		case MSG_BLE_DISCONNECT_IND:
			ble_connect_flag=FALSE;
			drv_led_set_mode_light(runing_mode);
			break;
		default:
			break;
		}

}
/*=========================================================
* FUNCTION NAME: sys_power_up
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void  sys_power_up(void){
		switch(act_msg){

		//case	MSG_KEY_LONG_PRESS:
		case  MSG_KEY_PRESS:	
			ble_connect_flag=FALSE;
			msg_processed=TRUE; // msg be processed
			runing_time_count=0;
			mmi_pressure=0;
			mmi_battery_level=BAT_FULL;
			enable_power();
			drv_at_mode();
			runing_mode=MODE_SPEED_1600;
			drv_motor_start(runing_mode);
			drv_led_set_mode_light(runing_mode);
			drv_led_set_current_light(LED_CURRENT1);
			drv_led_set_battery_light(mmi_battery_level);
			if(mmi_battery_level==BAT_LOW){
				low_power_led_flash=TRUE;
				Start_evTimer(TIMEOUT_100MS(5), MSG_LOW_POWER_LED_FLASH_TIMEOUT);
			}
#if BLE_MODULE				
			Start_evTimer(TIMEOUT_SECOND(10), MSG_BLE_NOTICE_TIMEOUT);
#endif
			Start_evTimer(TIMEOUT_SECOND(60),MSG_RUNING_1MIN_TIMEOUT);
			set_sys_state(SYS_RUNING_STATE);
			break;

		case MSG_BATTERY_CHANGE_IND:
			 msg_processed=TRUE;
			 mmi_battery_level=act_info&0xff;
			 //drv_led_set_bat_level(mmi_battery_level);
			 break;  
		case MSG_CHARGE_IND:
			 mmi_charge=TRUE;
			 mmi_battery_charge_disposition=mmi_battery_level;
			 drv_led_set_battery_charge_light(mmi_battery_level,mmi_battery_charge_disposition);
			 drv_led_set_charge_onoff(LED_CHARGE_ON);
			 drv_led_set_current_light_reset();
			 Start_evTimer(TIMEOUT_100MS(10),MSG_CHARGE_FLASH_TIMEOUT);
			 set_sys_state(SYS_CHARGIN_STATE);
			break;
			 
		default:
			break;
		}
}

/*=========================================================
* FUNCTION NAME: sys_stop
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

void sys_stop(void){


		switch(act_msg){
		case	MSG_KEY_PRESS:
			msg_processed=TRUE;
			if(ble_connect_flag==FALSE){
				
				runing_mode=MODE_SPEED_1600;
				drv_motor_start(runing_mode);
				drv_led_set_mode_light(runing_mode);
				drv_led_set_current_light(LED_CURRENT1);
#if BLE_MODULE				
				Start_evTimer(TIMEOUT_SECOND(1), MSG_BLE_NOTICE_TIMEOUT);
#endif
				set_sys_state(SYS_RUNING_STATE);
			}
			break;	

		
		case	MSG_BLE_CMD_START:
			msg_processed=TRUE;
			if(ble_connect_flag){
					
				
				if(act_info==1){
					runing_mode=MODE_SPEED_FLOW;
				}else{
					runing_mode=MODE_SPEED_1600;
				}
				drv_motor_start(runing_mode);
				drv_led_set_mode_light(runing_mode);
				drv_led_set_current_light(LED_CURRENT1);
#if BLE_MODULE				
				Start_evTimer(TIMEOUT_SECOND(1), MSG_BLE_NOTICE_TIMEOUT);
#endif
				set_sys_state(SYS_RUNING_STATE);
				}
			break;

		case MSG_POWER_OFF_TIMEOUT: 
			msg_processed=TRUE;
			drv_led_set_mode_light_off();
			drv_led_set_current_light_reset();
			drv_led_set_battery_light_off();
			disable_power();
			set_sys_state(SYS_POWER_UP);
			break;
		case MSG_CHARGE_IND:
			mmi_charge=TRUE;
			Stop_evTimer(MSG_LOW_POWER_LED_FLASH_TIMEOUT);
			mmi_battery_charge_disposition=mmi_battery_level;
			drv_led_set_battery_charge_light(mmi_battery_level,mmi_battery_charge_disposition);
			drv_led_set_charge_onoff(LED_CHARGE_ON);
			drv_led_set_current_light_reset();
			Start_evTimer(TIMEOUT_100MS(10),MSG_CHARGE_FLASH_TIMEOUT);
			set_sys_state(SYS_CHARGIN_STATE);
			break;
		default:
			break;
		}
}
/*=========================================================
* FUNCTION NAME: sys_runing
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void sys_runing(void){

		switch(act_msg){
		case	MSG_KEY_PRESS:
			msg_processed=TRUE;
			if(ble_connect_flag==FALSE){
				runing_mode++;
				if(runing_mode>=MODE_MAX) {
					drv_motor_stop();
					drv_led_set_mode_light_off();
					drv_led_set_battery_light_off();
					drv_led_set_current_light_reset();
					disable_power();
					set_sys_state(SYS_POWER_UP);
				}
				else
				{
					drv_led_set_mode_light(runing_mode);
					drv_motor_change_mode_speed(runing_mode);
				}
			}
			break;
		case MSG_CHARGE_IND:
			mmi_charge=TRUE;
			drv_motor_stop();
			drv_led_set_mode_light_off();
			Stop_evTimer(MSG_LOW_POWER_LED_FLASH_TIMEOUT);
			mmi_battery_charge_disposition=mmi_battery_level;
			drv_led_set_battery_charge_light(mmi_battery_level,mmi_battery_charge_disposition);
			drv_led_set_charge_onoff(LED_CHARGE_ON);
			drv_led_set_current_light_reset();
			Start_evTimer(TIMEOUT_100MS(10),MSG_CHARGE_FLASH_TIMEOUT);
			set_sys_state(SYS_CHARGIN_STATE);
			break;
			

		case MSG_CURRENT_CHANGE_IND:
			msg_processed=TRUE;
			mmi_pressure=act_info;
			drv_led_set_current_light(mmi_pressure);
			break;

		case   MSG_MOTOR_KNOCKED:	
			msg_processed=TRUE;
			drv_motor_stop();
			knock_flash_count=0;
			knock_led_flash=TRUE;
			drv_led_set_mode_light_off();
			drv_led_set_battery_light_off();
			drv_led_set_current_light(LED_CURRENT5);
			Start_evTimer(TIMEOUT_100MS(5),MSG_ERROR_FLASH_TIMEOUT);
			set_sys_state(SYS_ERROR_STATE);
			
			break;
		case MSG_BLE_CMD_STOP:
			msg_processed=TRUE;
			if(ble_connect_flag==TRUE){
				drv_motor_stop();
				Start_evTimer(TIMEOUT_SECOND(10),MSG_POWER_OFF_TIMEOUT);
				set_sys_state(SYS_STOP_STATE);
			}
			break;
		case	MSG_RUNING_1MIN_TIMEOUT:
			msg_processed=TRUE;
			runing_time_count++;
			if(runing_time_count>=10){
				drv_motor_stop();
				drv_led_set_mode_light_off();
				drv_led_set_battery_light_off();
				drv_led_set_current_light_reset();
				disable_power();
				set_sys_state(SYS_POWER_UP);
			}else{		
				Start_evTimer(TIMEOUT_SECOND(60), MSG_RUNING_1MIN_TIMEOUT);
			}
			break;
		default:
			break;
		}

}

/*=========================================================
* FUNCTION NAME: sys_charge
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void sys_charge(void){
		switch(act_msg){
		case MSG_DISCHARGE_IND:
			mmi_charge=TRUE;
	
			drv_led_set_mode_light_off();
			drv_led_set_charge_onoff(LED_CHARGE_OFF);
			drv_led_set_battery_light_off();
			drv_led_set_current_light_reset();
			Stop_evTimer(MSG_CHARGE_FLASH_TIMEOUT);
			disable_power();
			set_sys_state(SYS_POWER_UP);

			break;
		case	MSG_CHARGE_FLASH_TIMEOUT:
			msg_processed=TRUE;
			if(mmi_battery_level<BAT_FULL){
				mmi_battery_charge_disposition++;
				if(mmi_battery_charge_disposition>BAT_FULL){
					if(mmi_battery_level<=BAT_LEVEL1){
						mmi_battery_charge_disposition=BAT_LEVEL1;
					}else{
						mmi_battery_charge_disposition=mmi_battery_level;
					}
				}
				drv_led_set_battery_charge_light(mmi_battery_level,mmi_battery_charge_disposition);
				Start_evTimer(TIMEOUT_100MS(10),MSG_CHARGE_FLASH_TIMEOUT);
			}
			else
			{
				Stop_evTimer(MSG_CHARGE_FLASH_TIMEOUT);
				drv_led_set_charge_onoff(LED_CHARGE_OFF);
				drv_led_set_current_light(LED_CURRENT1);
				Start_evTimer(TIMEOUT_SECOND(5),MSG_BATTERY_DISPLAY_TIMEOUT);
				set_sys_state(SYS_CHARGE_FULL);
			}
			break;

		


		case MSG_BATTERY_CHANGE_IND:
			msg_processed=TRUE;
			mmi_battery_level=act_info&0xff;
			drv_led_set_bat_level(mmi_battery_level);
			if(mmi_battery_level==BAT_FULL){
				
				Stop_evTimer(MSG_CHARGE_FLASH_TIMEOUT);
				drv_led_set_charge_onoff(LED_CHARGE_OFF);
				drv_led_set_current_light(LED_CURRENT1);
				drv_led_set_battery_light(BAT_FULL);
				Start_evTimer(TIMEOUT_SECOND(5),MSG_BATTERY_DISPLAY_TIMEOUT);
				set_sys_state(SYS_CHARGE_FULL);
			}
			break;
		case	MSG_KEY_TEST_PRESS:
				msg_processed=TRUE;
				key_count=0;
				ischarging_full=FALSE;
				drv_led_set_test_mode_light();
				Start_evTimer(TIMEOUT_SECOND(3), MSG_DOUBLE_KEY_TIMEOUT);
				set_sys_state(SYS_TEST_WAIT);
			break;
		case	MSG_KEY_LONG_PRESS: 
				msg_processed=TRUE;

			break;
		default:
			break;
		}
}

/*=========================================================
* FUNCTION NAME: sys_charge_full
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

void sys_charge_full(void){

	switch(act_msg){
	case	MSG_BATTERY_DISPLAY_TIMEOUT:
		msg_processed=TRUE;
		drv_led_set_battery_light_off();
		drv_led_set_current_light_reset();
		break;

	case MSG_KEY_PRESS:
		msg_processed=TRUE;
		drv_led_set_current_light(LED_CURRENT1);
		drv_led_set_battery_light(mmi_battery_level);
		Start_evTimer(TIMEOUT_SECOND(5),MSG_BATTERY_DISPLAY_TIMEOUT);
		break;
	case	MSG_KEY_TEST_PRESS:
		msg_processed=TRUE;
		key_count=0;
		ischarging_full=TRUE;
		//drv_led_set_battery_light(BAT_FULL);
		drv_led_set_test_mode_light();
		Start_evTimer(TIMEOUT_SECOND(3), MSG_DOUBLE_KEY_TIMEOUT);
		set_sys_state(SYS_TEST_WAIT);
		
		break;
	case MSG_BATTERY_CHANGE_IND:
		msg_processed=TRUE;
		mmi_battery_level=act_info&0xff;
		drv_led_set_bat_level(mmi_battery_level);	
		break;
	case MSG_DISCHARGE_IND:
		mmi_charge=TRUE;
		drv_led_set_mode_light_off();
		drv_led_set_charge_onoff(LED_CHARGE_OFF);
		drv_led_set_battery_light_off();
		drv_led_set_current_light_reset();
		Stop_evTimer(MSG_CHARGE_FLASH_TIMEOUT);
		disable_power();
		set_sys_state(SYS_POWER_UP);
		break;
	case	MSG_KEY_LONG_PRESS:	
		msg_processed=TRUE;
		break;

	default:
		break;
	}


}
/*=========================================================
* FUNCTION NAME: sys_power_none
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

void sys_power_none(void){

		switch(act_msg){
		case	MSG_POWER_NONE_FLASH_TIMEOUT:
			msg_processed=TRUE;
			if(none_power_led_flash==TRUE){
				none_power_led_flash=FALSE;
				drv_led_set_battery_light_off();
				none_power_count++;
				if(none_power_count>=5){
					drv_led_set_charge_onoff(LED_CHARGE_OFF);
					drv_led_set_battery_light_off();
					drv_led_set_current_light_reset();
					disable_power();
					set_sys_state(SYS_POWER_UP);
				}else{
					Start_evTimer(TIMEOUT_10MS(25),MSG_POWER_NONE_FLASH_TIMEOUT);
				}
			}else{
				none_power_led_flash=TRUE;
				drv_led_set_battery_light(BAT_LEVEL1);
				Start_evTimer(TIMEOUT_10MS(25),MSG_POWER_NONE_FLASH_TIMEOUT);
			}
			break;
#if 0		
		case MSG_POWER_NONE_TIMEOUT:
			msg_processed=TRUE;
			drv_led_set_mode_light_off();
			drv_led_set_charge_onoff(LED_CHARGE_OFF);
			drv_led_set_battery_light_off();
			drv_led_set_current_light_reset();
			disable_power();
			set_sys_state(SYS_POWER_UP);
			break;
#endif

		case MSG_CHARGE_IND:
			mmi_charge=TRUE;
			Stop_evTimer(MSG_POWER_NONE_TIMEOUT);
			Stop_evTimer(MSG_POWER_NONE_FLASH_TIMEOUT);
			mmi_battery_charge_disposition=mmi_battery_level;
			drv_led_set_battery_charge_light(mmi_battery_level,mmi_battery_charge_disposition);
			drv_led_set_charge_onoff(LED_CHARGE_ON);
			drv_led_set_current_light_reset();
			Start_evTimer(TIMEOUT_100MS(10),MSG_CHARGE_FLASH_TIMEOUT);
			set_sys_state(SYS_CHARGIN_STATE);

		
			break;
		case MSG_DISCHARGE_IND:
			mmi_charge=FALSE;
			break;
		default:
			break;
			
		}


}
/*=========================================================
* FUNCTION NAME: sys_error
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void sys_error(void){
		switch(act_msg){
			case MSG_ERROR_FLASH_TIMEOUT:
			if(knock_led_flash==TRUE){
				knock_led_flash=FALSE;
				drv_set_led_r_duty(0);
				knock_flash_count++;

			}else{
				knock_led_flash=TRUE;
				drv_set_led_r_duty(19998);
			}
			if(knock_flash_count>=5){

				
				if(mmi_charge){
					mmi_battery_charge_disposition=mmi_battery_level;
					drv_led_set_battery_charge_light(mmi_battery_level,mmi_battery_charge_disposition);
					drv_led_set_charge_onoff(LED_CHARGE_ON);
					drv_led_set_current_light_reset();
					Start_evTimer(TIMEOUT_100MS(5),MSG_CHARGE_FLASH_TIMEOUT);
					set_sys_state(SYS_CHARGIN_STATE);
				}else{
					drv_led_set_current_light_reset();
					drv_led_set_battery_light_off();
					disable_power();
					set_sys_state(SYS_POWER_UP);
				}

			}else{
				Start_evTimer(TIMEOUT_100MS(5),MSG_ERROR_FLASH_TIMEOUT);
			}
			break;
		case MSG_CHARGE_IND:
			mmi_charge=TRUE;
			break;
		case MSG_DISCHARGE_IND:
			mmi_charge=FALSE;
			break;
		default:
		break;
		}
}



/*=========================================================
* FUNCTION NAME: sys_test_wait
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void sys_test_wait(void){
		switch(act_msg){
		case MSG_KEY_PRESS:
			msg_processed=TRUE;
			key_count++;
			if(key_count>=2){
				Stop_evTimer(MSG_DOUBLE_KEY_TIMEOUT);
				runing_mode=MODE_SPEED_2600;
				drv_motor_start(runing_mode);
				
				drv_led_set_test_mode_light();
				drv_set_led_r_duty(19998);
				drv_set_led_b_duty(19998);
				Start_evTimer(TIMEOUT_SECOND(60), MSG_RUNING_1MIN_TIMEOUT);
				set_sys_state(SYS_TES_STATE);
			}
			break;
		case MSG_DOUBLE_KEY_TIMEOUT:
			msg_processed=TRUE;
			if(ischarging_full==TRUE){
				drv_led_set_battery_light_off();
				set_sys_state(SYS_CHARGE_FULL);
			}
			else
			{
				mmi_battery_charge_disposition=mmi_battery_level;
				drv_led_set_battery_charge_light(mmi_battery_level,mmi_battery_charge_disposition);
				drv_led_set_charge_onoff(LED_CHARGE_ON);
				drv_led_set_current_light_reset();
				Start_evTimer(TIMEOUT_100MS(10),MSG_CHARGE_FLASH_TIMEOUT);
				set_sys_state(SYS_CHARGIN_STATE);
			}
			break;
		default:
		break;
		}


}

/*=========================================================
* FUNCTION NAME: sys_error
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void sys_test(void){
		switch(act_msg){
		case MSG_RUNING_1MIN_TIMEOUT:
			msg_processed=TRUE;
			runing_time_count++;
			if(runing_time_count>=TEST_RUNING_MIN){
				runing_time_count=0;
				drv_motor_stop();
				drv_led_set_current_light_reset();
				drv_led_set_mode_light_off();
				Start_evTimer(TIMEOUT_SECOND(60), MSG_TEST_STOP_TIMEOUT);
			}else{
				Start_evTimer(TIMEOUT_SECOND(60), MSG_RUNING_1MIN_TIMEOUT);
			}
			break;
		case MSG_TEST_STOP_TIMEOUT:
			msg_processed=TRUE;
			runing_mode=MODE_SPEED_2600;
			drv_motor_start(runing_mode);
			drv_led_set_test_mode_light();
			drv_set_led_r_duty(19998);
			drv_set_led_b_duty(19998);

			Start_evTimer(TIMEOUT_SECOND(60), MSG_RUNING_1MIN_TIMEOUT);
			break;
		case MSG_BATTERY_CHANGE_IND:
			msg_processed=TRUE;
			break;

		
		case MSG_CURRENT_CHANGE_IND:
			msg_processed=TRUE;
			break;
		case MSG_KEY_LONG_PRESS:
			disable_power();
			drv_motor_stop();
			drv_led_set_mode_light_off();
			drv_led_set_current_light_reset();
			drv_led_set_battery_light_off();
			set_sys_state(SYS_POWER_KEY_REL_WAIT);
			break;
		default:
		break;
		}


}
/*=========================================================
* FUNCTION NAME: sys_powerkey_release_wait
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

void sys_powerkey_release_wait(void){

	switch(act_msg){
	case	MSG_KEY_RELEASE:
		msg_processed=TRUE;
		if(mmi_charge){
			mmi_battery_charge_disposition=mmi_battery_level;
			drv_led_set_battery_charge_light(mmi_battery_level,mmi_battery_charge_disposition);
			drv_led_set_charge_onoff(LED_CHARGE_ON);
			drv_led_set_current_light_reset();
			Start_evTimer(TIMEOUT_100MS(10),MSG_CHARGE_FLASH_TIMEOUT);
			set_sys_state(SYS_CHARGIN_STATE);
		}else{
			set_sys_state(SYS_POWER_UP);
		}
		break;
	case MSG_CHARGE_IND:
		mmi_charge=TRUE;
		break;
	case MSG_DISCHARGE_IND:
		mmi_charge=FALSE;
		break;
	default:
		break;
	}
}

/*=========================================================
* FUNCTION NAME: sys_process
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void sys_process(void){



		if(get_message(&act_msg,&act_info)==0){
			return;
		}
		msg_processed=0;
		switch(sys_state){
		case SYS_POWER_UP:
			sys_power_up();
			break;
		case SYS_STOP_STATE:
			sys_stop();
			break;
		case SYS_RUNING_STATE:
			sys_runing();
			break;
		case SYS_CHARGIN_STATE:
			sys_charge();
			break;

		case SYS_CHARGE_FULL:
			sys_charge_full();
			break;
		case SYS_POWER_NONE:
			sys_power_none();
			break;
		case SYS_POWER_KEY_REL_WAIT:
			sys_powerkey_release_wait();
			break;
		case SYS_ERROR_STATE:
			sys_error();
			break;
		case SYS_TEST_WAIT:
			sys_test_wait();
			break;
		
		case SYS_TES_STATE:
			sys_test();
			break;
		default:
		break;
		}

		if(msg_processed==0){
			common_ev_Process();
		}
		
}

void sys_process_ini(void){

		sys_state=SYS_POWER_UP;

}
