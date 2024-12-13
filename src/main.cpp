#include "lora_rx_thread.h"
#include "lora_tx_thread.h"
#include "config.h"
#include "types.h"

#include <RH_RF95.h>

RH_RF95 rf95(RFM95_CS, RFM95_INT); //create the rf95 obj

lora_rx_thread* lora_Rx_thread = nullptr;
lora_tx_thread* lora_Tx_thread = nullptr;

std::queue<data> upload_queue;

void setup(void)
{
    Serial.begin(115200);
    
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    printf("[%s]: lora setup ok, initialise module\n", ID);

    while (!rf95.init()) 
    {
        printf("[%s]: LoRa radio init failed\n", ID);
    }

    printf("[%s]: lora setup ok, setting lora freq\n", ID);

    if (!rf95.setFrequency(RF95_FREQ)) 
    {
        printf("[%s]: lora setup error, lora module unable to set frequency\n", ID);
        while (1);
    }

    printf("[%s]: lora setup established to 915 Mhz \n", ID);

    printf("[%s]: lora setup, setting sync word.\n", ID);

    rf95.setTxPower(23, false);
    rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);

    lora_Rx_thread = new lora_rx_thread(rf95);
    lora_Rx_thread->start();
    
    lora_Tx_thread = new lora_tx_thread(rf95);
    lora_Tx_thread->start();

    //init threads
    
    //lora_rx_thread
    //while lora is available:
    //  recv:
    //if dest_node is equal to nodeId:

    // case switch(msg_id):
    //      case 0x01: (env request)
    //      
    // 
    //else:
    //    write_to_buffer()  
    //    relay_msg()
    //      

    //lora_rx_thread
}

void loop(void) 
{
    //
}