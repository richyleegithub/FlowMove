#ifndef TASK_BATTERY_H
#define TASK_BATTERY_H

#include "types.h"

#define BATTERY_DISCHARGE			0
#define BATTERY_CHARGE			  1

///////////////////////////////////////////////////////////////
#ifdef LDO_5V
#define VOLT_24V5							540//24.5*36.47
#define VOLT_22V0							485//(22.0*36.47)
#define VOLT_21V0							463//(21.0*36.47)
#define VOLT_20V0							441//(20.9*36.47)
#define VOLT_19V0							419//(19.0*36.47)
#define VOLT_18V0							396//(18.0*36.47)3.724
#define VOLT_17V0							380//(17.0*36.47)
#else
#define VOLT_24V5							805//24.5*36.47
#define VOLT_22V0							723//(22.0*36.47)
#define VOLT_21V0							690//(21.0*36.47)
#define VOLT_20V0							658//(20.9*36.47)
#define VOLT_19V0							625//(19.0*36.47)
#define VOLT_18V0							592//(18.0*36.47)3.724
#define VOLT_17V0							559//(17.0*36.47)
#endif


/////////////////////////////////////////////////////////////////
#define CHARGE_VOLT_24V5					(VOLT_24V5)
#define CHARGE_VOLT_22V0					(VOLT_22V0)
#define CHARGE_VOLT_21V0					(VOLT_21V0)
#define CHARGE_VOLT_20V0					(VOLT_20V0)
#define CHARGE_VOLT_19V0					(VOLT_19V0)
#define CHARGE_VOLT_18V0					(VOLT_18V0)
#define CHARGE_VOLT_17V0					(VOLT_17V0)
///////////////////////////////////////////////////////////////////
enum{
	BAT_NONE,
	BAT_LOW,
	BAT_LEVEL1,
	BAT_LEVEL2,
	BAT_LEVEL3,
	BAT_LEVEL4,
	BAT_LEVEL5,
	BAT_FULL,
};


enum{
	ST_BATTERY_POWER_ON,
	ST_BATTERY_IDLE,
	ST_BATTERY_CHK,	



};

typedef struct{
	uint8_t state;
	uint8_t batery_level;
	uint8_t debouceTime;
}BATTERY_CTRL_t;
/////////////////////////////////////////////
extern uint8_t drv_battery_tick;


void  drv_battery(void);
void  drv_battery_ini(void);

#endif
