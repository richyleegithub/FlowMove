
#include "sys_message.h"

message_t  message[MESSAGE_MAX_NUM];
uint8_t		 fetch_msg;
uint8_t		 total_msg;
/*=========================================================
* FUNCTION NAME: ini_message
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void ini_message(void){
		fetch_msg=0;
		total_msg=0;
}
/*=========================================================
* FUNCTION NAME: put_message
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
void put_message(uint8_t msg,uint16_t info){
uint8_t sum;
			sum=(uint8_t)(fetch_msg+total_msg)%MESSAGE_MAX_NUM;
			message[sum].msg=msg;
			message[sum].info=info;
			total_msg++;
}

/*=========================================================
* FUNCTION NAME: get_message
* input :				 none
* Output:				 none
* description:   
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/
BOOL get_message(uint8_t *msg,uint16_t *info){
BOOL  ret=FALSE;	
			if(total_msg>0){
				
				*msg=message[fetch_msg].msg;
				*info=message[fetch_msg].info;
				 fetch_msg = (fetch_msg + 1) % MESSAGE_MAX_NUM;
				 total_msg--;
					ret=TRUE;
			}
			return ret;	
		
}

