#include "lora_rx_thread.h"
#include "config.h"

#include <SPI.h>
#include <RH_RF95.h>


RH_RF95 rf95(RFM95_NSS, RFM95_INT); // Create the rf95 obj

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
        while (1);
    }

    printf("[%s]: LoRa setup established to 915 MHz\n", NODE_ID);

    printf("[%s]: LoRa setup, setting sync word.\n", NODE_ID);

    rf95.setTxPower(23, false);
    rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
}

/*
TODO: write thread to listen to lora msg
*/
void lora_listener(void* parameter)
{
    while(1) 
    {
        if (rf95.available()) 
        {
            uint8_t rec_buf[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t buf_len = sizeof(rec_buf);
            
            if (rf95.recv(rec_buf, &buf_len))
            {                
                //TODO: what happens when we rec
                for (int i=0; i < buf_len; i++)
                {
                    printf("%x", rec_buf[i]);
                }
                printf("\n");
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}