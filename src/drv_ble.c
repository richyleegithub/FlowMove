
#include "utiles.h"
REQUEST_QUEUE 	commandQueue;
CALLBACK_QUEUE	callbackQueue;


/******************************************************************************************
*  FUNCTION NAME: LockStateInd
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
void InitialQueue(void){
	commandQueue.requestCnt=0;
	commandQueue.requestIn=0;
	commandQueue.requestOut=0;
	callbackQueue.requestCnt=0;
	callbackQueue.requestIn=0;
	callbackQueue.requestOut=0;
}
/******************************************************************************************
*  FUNCTION NAME: SendSysRuningParam
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/

BOOL  SendSysRuningParam(  uint8_t pressure, uint8_t temperature,   uint8_t battery  )
{
  
  if (commandQueue.requestCnt < MAX_CALLREQ_QUEUE)
  {
    commandQueue.requestCnt++;

	commandQueue.requestQueue[commandQueue.requestIn].wBuf[0]=pressure;
	commandQueue.requestQueue[commandQueue.requestIn].wBuf[0]=temperature;
	commandQueue.requestQueue[commandQueue.requestIn].wBuf[0]=battery;
    if (++commandQueue.requestIn >= MAX_CALLREQ_QUEUE)
    {
      commandQueue.requestIn = 0;
    }
    return TRUE;
  }
  return FALSE;
}
/******************************************************************************************
*  FUNCTION NAME: PopCommandQueue
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
void PopCommandQueue(void)
{
  if (commandQueue.requestCnt)
  {
    commandQueue.requestCnt--;
    if (++commandQueue.requestOut >= MAX_CALLBACK_QUEUE)
    {
      commandQueue.requestOut = 0;
    }
  }
  else
  {
    commandQueue.requestOut = commandQueue.requestIn;
  }

}

/******************************************************************************************
*  FUNCTION NAME: SendAppAck
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/

BOOL  SendAppAck( uint8_t cmd)
{
  if (callbackQueue.requestCnt < MAX_CALLREQ_QUEUE)
  {
    callbackQueue.requestCnt++;

	callbackQueue.requestQueue[callbackQueue.requestIn].wCmd =cmd,
    if (++callbackQueue.requestIn >= MAX_CALLBACK_QUEUE)
    {
      callbackQueue.requestIn = 0;
    }
    return TRUE;
  }
  return FALSE;
}
/******************************************************************************************
*  FUNCTION NAME: PopNoneAckCmdQueue
*  INPUTS:
*  OUTPUTS:
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
void PopNoneAckCmdQueue(void)
{
  if (callbackQueue.requestCnt)
  {
    callbackQueue.requestCnt--;
    if (++callbackQueue.requestOut >= MAX_CALLBACK_QUEUE)
    {
      callbackQueue.requestOut = 0;
    }
  }
  else
  {
    callbackQueue.requestOut = callbackQueue.requestIn;
  }

}

