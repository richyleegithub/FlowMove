#ifndef DRV_TIMER_H
#define DRV_TIMER_H
#include "types.h"

#define 	DRV_TIMER_TICK			10
#define		TIMEOUT_10MS(n)			(n*(10/DRV_TIMER_TICK))
#define 	TIMEOUT_100MS(n)		(n*(100/DRV_TIMER_TICK))
#define 	TIMEOUT_SECOND(n)		(n*(1000/DRV_TIMER_TICK))


typedef struct{
	uint8_t id;
	uint8_t ev;
	uint16_t delay;
}ev_timer_t;

extern uint8_t drv_timer_tick;
///////////////////////////////////////////////////

void drv_time(void);
void Init_evTimers(void);
void Start_evTimer(	uint16_t Time,	uint8_t ev	   );
void Stop_evTimer(uint8_t ev);

#endif


