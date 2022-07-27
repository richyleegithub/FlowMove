
#include "drv_timer.h"
#include "types.h"
#include "sys_message.h"


#define   	EV_TIMER_SUM			8


ev_timer_t   ev_timer[EV_TIMER_SUM];

uint8_t   	drv_timer_tick;

/******************************************************************************************
*  FUNCTION NAME: 	  
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
void Init_evTimers(void)
{
  uint8_t u8_Index;

	  for(u8_Index = 0; u8_Index < EV_TIMER_SUM; u8_Index++)
  	{
      		ev_timer[u8_Index].delay = 0;
  	}
}


/******************************************************************************************
*  FUNCTION NAME: 	  
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
void Start_evTimer(	uint16_t Time,	uint8_t ev	   )
{
  ev_timer_t  *pTimer;
  ev_timer_t  *pFreeTimer = NULL;
  uint8_t     u8_Index;

 	  	pTimer = ev_timer;
  		for(u8_Index = 0; u8_Index < EV_TIMER_SUM; u8_Index++)
  		{
      			if(pTimer->delay == 0)
      			{
	 				 /* This timer entry is free i.e. timer not running */
                    if(pFreeTimer == NULL)
                    {
	     				 /* This is the first free entry in the timer array */
	      				pFreeTimer = pTimer;
                    }
     			 }
    			  else
      			{

                      if(pTimer->ev == ev)
                      {
	      					/* This running timer has the same destination as
	       				 * the requested one
	       				*/
	      					if(pFreeTimer != NULL)
	      					{
		 					 /* We found a free timer entry with a lower index already.
		   					  * Use this entry to save time 
		   					  */
                                  pTimer->delay = 0;
	      					 }
	      					 else
	     					 {
                                pFreeTimer = pTimer;
	      					 }
	      					break; /* Abort search over array */
                      }
    			  }
    	  pTimer++;
 	 }

 	 pFreeTimer->delay   = Time;
  	 pFreeTimer->ev      = ev;

}

/******************************************************************************************
*  FUNCTION NAME: 	  
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
void Stop_evTimer(uint8_t ev){
  ev_timer_t  *pTimer;
  uint8_t      u8_Index;

   //BoardDisableIrq();
   	
   	pTimer = ev_timer;
   	for(u8_Index = 0; u8_Index < EV_TIMER_SUM; u8_Index++)
  	{
      	if (pTimer->delay)
      	{
 
			 if(pTimer->ev == ev) 
		  	{
	      		pTimer->delay = 0; /* Timer entry found */
		  	}
     	}
      	pTimer++;
  	}

    
 	//BoardEnableIrq();
 	
}

/******************************************************************************************
*  FUNCTION NAME: 	  
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

void drv_time(void){


ev_timer_t  *pTimer;
uint8_t 	u8_Index;
		if(drv_timer_tick<DRV_TIMER_TICK) return ;
		drv_timer_tick=0;

	  	pTimer=ev_timer;
		for(u8_Index = 0; u8_Index < EV_TIMER_SUM; u8_Index++)
	    {
			  if(pTimer->delay != 0)
			  {
			  		if(pTimer->delay <= 1)
			  		{
				  		pTimer->delay = 0; /* Timer expired */
				  		put_message( pTimer->ev,0);
			  		}
			  		else
			  		{
					 	 pTimer->delay --; /* Timer still running */
			  		}
			  }
			  pTimer++;
	  }

}
