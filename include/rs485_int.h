#ifndef RS485_INT_H
#define RS485_INT_H

#define REC_DATA_LEN 19
#define READ_DATA_LEN 8

#include <Arduino.h>

extern uint8_t read_data_msg[];
extern uint8_t rec_data_buffer[REC_DATA_LEN];

// Function to process received message
bool read_sensor(uint8_t* rs485_data);

#endif // RS485_INT_H
