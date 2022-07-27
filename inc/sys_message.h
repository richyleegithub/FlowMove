/*-------------------------------------------------------------------------
    
-------------------------------------------------------------------------*/

#ifndef SYS_MESSAGE_H
#define SYS_MESSAGE_H
#include "types.h"


#define    MESSAGE_MAX_NUM				10

enum{
	MSG_KEY_PRESS,
	MSG_KEY_LONG_PRESS,
	MSG_KEY_TEST_PRESS,
	MSG_KEY_RELEASE,
	MSG_CURRENT_CHANGE_IND,
	MSG_CHARGE_IND,
	MSG_DISCHARGE_IND,
	MSG_MOTOR_KNOCKED,
	
	MSG_BATTERY_CHANGE_IND,
	MSG_BLE_CONNECT_IND,
	MSG_BLE_DISCONNECT_IND,
	MSG_BLE_CMD_START,
	MSG_BLE_CMD_STOP,
	MSG_BLE_SET_SPEED,
	MSG_BLE_SET_FLOWTIME,
	MSG_BLE_NOTICE_TIMEOUT,
	MSG_LOW_POWER_LED_FLASH_TIMEOUT,
	MSG_POWER_NONE_FLASH_TIMEOUT,
	MSG_POWER_NONE_TIMEOUT,
	MSG_CHARGE_FLASH_TIMEOUT,
	MSG_BATTERY_DISPLAY_TIMEOUT,	
	MSG_POWER_OFF_TIMEOUT,
	MSG_ERROR_TIMEOUT,
	MSG_ERROR_FLASH_TIMEOUT,
	MSG_TEST_STOP_TIMEOUT,
	MSG_RUNING_1MIN_TIMEOUT,
	MSG_DOUBLE_KEY_TIMEOUT,


};



typedef struct{
		uint8_t msg;
		uint16_t info;
} message_t;
//////////////////////////////////////////////////////////
BOOL 	get_message(uint8_t *msg,uint16_t *info);
void 		put_message(uint8_t msg,uint16_t info);
void 		ini_message(void);
#endif


