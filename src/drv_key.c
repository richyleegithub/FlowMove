#include  "MC96FD316.h"
#include "types.h"
#include "sys_message.h"
#include "drv_key.h"

#define 	KEY_TICK				(5)
#define 	DEBOUCE_TIME			(40/KEY_TICK)
#define 	LONG_KEY_PRESS_TIME 	(1500/KEY_TICK)
#define 	LONG_KEY_PRESS_TIME5S	(2000/KEY_TICK)
#define 	KEY_PORT				P1
#define 	KEY_PIN 				0X01



KEY_CTRL_t  keyCtrl;
uint8_t 	drv_key_tick;
/******************************************************************************************
*  FUNCTION NAME: drv_key
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
void drv_key(void){
  	  if(drv_key_tick<5) return;
	  drv_key_tick=0;
	  
      switch(keyCtrl.sate){   
      case  KEY_SCAN_IDLE:
          keyCtrl.debouce=0;
	  	  keyCtrl.longKeyPress=0;
          keyCtrl.release_time=1;
          keyCtrl.sate=KEY_SCAN_CHECK;
          break;
      
      case  KEY_SCAN_CHECK:
          
          if((KEY_PORT&KEY_PIN)==0){
            keyCtrl.debouce=0;
            keyCtrl.sate=KEY_SCAN_DEBOUCNE;
          }
          break;
          
      case  KEY_SCAN_DEBOUCNE:
        if((KEY_PORT&KEY_PIN)==0){
            keyCtrl.debouce++;
            if(keyCtrl.debouce>=DEBOUCE_TIME){
              keyCtrl.debouce=0;
             // put_message(MSG_KEY_PRESS,0);
              keyCtrl.sate=KEY_SCAN_RELEASE;
            }
        }else{
          keyCtrl.sate=KEY_SCAN_IDLE;
        }
        break;
        
        
      case  KEY_SCAN_RELEASE:
        if((KEY_PORT&KEY_PIN)==0){
                                                    
          keyCtrl.debouce=0;
          if(keyCtrl.release_time>0){
              keyCtrl.release_time++;
          }

		  if(keyCtrl.release_time>LONG_KEY_PRESS_TIME5S){
			  keyCtrl.release_time=0;
  		  	  put_message(MSG_KEY_TEST_PRESS,0);
		  } 
		  else
		  {

			if(keyCtrl.release_time>LONG_KEY_PRESS_TIME){
            	if(keyCtrl.longKeyPress==0){
		  	  	 	keyCtrl.longKeyPress=1 ;
              		put_message(MSG_KEY_LONG_PRESS,0);
				}
          	}
		  }
          
        }
		else
		{
        keyCtrl.debouce++;
        if(keyCtrl.debouce>=DEBOUCE_TIME){
              keyCtrl.debouce=0;
			  if((keyCtrl.longKeyPress==1)
			  	||(keyCtrl.release_time==0))
			  {
			  	put_message(MSG_KEY_RELEASE,0);
				
			  }else{
			   put_message(MSG_KEY_PRESS,0);
				 
			  }
              keyCtrl.sate=KEY_SCAN_IDLE;
          }
          
        }
        break;
        
      default:
        break;
      
      }
}


