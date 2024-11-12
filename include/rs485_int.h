#ifndef RS485_INT_H
#define RS485_INT_H

#define REC_DATA_LEN 19
#define READ_DATA_LEN 8

#include <Arduino.h>

extern u_int8_t read_data_msg[];
extern u_int8_t rec_data_buffer[REC_DATA_LEN];

// Structure to store processed data
typedef struct {
    float humidity; // Humidity value (e.g., 12.3)
    float ph;       // pH value (e.g., 45.0)
    float conductivity;
    float temperature;
    float nitrogen;
    float phosphorus;
    float potassium; 
} data;

// Function to process received message
int process_rs485_msg(u_int8_t rec_data[REC_DATA_LEN], data *rs485_data);

#endif // RS485_INT_H
