#include "lora_rx_thread.h"
#include "config.h"
#include "sensor_int.h"

#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95(RFM95_NSS, RFM95_INT); // Create the rf95 obj

void lora_listener(void *parameter);
void swap_src_dest_addresses(uint8_t buffer[]);
void send_packet(uint8_t lora_data_tx[]);

void setup_lora()
{
    while (!rf95.init())
    {
        printf("[%s]: LoRa radio init failed\n", NODE_ID);
    }

    printf("[%s]: LoRa setup ok, setting LoRa frequency\n", NODE_ID);

    if (!rf95.setFrequency(RF95_FREQ))
    {
        printf("[%s]: LoRa setup error, LoRa module unable to set frequency\n", NODE_ID);
        while (1)
            ;
    }

    printf("[%s]: LoRa setup established to 915 MHz\n", NODE_ID);

    printf("[%s]: LoRa setup, setting sync word.\n", NODE_ID);

    rf95.setTxPower(23, false);
    rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
}

/*
TODO: write thread to listen to lora msg
*/
void lora_listener(void *parameter)
{
    while (true)
    {
        if (rf95.available())
        {
            uint8_t lora_data_rx[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t buf_len = sizeof(lora_data_rx);

            if (rf95.recv(lora_data_rx, &buf_len))
            {
                printf("lora_data_rx: %s\n", lora_data_rx);
                if (memcmp(lora_data_rx, NODE_ID, ADDRESS_SIZE) == MEMORY_CMP_SUCCESS)
                // if (true)
                {
                    printf("Lora packet received\n");
                    // process packet
                    bool res = read_sensor(lora_data_rx);
                    if (res)
                        printf("Sensor reading worked\n");

                    // swap src and dest addresses
                    swap_src_dest_addresses(lora_data_rx);
                }
                printf("lora_data_rx: %s\n", lora_data_rx);
                printf("%d ", lora_data_rx[12]);
                printf("%d ", lora_data_rx[13]);
                printf("%d ", lora_data_rx[14]);
                printf("%d ", lora_data_rx[15]);
                printf("%d ", lora_data_rx[16]);
                printf("%d ", lora_data_rx[17]);
                printf("%d ", lora_data_rx[18]);

                // send packet
                send_packet(lora_data_rx);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}

// TODO: move this to utils
void swap_src_dest_addresses(uint8_t buffer[])
{
    uint8_t tmp[ADDRESS_SIZE];
    memcpy(tmp, buffer, ADDRESS_SIZE);
    memcpy(buffer, buffer + ADDRESS_SIZE, ADDRESS_SIZE);
    memcpy(buffer + ADDRESS_SIZE, tmp, ADDRESS_SIZE);
}

void send_packet(uint8_t lora_data_tx[])
{
    if (rf95.send(lora_data_tx, LORA_DATA_LEN))
    {
        printf("Packet sent\n");
    }
    else
    {
        printf("Packet failed\n");
    }
}