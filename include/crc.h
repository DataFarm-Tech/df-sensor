#ifndef CRC_H
#define CRC_H

#include <Arduino.h>

// Function to process received message
int validate_crc(u_int8_t * buf);
u_int16_t calculate_crc(u_int8_t *data, size_t length);

#endif // CRC_H
