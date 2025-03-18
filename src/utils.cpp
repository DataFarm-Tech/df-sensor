#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <Arduino.h>
#include "utils.h"
#include "config.h"

#define MASK 0xFFFF
#define CRC_16_POLY 0xA001

/**
 * @brief Computes the CRC-16 (Modbus) checksum for a given data buffer.
 * 
 * @param data Pointer to the data buffer.
 * @param length Length of the data buffer (excluding CRC bytes).
 * @return The computed 16-bit CRC value.
 */
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
                crc = (crc >> 1) ^ CRC_16_POLY;  // XOR with polynomial
            } 
            else 
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}


/**
 * @brief Validates the CRC-16 (Modbus) checksum of a given packet.
 * 
 * @param packet Pointer to the full packet, including CRC.
 * @param length Length of the entire packet (data + 2 CRC bytes).
 * @return 1 if the CRC is valid, 0 otherwise.
 */
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


/**
 * @brief Swaps the source and destination addresses in a given buffer.
 *        The function assumes that the first ADDRESS_SIZE bytes represent
 *        the source address, and the next ADDRESS_SIZE bytes represent 
 *        the destination address.
 * 
 * @param buffer Pointer to the buffer containing the addresses.
 */
void swap_src_dest_addresses(uint8_t buffer[])
{
    uint8_t tmp[ADDRESS_SIZE];
    memcpy(tmp, buffer, ADDRESS_SIZE);
    memcpy(buffer, buffer + ADDRESS_SIZE, ADDRESS_SIZE);
    memcpy(buffer + ADDRESS_SIZE, tmp, ADDRESS_SIZE);
}


/**
 * @brief Helper function to calc the median of a sorted array of size 'n'
 * @param arr A uint8_t array holding unsorted values
 * @param n The size of arr
*/
uint8_t calc_med(uint8_t arr[], int n) 
{
    sort_arr(arr, n);

    // Return the middle element (median) after sorting
    if (n % 2 != 0) 
    {
        return arr[n / 2];
    } 
    else 
    {
        // Even number of elements, median is the average of the two middle elements
        uint8_t middle1 = arr[n / 2 - 1];
        uint8_t middle2 = arr[n / 2];
        return (middle1 + middle2) / 2; // Average of the two middle elements
    }
}


/**
 * @brief A helper function to sort an array of size 'n' using bubble sort.
 * @param arr array holding unsorted vals
 * @param n size of arr
 * @return void
*/
void sort_arr(uint8_t arr[], int n) 
{
    uint8_t temp;
    
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = i + 1; j < n; j++) 
        {
            if (arr[i] > arr[j]) 
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}