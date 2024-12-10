#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#include "config.h"
#include "rs485_int.h"

RH_RF95 rf95(RFM95_NSS, RFM95_DIO0);

void setup(void)
{
    Serial.begin(115200); //start serial

    while (!Serial) 
    {
        // wait for serial to initialise
    }

    Serial2.begin(RS485_BAUD, SERIAL_8N1, RS485_RX, RS485_TX);
    pinMode(RS485_RTS, OUTPUT);
    digitalWrite(RS485_RTS, LOW); // SET PIN NORMALLY LOW

    while (!rf95.init())
    {
        printf("Lora not connecting\n");
    }

    if (!rf95.setFrequency(RF95_FREQ))
    {
        printf("incorrect frequency setup\n");
        while(1);
    }

    rf95.setTxPower(23, false);
    rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);

    printf("[%s] init serial console\n", ID);
}

void loop(void) 
{
    uint8_t rs485_data[13];

    if (!read_sensor(rs485_data)) 
    {
        printf("Failed to read sensor\n");
    } 

    if (rf95.send(rs485_data, sizeof(rs485_data)))
    {
        printf("[%s] packet successfully sent\n", ID);
        
        // Print the sensor data values along with the nodeId

        Serial.printf("[%s] Humidity: %d\n", ID, rs485_data[0]);
        Serial.printf("[%s] Temperature: %d\n", ID, rs485_data[1]);
        Serial.printf("[%s] Conductivity: %d\n", ID, rs485_data[2]);
        Serial.printf("[%s] pH: %d\n", ID, rs485_data[3]);
        Serial.printf("[%s] Nitrogen: %d\n", ID, rs485_data[4]);
        Serial.printf("[%s] Phosphorus: %d\n", ID, rs485_data[5]);
        Serial.printf("[%s] Potassium: %d\n", ID, rs485_data[6]);
    }
    else 
    {
        printf("[%s] error: unable to send packet\n", ID);
    }
    
    
    delay(20000);
}