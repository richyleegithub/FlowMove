
#ifndef SYS_MANAGE_H
#define SYS_MANAGE_H

enum{

		SYS_POWER_UP,
		SYS_STOP_STATE,
		SYS_RUNING_STATE,
		SYS_CHARGIN_STATE,
		SYS_CHARGE_FULL,
		SYS_POWER_NONE,
		SYS_POWER_KEY_REL_WAIT,
		SYS_ERROR_STATE,
		SYS_TEST_WAIT,
		SYS_TES_STATE,
};


void sys_process(void);
void sys_process_ini(void);
#endif //