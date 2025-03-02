#include "lora_rx_thread.h"
#include "config.h"
#include "rs485_int.h"
#include "utils.h"

#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95(RFM95_NSS, RFM95_INT); // Create the rf95 obj

void lora_listener(void *parameter);
void send_packet(uint8_t lora_data_tx[]);

/**
 * @brief Initializes the LoRa module and configures its settings.
 *        This function sets the frequency, transmission power, and modem configuration.
 *        It loops until the LoRa module is successfully initialized.
 */
void init_lora() //TODO: add timeout to prevent stalling
{
    while (!rf95.init())
    {
        printf("[%s]: LoRa radio init failed\n", NODE_ID);
    }

    printf("[%s]: LoRa setup ok, setting LoRa frequency\n", NODE_ID);

    if (!rf95.setFrequency(RF95_FREQ))
    {
        printf("[%s]: LoRa setup error, LoRa module unable to set frequency\n", NODE_ID);
        while (1);
    }

    printf("[%s]: LoRa setup established to 915 MHz\n", NODE_ID);

    printf("[%s]: LoRa setup, setting sync word.\n", NODE_ID);

    rf95.setTxPower(23, false);
    rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
}

/**
 * @brief LoRa listener thread that continuously listens for incoming LoRa messages.
 *        If a valid message is received, it processes the data and forwards it accordingly.
 * 
 * @param parameter Unused parameter (for FreeRTOS compatibility).
 */
void lora_listener(void *parameter)
{
    PRINT_STR("init lora listen thread");
    
    while (1)
    {
        if (rf95.available())
        {
            uint8_t lora_data_rx[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t buf_len = sizeof(lora_data_rx);

            if (rf95.recv(lora_data_rx, &buf_len))
            {
                if (memcmp(lora_data_rx, NODE_ID, ADDRESS_SIZE) == MEMORY_CMP_SUCCESS)
                {
                    if (!is_rs485_alive)
                    {
                        pinMode(RS485_STATUS_ERROR, OUTPUT);
                        digitalWrite(RS485_STATUS_ERROR, HIGH);

                        printf("rs485 has died\n");
                        vTaskDelete(NULL); // Delete the current task
                    }

                    read_sensor(lora_data_rx);
                    swap_src_dest_addresses(lora_data_rx);
                }
                
                // send packet
                send_packet(lora_data_rx);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}

/**
 * @brief Sends a LoRa packet.
 * 
 * @param lora_data_tx The buffer containing the data to be transmitted.
 */
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