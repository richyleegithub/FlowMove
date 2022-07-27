#ifndef DRV_BLE_H
#define DRV_BLE_H
#include "types.h"

#define  REQ_PARAM_MAX_LEN			3
#define  MAX_CALLREQ_QUEUE			4
#define  MAX_CALLBACK_QUEUE			4




  typedef struct _request_element
  {
	// uint8_t	   wCmd;
	// uint8_t	   wLen;
	 uint8_t	   wBuf[REQ_PARAM_MAX_LEN];
  } REQUEST_ELEMENT;
   
 typedef struct _request_queue
 {
	 uint8_t requestOut;
	 uint8_t requestIn;
	 uint8_t requestCnt;
	 REQUEST_ELEMENT requestQueue[MAX_CALLREQ_QUEUE];
 } REQUEST_QUEUE;
 

 typedef struct _callback_element
 {
	uint8_t	  wCmd;
 } CALLBACK_ELEMENT;
  
typedef struct _callback_queue
{
	uint8_t requestOut;
	uint8_t requestIn;
	uint8_t requestCnt;
	CALLBACK_ELEMENT requestQueue[MAX_CALLBACK_QUEUE];
} CALLBACK_QUEUE;



extern REQUEST_QUEUE 	xdata commandQueue;
extern CALLBACK_QUEUE	xdata callbackQueue;

//////////////////////////////////////////////////////////////////////////////
void 	InitialQueue(void);
BOOL  	SendQueueSysRuningParam(  uint8_t pressure, uint8_t temperature,   uint8_t battery  );
void  	PopCommandQueue(void);
BOOL  	SendQueueAppAck( uint8_t cmd);
void 	PopAckQueue(void);





#endif
