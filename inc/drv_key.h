#ifndef KEY_H
#define KEY_H


enum{
	KEY_SCAN_IDLE,
	KEY_SCAN_CHECK,
	KEY_SCAN_DEBOUCNE,
	KEY_SCAN_RELEASE,
};

enum{
	
		KEY_NONE_VALUE,
		KEY_PRESS_VALUE,
		KEY_LONG_VALUE,
		KEY_RELEASE_VALUE,
};

typedef struct{
	uint8_t sate;
	uint8_t debouce;
	uint8_t longKeyPress;
	uint16_t release_time;
}KEY_CTRL_t;

extern uint8_t drv_key_tick;

//////////////////////////////////////////////////////////

void drv_key(void);

#endif
