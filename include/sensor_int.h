#ifndef SENSOR_INT_H
#define SENSOR_INT_H

#include <Arduino.h>

extern int is_rs485_alive;

// Function to process received message
void read_sensor(uint8_t *rs485_data);
void rs485_poll(void *parameter);
void init_rs485();

#endif // SENSOR_INT_H