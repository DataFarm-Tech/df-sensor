#ifndef CONFIG_H
#define CONFIG_H

#define BAUD_RATE 115200

#define RS485_RX 16
#define RS485_TX 17
#define RS485_RTS 4
#define RS485_BAUD 4800

#define RFM95_NSS 5	 // NSS/CS P
#define RFM95_RST 14 // RST
#define RFM95_INT 27 // Interrupt/DIO0
#define RFM95_MOSI 23
#define RFM95_MISO 19
#define RFM95_SCK 18
#define RF95_FREQ 915.0 // DO NOT CHANGE FROM 915MHZ

#define NODE_ID "testN1"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define ADDRESS_SIZE 6
#define MEMORY_CMP_SUCCESS 0

#endif // CONFIG_H
