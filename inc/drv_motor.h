#ifndef DRV_MOTOR_H
#define DRV_MOTOR_H
#ifdef LDO_5V
#define  MOTOR_CURRENT_1A			160  //116		//0.4v 0.375V
#define  MOTOR_CURRENT_2A			240 //232		//0.8V
#define  MOTOR_CURRENT_3A			360 //348		//1.2
#define  MOTOR_CURRENT_4A			480 //465		//1.6V
#else
#define  MOTOR_CURRENT_1A			102  	//0.4v *0.8
#define  MOTOR_CURRENT_2A			248 	//0.8V
#define  MOTOR_CURRENT_3A			372 	//1.2
#define  MOTOR_CURRENT_4A			496 	//1.6V

#endif


enum{
	MOTOR_SOP,
	MOTOR_UP_SPEED,
	MOTOR_DOWN_SPEED,
	MOTOR_RUNING,
};

enum{
	MODE_SPEED_1600=0,
	MODE_SPEED_2100	,		
	MODE_SPEED_2600	,
	MODE_SPEED_FLOW,
	MODE_MAX,
};

typedef struct{

uint8_t  	state;
uint16_t	current_set_pwm;
uint16_t  	target_pwm;
uint8_t	 	pwm_step;
uint8_t		pre_level;
uint8_t     current_debouce;
uint8_t		targe_speed_pluse;
uint8_t		knock_time;
uint8_t		speed_check_time;
}MOTOR_CTRL_t;


extern uint8_t drv_motor_tick;
extern uint16_t	motor_FG_pluse;
////////////////////////////////////////////////////////////////
void drv_motor(void);
void drv_motor_start(uint8_t mode_speed);
void drv_motor_stop(void);
void drv_motor_change_mode_speed(uint8_t mode_speed);
void drv_motor_change_direct_speed(uint16_t speed);
void drv_motor_change_flow_time(uint8_t time);
#endif