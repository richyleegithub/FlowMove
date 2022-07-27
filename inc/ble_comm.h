#ifndef BLE_COMM_H
#define BLE_COMM_H

#define CMD_HANDSHAKE			0X00
#define CMD_RUNING				0X01
#define CMD_STOP				0X02
#define CMD_SET_SPEED			0X03  //big 
#define CMD_SET_FLOWTIME		0X04

#define ACK_CMD_HANDSHAKE		0X80
#define ACK_CMD_RUNING			0X81
#define ACK_CMD_STOP			0X82
#define ACK_CMD_SET_SPEED		0X83
#define ACK_CMD_SET_FLOWTIME	0X84
#define NOTICE_IND				0X85

void AppCommandParse(uint8_t *rx_buf);


#endif//




