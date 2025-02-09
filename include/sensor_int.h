#ifndef SENSOR_INT_H
#define SENSOR_INT_H

#define READ_DATA_LEN 8
#define RS485_DATA_LEN 19

#include <Arduino.h>

extern uint8_t read_data_msg[];

// Function to process received message
bool read_sensor(uint8_t *rs485_data);
void init_rs485();

#endif // SENSOR_INT_H