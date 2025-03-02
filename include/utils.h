
#include <stdio.h>
#include <stdint.h>

#define PRINT_STR(to_print) printf("[%s]: %s\n", NODE_ID, to_print)
#define PRINT_INT(to_print) printf("[%s]: %d\n", NODE_ID, to_print)

uint16_t compute_crc16(uint8_t *data, uint8_t length);
void swap_src_dest_addresses(uint8_t buffer[]);