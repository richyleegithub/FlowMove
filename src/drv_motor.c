#include	"MC96FD316.h"


#include  	"types.h"
#include	"func_def.h"
#include 	"drv_motor.h"
#include 	"drv_adc.h"
#include 	"sys_message.h"
#include    "utiles.h"
#include  	"drv_led.h"








//////////////////////////////////////////////////////////////
#define		DRV_MOTOR_TICK	50

#define 	PLUSE_CHECK_TIME	(1000/DRV_MOTOR_TICK)
#define 	CURRENT_CHECK_TIME	(200/DRV_MOTOR_TICK)



#define     read_speed_count()	  (motor_FG_pluse)
#define 	rest_speed_count()   do{\
									IE &= ~0x80;\
									motor_FG_pluse=0;\
									IE |=  0x80;\
								  }while(0)


#define drv_set_speed_duty(speed) do{\
									T2BDRL = speed&0XFF;\
									T2BDRH = (speed>>8)&0xff;\
								  }while(0)
/**
X =(999*A)/3200
	
***/

#define		SPEED_1600		  (uint16_t)498
#define 	SPEED_2100		  (uint16_t)655
#define		SPEED_2600		  (uint16_t)811

#define     SPEED_1600_PLUSE	133
#define		SPEED_2100_PLUSE	175
#define 	SPEED_2600_PLUSE	217
								
MOTOR_CTRL_t  xdata motor_ctrl;

uint8_t	 	drv_motor_tick;

uint16_t	motor_FG_pluse;

#if 0
/*=========================================================
* FUNCTION NAME: drv_set_speed_duty
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_set_speed_duty(uint16_t speed)
{
		T2BDRL = speed&0XFF;
		T2BDRH = (speed>>8)&0xff;

}
#endif

/*=========================================================
* FUNCTION NAME: drv_motor
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
uint8_t runing_current_check(uint16_t current){
uint8_t currrent_level;
		if(current>=MOTOR_CURRENT_4A){
			currrent_level=4;
		}else if(current>=MOTOR_CURRENT_3A){
			currrent_level=3;
		}else if(current>=MOTOR_CURRENT_2A){
			currrent_level=2;
		}else if(current>=MOTOR_CURRENT_1A){
			currrent_level=1;
		}else{
			currrent_level=0;
		}
		return currrent_level;

}

/*=========================================================
* FUNCTION NAME: drv_motor
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

void Motor_Current_Check(void){
uint16_t current_current;
uint8_t level;
	current_current=drv_read_adc_currrent();
	level=runing_current_check(current_current);
	if(level!=motor_ctrl.pre_level){
		motor_ctrl.current_debouce++;
		if(motor_ctrl.current_debouce>=CURRENT_CHECK_TIME){
			motor_ctrl.current_debouce=0;
			
			if(level>=motor_ctrl.pre_level){
				if(motor_ctrl.pre_level<LED_CURRENT5){
					motor_ctrl.pre_level++;
				}
			}else{
				if(motor_ctrl.pre_level>0){
					motor_ctrl.pre_level--;
				}
			}
			put_message(MSG_CURRENT_CHANGE_IND, motor_ctrl.pre_level);
		}

	}
	else
	{
		motor_ctrl.current_debouce=0;
	}

}
/*=========================================================
* FUNCTION NAME: drv_motor
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_motor(void){
uint16_t current_speed;		

		if(drv_motor_tick<DRV_MOTOR_TICK) return;
		drv_motor_tick=0;
		
		switch(motor_ctrl.state){
			case MOTOR_UP_SPEED:
				motor_ctrl.speed_check_time++;
				if(motor_ctrl.speed_check_time>PLUSE_CHECK_TIME)
				{	motor_ctrl.speed_check_time=0;
					current_speed=read_speed_count();
					rest_speed_count();
					if(current_speed<2){
						motor_ctrl.knock_time++;
						if(motor_ctrl.knock_time>=3){
							motor_ctrl.knock_time=0;
#ifndef DEBUG_KNOCK							
							put_message(MSG_MOTOR_KNOCKED,0);
#endif
						}
					}
					else
					{
							motor_ctrl.knock_time=0;
					}
				}
		
				if(motor_ctrl.current_set_pwm<motor_ctrl.target_pwm){
					 motor_ctrl.current_set_pwm+=motor_ctrl.pwm_step;
				}else{
					motor_ctrl.target_pwm=SPEED_1600;
					motor_ctrl.current_set_pwm=SPEED_2600;
					motor_ctrl.state=MOTOR_DOWN_SPEED;
				}
				drv_set_speed_duty(motor_ctrl.current_set_pwm);
			break;
			case MOTOR_DOWN_SPEED:
				motor_ctrl.speed_check_time++;
				if(motor_ctrl.speed_check_time>PLUSE_CHECK_TIME)
				{	motor_ctrl.speed_check_time=0;
					current_speed=read_speed_count();
					rest_speed_count();
					if(current_speed<2){
						motor_ctrl.knock_time++;
						if(motor_ctrl.knock_time>=3){
							motor_ctrl.knock_time=0;
#ifndef DEBUG_KNOCK							
							put_message(MSG_MOTOR_KNOCKED,0);
#endif
						}
					}
					else
					{
							motor_ctrl.knock_time=0;
					}
				}

			
				if(motor_ctrl.current_set_pwm>motor_ctrl.target_pwm){
					 motor_ctrl.current_set_pwm-=motor_ctrl.pwm_step;
				}else{
					motor_ctrl.target_pwm=SPEED_2600;
					motor_ctrl.current_set_pwm=SPEED_1600;
					motor_ctrl.state=MOTOR_UP_SPEED;
				}
				drv_set_speed_duty(motor_ctrl.current_set_pwm);
			break;
			case MOTOR_RUNING:
				motor_ctrl.speed_check_time++;
				if(motor_ctrl.speed_check_time>PLUSE_CHECK_TIME){

					motor_ctrl.speed_check_time=0;
					current_speed=read_speed_count();
					rest_speed_count();
					
					if(current_speed<2){
						motor_ctrl.knock_time++;
						if(motor_ctrl.knock_time>=3){
							motor_ctrl.knock_time=0;
#ifndef DEBUG_KNOCK								
							put_message(MSG_MOTOR_KNOCKED,0);
#endif

						}
					}
					else
					{
							motor_ctrl.knock_time=0;
					}
					
					//if(current_speed<motor_ctrl.targe_speed_pluse){
					//	motor_ctrl.current_set_pwm+=2;
					//	drv_set_speed_duty(motor_ctrl.current_set_pwm);
					//}else{
					//	motor_ctrl.current_set_pwm-=2;
					//	drv_set_speed_duty(motor_ctrl.current_set_pwm);
					//}
				}
				break;
			default:
				break;
		}
		if(motor_ctrl.state!=MOTOR_SOP){
			// check motor current;
			 Motor_Current_Check();
		
		}
		
}

/*=========================================================
* FUNCTION NAME: drv_motor
* input :				 none
* Output:				 none
* description:    
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_motor_start(uint8_t mode_speed){
	
		switch(mode_speed){
		case MODE_SPEED_1600:	

			motor_ctrl.target_pwm=SPEED_1600;
			motor_ctrl.current_set_pwm=SPEED_1600;
			motor_ctrl.targe_speed_pluse=SPEED_1600_PLUSE;
			motor_ctrl.state=MOTOR_RUNING;
			break;
		case MODE_SPEED_2100:
			
			motor_ctrl.target_pwm=SPEED_2100;
			motor_ctrl.current_set_pwm=SPEED_2100;
			motor_ctrl.targe_speed_pluse=SPEED_2100_PLUSE;
			motor_ctrl.state=MOTOR_RUNING;
			break;
		case MODE_SPEED_2600:
			
			motor_ctrl.target_pwm=SPEED_2600;
			motor_ctrl.current_set_pwm=SPEED_2600;
			motor_ctrl.targe_speed_pluse=SPEED_2600_PLUSE;
			motor_ctrl.state=MOTOR_RUNING;
			break;
		case MODE_SPEED_FLOW:
			motor_ctrl.current_set_pwm=SPEED_1600;
			motor_ctrl.target_pwm=SPEED_2600;
			motor_ctrl.targe_speed_pluse=SPEED_2600_PLUSE;
			motor_ctrl.state=MOTOR_UP_SPEED;
			motor_ctrl.pwm_step=5;

			break;
		default:
			break;
		}
		drv_set_speed_duty(motor_ctrl.current_set_pwm);
		T2CRH |= 0x80;  	// enable counter
		rest_speed_count();
		motor_ctrl.speed_check_time=0;
		motor_ctrl.knock_time=0;

}

/*===========================================================
* FUNCTION NAME: drv_motor_stop
* input :				 none
* Output:				 none
* description:    
************************************************************
author					release							date
*************************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_motor_stop(void){
	
	  	drv_set_speed_duty(0);
		T2CRH &= ~0x80;  	// enable counter
		motor_ctrl.target_pwm=0;
		motor_ctrl.current_set_pwm=0;
		motor_ctrl.pre_level=0;
		motor_ctrl.targe_speed_pluse=0;
		motor_ctrl.state=MOTOR_SOP;
}

/*=========================================================
* FUNCTION NAME: drv_motor_stop
* input :				 none
* Output:				 none
* description:    
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_motor_change_mode_speed(uint8_t mode_speed){
	
		switch(mode_speed){
		case MODE_SPEED_1600:	
			drv_set_speed_duty(SPEED_1600);
			motor_ctrl.target_pwm=SPEED_1600;
			motor_ctrl.current_set_pwm=SPEED_1600;
			motor_ctrl.targe_speed_pluse=SPEED_1600_PLUSE;
			motor_ctrl.state=MOTOR_RUNING;
			break;
		case MODE_SPEED_2100:
			drv_set_speed_duty(SPEED_2100);
			motor_ctrl.target_pwm=SPEED_2100;
			motor_ctrl.current_set_pwm=SPEED_2100;
			motor_ctrl.targe_speed_pluse=SPEED_2100_PLUSE;
			motor_ctrl.state=MOTOR_RUNING;
			break;
		case MODE_SPEED_2600:
			drv_set_speed_duty(SPEED_2600);
			motor_ctrl.target_pwm=SPEED_2600;
			motor_ctrl.current_set_pwm=SPEED_2600;
			motor_ctrl.targe_speed_pluse=SPEED_2600_PLUSE;
			motor_ctrl.state=MOTOR_RUNING;
			break;
		case MODE_SPEED_FLOW:
			motor_ctrl.current_set_pwm=SPEED_2600;
			motor_ctrl.target_pwm=SPEED_1600;
			motor_ctrl.targe_speed_pluse=SPEED_1600_PLUSE;
			motor_ctrl.pwm_step=5;
			motor_ctrl.state=MOTOR_DOWN_SPEED;
			break;
		default:
			break;
		}
		rest_speed_count();
		motor_ctrl.speed_check_time=0;
}

/*=========================================================
* FUNCTION NAME: drv_motor_change_direct_speed
* input :				 none
* Output:				 none
* description:    
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_motor_change_direct_speed(uint16_t speed){

		if(speed>2650){
			speed=2650;
		}
		motor_ctrl.current_set_pwm=((speed*10)/32);
		motor_ctrl.target_pwm=motor_ctrl.current_set_pwm;
		drv_set_speed_duty(motor_ctrl.current_set_pwm);
		
}

/*=========================================================
* FUNCTION NAME: drv_motor_change_direct_speed
* input :				 none
* Output:				 none
* description:    
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_motor_change_flow_time(uint8_t time){
uint16_t  t2c;
		if(time>=1&&time<=5){
			t2c=(uint16_t)time*(1000/DRV_MOTOR_TICK);//(time*1000)/50;
			motor_ctrl.pwm_step=((SPEED_2600-SPEED_1600)/time);
		}
}


