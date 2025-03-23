#include <iostream>
#include <cassert>
// #include "utils.h"
// #include <SPI.h>
// #include <RH_RF95.h>
// #include <Arduino.h>
#include <cstring>


#define ADDRESS_SIZE 6

void test_swap_src_dest_addresses();
void swap_src_dest_addresses(uint8_t buffer[]);


void test_swap_src_dest_addresses() {
    // Initialize a buffer with known source and destination addresses
    uint8_t buffer[2 * ADDRESS_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06,  // Source address
                                        0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C}; // Destination address

    // Expected buffer after swapping
    uint8_t expected_buffer[2 * ADDRESS_SIZE] = {0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,  // Destination address
                                                 0x01, 0x02, 0x03, 0x04, 0x05, 0x06}; // Source address

    // Call the function to swap addresses
    swap_src_dest_addresses(buffer);

    // Check if the buffer matches the expected buffer
    for (int i = 0; i < 2 * ADDRESS_SIZE; ++i) {
        assert(buffer[i] == expected_buffer[i]);
    }

    std::cout << "test_swap_src_dest_addresses passed!" << std::endl;
}

void swap_src_dest_addresses(uint8_t buffer[])
{
    uint8_t tmp[ADDRESS_SIZE];
    memcpy(tmp, buffer, ADDRESS_SIZE);
    memcpy(buffer, buffer + ADDRESS_SIZE, ADDRESS_SIZE);
    memcpy(buffer + ADDRESS_SIZE, tmp, ADDRESS_SIZE);
}

int main() {
    // Initialize your hardware and libraries here
    test_swap_src_dest_addresses();
}