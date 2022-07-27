
#include "drv_uart.h"
#include "utiles.h"
#include "sys_message.h"
#include "ble_comm.h"

/******************************************************************************************
*  FUNCTION NAME: SendFrame111
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
void AppCommandParse(uint8_t *rx_buf){

	switch(rx_buf[0]){
	case CMD_HANDSHAKE:

		SendQueueAppAck(ACK_CMD_HANDSHAKE);
		put_message(MSG_BLE_CONNECT_IND,1);
		break;
	case CMD_RUNING:

			SendQueueAppAck(ACK_CMD_RUNING);
			put_message(MSG_BLE_CMD_START,rx_buf[1]);

		break;
	case CMD_STOP:
		SendQueueAppAck(ACK_CMD_STOP);
		put_message(MSG_BLE_CMD_STOP,0);
		break;
	case CMD_SET_SPEED:
		SendQueueAppAck(ACK_CMD_SET_SPEED);
		put_message(MSG_BLE_SET_SPEED,(rx_buf[1]<<8|rx_buf[2]));
			break;
 	case CMD_SET_FLOWTIME:
		SendQueueAppAck(ACK_CMD_SET_FLOWTIME);
		put_message(MSG_BLE_SET_FLOWTIME,(uint16_t)rx_buf[1]);
	
		break;
	default:
	break;
	}
}
