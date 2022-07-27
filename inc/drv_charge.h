#ifndef DRV_CHARGE_H
#define DRV_CHARGE_H

enum{
	ST_CHARGE_IDLE,
	ST_DC_OUT_DEBOUNCE,
	ST_DC_OUT,
	ST_DC_IN_DEBOUNCE,
	ST_DC_IN,
};

typedef struct{
	uint8_t 	state;
	uint8_t 	charge;
	uint16_t 	debounce;
}CHARGE_CTRL_t;
//////////////////////////////////////////////////////////////////
extern uint8_t   drv_charge_tick;
/////////////////////////////////////////////////////////////////

BOOL ReadChargeState(void);
void  Drv_Charge(void);
void drv_charge_ini(void);

#endif//
