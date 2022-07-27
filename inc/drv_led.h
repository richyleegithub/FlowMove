#ifndef LED_DRV_H
#define LED_DRV_H
#include "types.h"

 #define LED_COM1			0
 #define LED_COM2			1

#define BIT(n)				(1<<n)				

//////////////////////////////////////////////////////
#define 	BAT_SEG_LESS_18V0	0X3E
#define		BAT_SEG_MORE_18V0	0X01
#define		BAT_SEG_MORE_19V0	0X03
#define		BAT_SEG_MORE_20V0	0X07
#define		BAT_SEG_MORE_21V0	0X0F
#define		BAT_SEG_MORE_22V0	0X1F
#define		BAT_SEG_MORE_24V5	0X3F

#define 	SEG0				P01
#define		SEG1				P30
#define		SEG2				P13
#define		SEG3				P12
#define		SEG4				P20
#define	    SEG5				P21
#define     SEG6				P03
#define 	SEG7				P31
#define 	SEG8				P04
#define 	SEG9				P05


enum{
		LED_MODE1=0X01,
		LED_MODE2=0X02,
		LED_MODE3=0X03,
		LED_MODE4=0X04,
		LED_BT	 =0X08
};


enum{
		LED_BAT_NONE,
		LED_BAT_LOW,
	
		LED_BAT_LEVEL18V0,
		LED_BAT_LEVEL19V0,
		LED_BAT_LEVEL20V0,
		LED_BAT_LEVEL21V0,
		LED_BAT_LEVEL22V0,
		LED_BAT_LEVEL24V5,
};





enum{
		LED_CURRENT1=0,
		LED_CURRENT2,
		LED_CURRENT3,
		LED_CURRENT4,
		LED_CURRENT5,
		LED_CURRENT_MAX,
};


enum{
	LED_CHARGE_OFF=0,
	LED_CHARGE_ON=1,

};

extern uint8_t 	 drv_led_tick;
////////////////////////////////////////////////////////////////
void drv_led_ini(void);
void drv_led(void);

void drv_led_set_mode_connect(void);
void drv_led_set_mode_light(uint8_t mode);
void drv_led_set_mode_light_off(void);

void drv_led_set_current_light(uint8_t current);
void drv_led_set_current_light_reset(void);

void drv_led_set_battery_light(uint8_t bat_level);
void drv_led_set_battery_charge_light(uint8_t bat_level,uint8_t fl);


void drv_led_set_battery_light_off(void);
void drv_led_set_bat_level(uint8_t level);
void drv_led_set_charge_onoff(uint8_t onoff);

void drv_led_set_test_mode_light(void);


#endif///
