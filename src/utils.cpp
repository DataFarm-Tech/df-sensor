#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <Arduino.h>
#include "utils.h"
#include "config.h"

#define MASK 0xFFFF

uint16_t compute_crc16(uint8_t *data, uint8_t length) 
{
    uint16_t crc = MASK;  // Standard Modbus initial value
    for (uint8_t i = 0; i < length; i++) 
    {
        crc ^= data[i];  // XOR with byte
        for (uint8_t j = 0; j < 8; j++) 
        {  // Process 8 bits
            if (crc & 1) 
            {
                crc = (crc >> 1) ^ 0xA001;  // XOR with polynomial
            } 
            else 
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

int is_crc_valid(uint8_t *packet, uint8_t length) 
{
    if (length < 3)
    {
        return 0;  // Must have at least one data byte + 2 CRC bytes
    }

    uint16_t computed_crc = compute_crc16(packet, length - 2);  // Compute CRC over data
    uint16_t received_crc = (packet[length - 1] << 8) | packet[length - 2];  // Extract CRC (little-endian)

    return computed_crc == received_crc;  // Return 1 if CRC matches, 0 otherwise
}


// TODO: move this to utils
void swap_src_dest_addresses(uint8_t buffer[])
{
    uint8_t tmp[ADDRESS_SIZE];
    memcpy(tmp, buffer, ADDRESS_SIZE);
    memcpy(buffer, buffer + ADDRESS_SIZE, ADDRESS_SIZE);
    memcpy(buffer + ADDRESS_SIZE, tmp, ADDRESS_SIZE);
}