#include "crc.h"
#include "rs485_int.h"

int validate_crc(u_int8_t * buf)
{
    u_int16_t crc = calculate_crc(buf, REC_DATA_LEN - 2);
    
    u_int16_t * p;
    p = (u_int16_t *)&buf[REC_DATA_LEN - 2];

    if (crc == *p)
    {
        return 0;
    }

    return 1;
}

u_int16_t calculate_crc(u_int8_t *data, size_t length) 
{
    uint16_t crc = 0xFFFF;  // Start with CRC = 0xFFFF

    for (size_t i = 0; i < length; i++) 
    {
        uint8_t byte = data[i];
        crc ^= byte;  // XOR byte into CRC

        // Process each bit of the byte
        for (uint8_t bit = 0; bit < 8; bit++) 
        {
            if (crc & 0x0001) 
            {
                crc >>= 1;
                crc ^= 0xA001;  // Polynomial used in Modbus CRC
            } 
            else 
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}


