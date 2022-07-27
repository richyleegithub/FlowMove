#include	"MC96FD316.h"
#include	"func_def.h"
#include 	"types.h"
#include   	"drv_led.h"
#include 	"drv_battery.h"

#define  	LED_TICK				  (11)


#define 	LED_MODE_REQ				0X01
#define 	LED_BATTERY_REQ				0X02

uint8_t 	 drv_led_tick;
uint8_t 	led_req;
uint8_t 	drv_led_state;
uint8_t      drv_led_bat_level;

uint8_t 	 led[2];
uint16_t 	idata breath_duty;
uint8_t		idata breath_dir;
//code uint8_t charge_light_on[8][6]={
//code uint8_t charge_light_on_none[6]=	{0x01,0x02,0x04,0x08,0x10,0x20};//none
										/*NONE,LOW, L1   L2,  L3,  L4,  L5   FULL*/
code uint8_t charge_light_on_low[8]=	{0X01, 0X01,0x01,0x02,0x04,0x08,0x10,0x20};//low
code uint8_t charge_light_on_l1[8]=		{0X01, 0X01,0x01,0x03,0x05,0x09,0x11,0x21};//18
code uint8_t charge_light_on_l2[8]=		{0X03, 0X03,0x03,0x03,0x07,0x0b,0x13,0x23};//19
code uint8_t charge_light_on_l3[8]=		{0X07, 0X07,0x07,0x07,0x07,0x0f,0x17,0x27};//20
code uint8_t charge_light_on_l4[8]=		{0X0F, 0X0F,0x0f,0x0f,0x0f,0x0f,0x1f,0x2f};//21
code uint8_t charge_light_on_l5[8]=		{0X1F, 0X1F,0x1f,0x1f,0x1f,0x1f,0x1f,0x3f};//22
//code uint8_t charge_light_on_l6[6]=		{0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};//level6
//};

/*=========================================================
* FUNCTION NAME: drv_led_set_mode_light
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_set_mode_connect(void){
	

		led[0]=0x0f;
		led_req|=LED_MODE_REQ;	
}


/*=========================================================
* FUNCTION NAME: drv_led_set_mode_light
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_set_mode_light(uint8_t mode){
	

		led[0]=(1<<mode);
		led_req|=LED_MODE_REQ;	
}
/*=========================================================
* FUNCTION NAME: drv_led_set_mode_light
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_set_test_mode_light(void){
	
		led[0]=0xff;
		led[1]=0xff;
		
		led_req|=LED_MODE_REQ;	
		led_req|=LED_BATTERY_REQ;	
}


/*=========================================================
* FUNCTION NAME: drv_led_set_mode_light_off
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_set_mode_light_off(void){

		led[0]=0;
		led_req|=LED_MODE_REQ;	

}
/*=========================================================
* FUNCTION NAME: drv_led_set_battery_light
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_set_battery_light(uint8_t bat_level){

		switch(bat_level){
		case BAT_LOW:
			led[1]=BAT_SEG_LESS_18V0;
		break;
		case BAT_LEVEL1:
			led[1]=BAT_SEG_MORE_18V0;
		break;
		case BAT_LEVEL2:
			led[1]=BAT_SEG_MORE_19V0;
		break;
		case	BAT_LEVEL3:
			led[1]=BAT_SEG_MORE_20V0;
		break;
		case BAT_LEVEL4:
			led[1]=BAT_SEG_MORE_21V0;
		break;
		case BAT_LEVEL5:
			led[1]=BAT_SEG_MORE_22V0;
		break;
		case BAT_FULL:
			led[1]=BAT_SEG_MORE_24V5;
			break;
		default:
			break;
	}
	led_req|=LED_BATTERY_REQ;	
}
/*=========================================================
* FUNCTION NAME: drv_led_set_battery_light
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_set_battery_charge_light(uint8_t bat_level,uint8_t fl){
		switch(bat_level){
		case BAT_NONE:
		case BAT_LOW:
			led[1]=charge_light_on_low[fl];
			break;
		case BAT_LEVEL1:
			led[1]=charge_light_on_l1[fl];
			break;
		case BAT_LEVEL2:
			led[1]=charge_light_on_l2[fl];
			break;
		case	BAT_LEVEL3:
			led[1]=charge_light_on_l3[fl];
			break;
		case BAT_LEVEL4:
			led[1]=charge_light_on_l4[fl];
			break;
		case BAT_LEVEL5:
			led[1]=charge_light_on_l5[fl];
			break;
		case BAT_FULL:
			led[1]=0X3F;
			break;
		default:
			break;
		}

		led_req|=LED_BATTERY_REQ;

}

/*=========================================================
* FUNCTION NAME: drv_led_set_battery_light_off
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

void drv_led_set_battery_light_off(void){

		led[1]=0;
		led_req|=LED_BATTERY_REQ;
}

/*=========================================================
* FUNCTION NAME: drv_set_led_r_duty
* input :				 duty
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							data
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_set_led_r_duty(uint16_t duty)
{
		T3ADRL = duty&0XFF;
		T3ADRH = (duty>>8)&0xff;

}
/*=========================================================
* FUNCTION NAME: drv_set_led_b_duty
* input :				 duty
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_set_led_b_duty(uint16_t duty)
{
		T3BDRL = duty&0XFF;
		T3BDRH = (duty>>8)&0xff;

}

/*=========================================================
* FUNCTION NAME: drv_led_set_current_light
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_set_current_light(uint8_t current){
		switch(current){
		case  LED_CURRENT1://<1A	
			drv_set_led_b_duty(4000);  //6000
			drv_set_led_r_duty(0);
			break;

		case  LED_CURRENT2://1a<x<3a
			drv_set_led_b_duty(16000);//
			drv_set_led_r_duty(0);
			break;
		case  LED_CURRENT4://3a<x<4a
		case  LED_CURRENT5://>4a
			drv_set_led_b_duty(0);
			drv_set_led_r_duty(19998);
			break;
		default:
		break;
		}
		drv_led_state=0;
}

/*=========================================================
* FUNCTION NAME: drv_set_led_b_duty
* input :				 duty
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_set_current_light_reset(void){
	breath_duty=200;
	breath_dir=1;
	drv_set_led_b_duty(0);
	drv_set_led_r_duty(0);

}
/*=========================================================
* FUNCTION NAME: drv_led_write_mode
* input :				 duty
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_write_mode(void){
	if(led[0]&BIT(0)){	   	SEG0=0;	}
	else{	   				SEG0=1; }
	if(led[0]&BIT(1)){	   	SEG1=0;	}
	else{	   				SEG1=1;	}
	if(led[0]&BIT(2)){	   	SEG2=0;	}
	else{	   				SEG2=1;	}
	if(led[0]&BIT(3)){	    SEG3=0;	}
	else{	   				SEG3=1;}



}
/*=========================================================
* FUNCTION NAME: drv_led_write_battery
* input :				 duty
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_write_battery(void){

		if(led[1]&BIT(0)){	   SEG5=0;}
		else{		   		   SEG5=1;}
		
		if(led[1]&BIT(1)){	   SEG4=0;}
		else{				   SEG4=1;}

		if(led[1]&BIT(2)){	   SEG9=0;}
		else{				   SEG9=1;}
		
		if(led[1]&BIT(3)){	   SEG8=0;}
		else{				   SEG8=1;}

		
		if(led[1]&BIT(4)){	   SEG7=0;}
		else{				   SEG7=1;}

		if(led[1]&BIT(5)){    SEG6=0;}
		else{				   SEG6=1;}

}
/*=========================================================
* FUNCTION NAME: drv_led
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void breath_duty_cal(void){
	if(breath_dir){
		breath_duty+=100;
		if(breath_duty>=20000){
			breath_dir=0;
		}
	}else{
		if(breath_duty>=300){
			breath_duty-=100;
		}else{
			breath_duty=200;
			breath_dir=1;
		}
	}
}
/*=========================================================
* FUNCTION NAME: drv_led
* input :				 none
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led(void){

	    if(drv_led_tick<LED_TICK) return;
		drv_led_tick=0;
		
		
		if(drv_led_state==LED_CHARGE_ON)
		{
			breath_duty_cal();
			if(drv_led_bat_level>=LED_BAT_LEVEL22V0){
				drv_set_led_r_duty(0);
				drv_set_led_b_duty(breath_duty);
			}else{
				drv_set_led_r_duty(breath_duty);
				drv_set_led_b_duty(0);
			}
		}

		
		if(led_req){
			if((led_req&LED_MODE_REQ)==LED_MODE_REQ){
				drv_led_write_mode();
			}
			if((led_req&LED_BATTERY_REQ)==LED_BATTERY_REQ){
				drv_led_write_battery();
			}
			led_req=0;
	  }

}



/*=========================================================
* FUNCTION NAME: drv_led_ini
* input :				 duty
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void drv_led_ini(void){
	drv_set_led_b_duty(0);
	drv_set_led_r_duty(0);
	
	led[0]=0;
	led[1]=0;
	drv_led_write_mode();
	drv_led_write_battery();
	drv_led_state=0;
	drv_led_tick=0;

}
/*=========================================================
* FUNCTION NAME: drv_led_set_bat_level
* input :				 duty
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

void drv_led_set_bat_level(uint8_t level){

	drv_led_bat_level=level;


}

/*=========================================================
* FUNCTION NAME: drv_led_set_bat_level
* input :				 duty
* Output:				 none
* description:   period drive the led io to scan led 
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

void drv_led_set_charge_onoff(uint8_t onoff){
	drv_led_state=onoff;


}


