#include	"MC96FD316.h"
#include 	"types.h"
#include 	"drv_uart.h"
#include 	"utiles.h"
#include 	"ble_comm.h"
#include 	"sys_message.h"



USART_FIFO_t    xdata	 usart_fifo;
DRVUART_RX_t  	xdata ComRxCtrl;
DRVUART_TX_t  	xdata ComTxCtrl;
uint8_t 	  	xdata commState;
uint8_t			xdata  comSubState;
uint8_t			xdata  rx_buf[2];
uint16_t			rx_timeout;
uint16_t			rx_byte_timeout;
uint8_t			xdata ConnectData[10];
code uint8_t	ConnectInd[10]="Connect:1";
code uint8_t	DisconnectInd[10]="Connect:0";
/*=========================================================
* FUNCTION NAME: drv_uart_ini
* input :				 none
* Output:				 none
* description:    
*********************************************************
author				release							date
*********************************************************
richy.lee			first release				2022/04/21
===========================================================*/

void drv_uart_ini(void){
	usart_fifo.rd=0;
	usart_fifo.wr=0;
	commState=STATE_AT_MODE;
	comSubState=COMMAND_AT_NONE;
	InitialQueue();

}

void drv_at_mode(void){
	comSubState=COMMAND_AT;
}
/*=========================================================
* FUNCTION NAME: drv_uart_ini
* input :				 none
* Output:				 none
* description:    
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

#define drv_uart_put( chr)\
			do{\
				usart_fifo.fifo[usart_fifo.wr]=chr;\
				usart_fifo.wr++;\
				if(usart_fifo.wr>=UART_FIFO_LENGTH){\
					usart_fifo.wr=0;\
				}\
			}while(0)

/*=========================================================
* FUNCTION NAME: drv_uart_get
* input :				 none
* Output:				 none
* description:    
*********************************************************
author					release							date
*********************************************************
richy.lee				first release				2022/04/21
===========================================================*/

BOOL drv_uart_get(uint8_t *chr){
BOOL ret=FALSE;
	if(usart_fifo.rd!=usart_fifo.wr){
		*chr=usart_fifo.fifo[usart_fifo.rd];
		usart_fifo.rd++;
		if(usart_fifo.rd>=UART_FIFO_LENGTH){
			usart_fifo.rd=0;
		}
		ret=TRUE;
	}
	return ret;
}
/******************************************************************************************
*  FUNCTION NAME: INT_USI_Rx
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

void INT_USI_Rx() interrupt 9
{
	uint8_t temp;
	temp=USIST1;
	if(temp & 0x20)
	{
		temp=USIDR;
		drv_uart_put(temp);
		
	}

}



#ifndef UART_TX_INT
/******************************************************************************************
*  FUNCTION NAME: SendFrame
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
void drv_uart_tx(void){
	switch(ComTxCtrl.state){
	case UART_TX_START:
		ComTxCtrl.index=0;
		if((USIST1 & 0x80)==0x80){
			USIDR =  ComTxCtrl.buf[ComTxCtrl.index];
			ComTxCtrl.index++;
			ComTxCtrl.len--;
			ComTxCtrl.state=UART_TX_WAIT;
		}
		break;
	case UART_TX_WAIT:
		if((USIST1 & 0x80)==0x80){
			if(ComTxCtrl.len>0)
			{

				USIDR =  ComTxCtrl.buf[ComTxCtrl.index];
				ComTxCtrl.index++;
				ComTxCtrl.len--;
			}else{
				ComTxCtrl.state=UART_TX_IDLE;
			}
		}
		break;
	default:
		break;
	}

}
#else	
/******************************************************************************************
*  FUNCTION NAME: INT_USI_Rx
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

void INT_USI_Tx() interrupt 10
{
	uint8_t temp;
	temp=USIST1;

	if((temp & 0x80)==0x80){
		if(ComTxCtrl.len>0){
			USIDR =  ComTxCtrl.buf[ComTxCtrl.index];
			ComTxCtrl.index++;
			ComTxCtrl.len--;
		}else{
			ComTxCtrl.state=UART_TX_IDLE;
		}

	}

}

/******************************************************************************************
*  FUNCTION NAME: SendATcommad
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
void Send_Start(void)
{
	while(!(USIST1 & 0x80));	// wait
	ComTxCtrl.index=0;
	USIDR = ComTxCtrl.buf[ComTxCtrl.index];
	ComTxCtrl.len--;
}
#endif


/******************************************************************************************
*  FUNCTION NAME: SendATcommad
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
void SendATcommad(void){
		ComTxCtrl.buf[0]='+';		  //head
		ComTxCtrl.buf[1]='+'; 		  //length is fix 2
		ComTxCtrl.buf[2]='+';		  //cmd
		ComTxCtrl.len=3;
		ComTxCtrl.state=UART_TX_START;
#ifdef UART_TX_INT		
		Send_Start();
#endif



}


/******************************************************************************************
*  FUNCTION NAME: SendATNameChange
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
void SendATNameChange(void){
		ComTxCtrl.buf[0]='A';		  //head
		ComTxCtrl.buf[1]='T'; 		  //length is fix 2
		ComTxCtrl.buf[2]='+';		  //cmd
		ComTxCtrl.buf[3]='s';
		ComTxCtrl.buf[4]='e';
		ComTxCtrl.buf[5]='t';
		ComTxCtrl.buf[6]='N';
		ComTxCtrl.buf[7]='a';
		ComTxCtrl.buf[8]='m';
		ComTxCtrl.buf[9]='e';
		ComTxCtrl.buf[10]='=';
		ComTxCtrl.buf[11]='F';
		ComTxCtrl.buf[12]='l';
		ComTxCtrl.buf[13]='o';
		ComTxCtrl.buf[14]='w';
		ComTxCtrl.buf[15]='M';
		ComTxCtrl.buf[16]='o';
		ComTxCtrl.buf[17]='v';
		ComTxCtrl.buf[18]='e';
		ComTxCtrl.buf[19]='\r';
		ComTxCtrl.buf[20]='\n';
		ComTxCtrl.len=21;
		ComTxCtrl.state=UART_TX_START;

		
#ifdef UART_TX_INT		
		Send_Start();
		
#endif

}
/******************************************************************************************
*  FUNCTION NAME: SendATexit
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
void SendATexit(void){
		ComTxCtrl.buf[0]='A';		  //head
		ComTxCtrl.buf[1]='T'; 		  //length is fix 2
		ComTxCtrl.buf[2]='+';		  //cmd
		ComTxCtrl.buf[3]='e';
		ComTxCtrl.buf[4]='x';
		ComTxCtrl.buf[5]='i';
		ComTxCtrl.buf[6]='t';
		ComTxCtrl.buf[7]='\r';
		ComTxCtrl.buf[8]='\d';
		ComTxCtrl.len=9;
		ComTxCtrl.state=UART_TX_START;

#ifdef UART_TX_INT		
				Send_Start();
#endif

}

/******************************************************************************************
*  FUNCTION NAME: SendAckFrame
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
void SendAckFrame(  uint8_t    cmd) {

	  ComTxCtrl.buf[0]=0x9a; 		//head
	  ComTxCtrl.buf[1]=2;   		//length is fix 2
	  ComTxCtrl.buf[2]=cmd;			//cmd
	  ComTxCtrl.buf[3]=(SOF+ ComTxCtrl.buf[1]+cmd); //
	  ComTxCtrl.len=4;
	  ComTxCtrl.state=UART_TX_START;
#ifdef UART_TX_INT		
	  Send_Start();
#endif

}

/******************************************************************************************
*  FUNCTION NAME: SendNoticeFrame
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
void SendNoticeFrame(  uint8_t    pressure,uint8_t temperature,uint8_t battery)  
{

	  ComTxCtrl.buf[0]=SOF; 		//head
	  ComTxCtrl.buf[1]=0x05;   		//length is fix 5
	  ComTxCtrl.buf[2]=0x85;		//cmd
	  ComTxCtrl.buf[3]=pressure;
	  ComTxCtrl.buf[4]=temperature;
	  ComTxCtrl.buf[5]=battery;
	  ComTxCtrl.buf[6]=(SOF+5+0x85+pressure+temperature+battery); //
	  ComTxCtrl.len=7;
	  	ComTxCtrl.state=UART_TX_START;
#ifdef UART_TX_INT		
	  Send_Start();
#endif



	  
}


/******************************************************************************************
*  FUNCTION NAME: IsTxComplete
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
BOOL   IsTxComplete(void){

	return (ComTxCtrl.state==UART_TX_IDLE);
}



/******************************************************************************************
*  FUNCTION NAME: ReceiveUartAtOk
*  INPUTS:  BOOL
*  OUTPUTS: eUartStatus_t
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
eUartStatus_t    ReceiveUartAtOk( void ){
uint8_t  chr;
eUartStatus_t retVal = UART_NONE;

		while (drv_uart_get(&chr)){
			
		    //USIDR=chr;

			rx_buf[0]=rx_buf[1];
			rx_buf[1]=chr;
			if((rx_buf[0]=='o')&&(rx_buf[1]=='k')){
				retVal = UART_FRAME_RECEIVED;
				break;
			}
		}
		return retVal;
}
/******************************************************************************************
*  FUNCTION NAME: ReceiveUartFrame
*  INPUTS:  BOOL
*  OUTPUTS: eUartStatus_t
*  VALUE RETURNED: 	   void.
*  DESCRIPTION:   	 
*  CAUTIONS: 		.
******************************************************************************************
* MODIFICATION HISTORY:
* Date      		Person        					Change
* ---------------------------------------------------------------------------------------
* 9/06/2013    LIRIZHI (LRZ)         Initial creation
******************************************************************************************/
eUartStatus_t    ReceiveUartFrame( void ) {
uint8_t  chr;
uint8_t  i;
eUartStatus_t retVal = UART_NONE;
  
	  while (drv_uart_get(&chr))
	  {
	  		for(i=1;i<9;i++){
	  			ConnectData[i-1]=ConnectData[i];
	  		}
			ConnectData[8]=chr;
			
			for(i=0;i<9;i++){
				if(ConnectData[i]!=DisconnectInd[i]){
					break;
				}
			}
			if(i==9){
				put_message(MSG_BLE_DISCONNECT_IND,0);
			}
				


	      switch (ComRxCtrl.state)
	      {
	        case REC_SOF :
	          if (chr == SOF)
	          {
					
					ComRxCtrl.rxActive=1;
					ComRxCtrl.index=0;

					ComRxCtrl.buf[ComRxCtrl.index] = SOF;
					ComRxCtrl.index++;
					
					ComRxCtrl.bChecksum = SOF;
					rx_byte_timeout=0;
	            	ComRxCtrl.state=REC_LEN;
	          }
	          break;

	        case REC_LEN :
	          if ((chr < FRAME_DATA_MIN_LENGTH) || (chr > (RX_BUF_LENGHT-1)))
	          {
	          	ComRxCtrl.rxActive=0;
	            ComRxCtrl.state = REC_SOF; // Restart looking for SOF
	            
	          }
			  else
			  {
				ComRxCtrl.len=chr-1;
			  	ComRxCtrl.buf[ComRxCtrl.index] = chr;
				ComRxCtrl.index++;
				
				ComRxCtrl.bChecksum += chr;
				rx_byte_timeout=0;
			   	ComRxCtrl.state=REC_DATA;
	          }
			  break;

	        case REC_DATA :
				rx_byte_timeout=0;

            	ComRxCtrl.buf[ComRxCtrl.index] = chr;
            	ComRxCtrl.index++;
				ComRxCtrl.len--;
				ComRxCtrl.bChecksum += chr;

	            if (ComRxCtrl.len==0)
	            {
    	          	ComRxCtrl.state=REC_CHKSUM;
        	    }
				rx_byte_timeout=0;

			 
	          break;

	        case REC_CHKSUM :

			 
	          if (chr == ComRxCtrl.bChecksum)
	          {
	              retVal = UART_FRAME_RECEIVED;  // Tell THE world that we got a packet
	          }
	          else
	          {
	              retVal = UART_FRAME_ERR;
	          }
			  ComRxCtrl.rxActive=0;
	          ComRxCtrl.state = REC_SOF; // Restart looking for SOF
	          break;
            default :

	          break;
	      }
	  } 

   
	  /* Check for timeouts - if no other events detected */
	  if (retVal == UART_NONE)
	  {
	    /* Are we in the middle of collecting a frame and have we timed out? */
	    if ((ComRxCtrl.rxActive==1) 
			&& (  rx_byte_timeout>=1000))
	    {
		      /* Reset to SOF hunting */
		      ComRxCtrl.state = REC_SOF;
		      ComRxCtrl.rxActive = 0; /* Not inframe anymore */
		      retVal = UART_RECEIVE_TIMEOUT;
	    }
	  }
	  return retVal;
}


/*=======================================================================
* FUNCTION NAME: drv_uart
* input :				 none
* Output:				 none
* description:    
************************************************************************
author					release						date
************************************************************************
richy.lee				first release			2022/04/21
=========================================================================*/
void drv_comm(void){

 switch (commState){
  case STATE_AT_MODE:
  		switch(comSubState){
		case   COMMAND_AT_NONE:
			rx_timeout=0;
				break;
		case	COMMAND_AT:
			if(rx_timeout>=1000){
				SendATcommad();
				rx_timeout=0;
				comSubState=COMMAND_AT_WAIT;
			}
			break;
		case COMMAND_AT_WAIT:
			if( ReceiveUartAtOk()==UART_FRAME_RECEIVED){
			  SendATNameChange();
			  rx_timeout=0;
			  comSubState=COMMAND_CHANGE_NAME_WAIT;
			}else{
				if(rx_timeout>=1000){
					rx_timeout=0;
					SendATexit();
					comSubState=COMMAND_EIXT_AT_WAIT;
				}
			}
			break;
		case COMMAND_CHANGE_NAME_WAIT:
			if( ReceiveUartAtOk()==UART_FRAME_RECEIVED){
				rx_timeout=0;
				SendATexit();
				 comSubState=COMMAND_EIXT_AT_WAIT;
			}else{
				if(rx_timeout>=1000){
					SendATexit();
					rx_timeout=0;
					comSubState=COMMAND_EIXT_AT_WAIT;
				}
			}
			break;
		case COMMAND_EIXT_AT_WAIT:
			if( ReceiveUartAtOk()==UART_FRAME_RECEIVED){
				commState=STATE_IDLE;
			}else{
			
				if(rx_timeout>=1000){
					commState=STATE_IDLE;
				}
			}
			break;
		default:
			break;
  		}


		break;
   case STATE_IDLE:
	   if(callbackQueue.requestCnt){
		   // 应答发送，优先级比较高。
		   if(ComTxCtrl.state==UART_TX_IDLE){
			   SendAckFrame(  callbackQueue.requestQueue[callbackQueue.requestOut].wCmd);
			   commState=STATE_COMMAND_ACK_TX;
		   	}

	   }else if (commandQueue.requestCnt){
		   // 普通命令发送。
		     if(ComTxCtrl.state==UART_TX_IDLE){
		 		SendNoticeFrame(  commandQueue.requestQueue[commandQueue.requestOut].wBuf[0],\
					 				commandQueue.requestQueue[commandQueue.requestOut].wBuf[1],\
					 				commandQueue.requestQueue[commandQueue.requestOut].wBuf[2]	);
			 	commState=STATE_COMMAND_TX;
		     }
		 
	   }
	   else
	   {
		 /* Nothing to transmit. Check if we received anything */
		 if (ReceiveUartFrame() == UART_FRAME_RECEIVED)
		 {
		   		AppCommandParse(&ComRxCtrl.buf[FRAM_CMD_INDEX]);
		 }
	   }
 
   break;
   case STATE_COMMAND_ACK_TX:
	   if(IsTxComplete()){
		   PopAckQueue();// 发送结束，清除队列里面的ACK 命令
		   commState=STATE_IDLE;
	   }
   	  break;
   case STATE_COMMAND_TX:
	   if(IsTxComplete()){
			PopCommandQueue();// 发送结束，清除队列里面的命令
			commState=STATE_IDLE;
	  }
   break;
   default:
	 commState=STATE_IDLE;
	 break;
 }

}

