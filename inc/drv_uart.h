#ifndef DRV_UART_H
#define DRV_UART_H


#include "types.h"




#define  RECEIVE_TIMOUT			200
#define  SOF					0x9A



#define  UART_FIFO_LENGTH		16
#define  RX_BUF_LENGHT			16
#define  TX_BUF_LENGTH			32

#define  FRAME_DATA_MAX_LENGTH		4
#define  FRAME_DATA_MIN_LENGTH		1

#define	 FRAME_SOF_INDEX			0
#define  FRAME_LEN_INDEX			1
#define  FRAM_CMD_INDEX				2

 typedef enum 
 {
   UART_NONE,					 // returned if nothing special has happened
   UART_FRAME_RECEIVED, 		 // returned when a valid frame has been received
   UART_FRAME_ERR,				 // returned if frame has error in Checksum
   UART_RECEIVE_TIMEOUT,		 // returned if Rx timeout has happened
 }eUartStatus_t;

 enum
{
	  REC_SOF = 0,
	  REC_LEN ,
	  REC_DATA ,
	  REC_CHKSUM 
};


enum{
	UART_TX_IDLE,
	UART_TX_START,
	UART_TX_WAIT,
};


enum{
	STATE_AT_MODE,
	STATE_IDLE,
	STATE_COMMAND_ACK_TX,
	STATE_COMMAND_TX,
};

enum{
	COMMAND_AT_NONE,
	COMMAND_AT,
	COMMAND_AT_WAIT,
	COMMAND_CHANGE_NAME_WAIT,
	COMMAND_ADS_WAIT,
	COMMAND_EIXT_AT_WAIT,
};

typedef struct{
	uint8_t rd;
	uint8_t wr;
	uint8_t  fifo[UART_FIFO_LENGTH];
}USART_FIFO_t;


 typedef struct{
 	 uint8_t 		state;
	 uint8_t		rxActive;
	 uint8_t		bChecksum;
	 uint8_t		index;
	 uint8_t        len;
	 uint8_t		 buf[RX_BUF_LENGHT];
 }DRVUART_RX_t;

 typedef struct{
	uint8_t state;
	uint8_t	req;
	uint8_t	index;
	uint8_t len;
	uint8_t buf[TX_BUF_LENGTH];

 }DRVUART_TX_t;

 extern uint16_t			rx_timeout;
 extern uint16_t			 rx_byte_timeout;

 void drv_uart_ini(void);

 void drv_comm(void);
 void drv_uart_tx(void);
 void SendAckFrame(  uint8_t    cmd);
  void SendNoticeFrame(	uint8_t    pressure,uint8_t temperature,uint8_t battery)  ;
  void SendAckFrame(  uint8_t    cmd);
  eUartStatus_t    ReceiveUartFrame( void );
  
  void SendATcommad(void);
  void SendATNameChange(void);
  void SendATexit(void);
  void drv_at_mode(void);


#endif
