#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <RHMesh.h>

#include "config.h"
#include "rs485_int.h"
#include "crc.h"

RH_RF95 rf95(RFM95_SS, RFM95_DIO0);  
RHMesh mesh(rf95, MESH_ADDRESS); 

void setup(void)
{
    Serial.begin(115200);
    Serial2.begin(RS485_BAUD, SERIAL_8N1, RS485_RX, RS485_TX);
    pinMode(RS485_RTS, OUTPUT);
    digitalWrite(RS485_RTS, LOW); // SET PIN NORMALLY LOW
    while (!Serial) 
    {
        // wait for serial
    }

    // SPI pins setup for LoRa
    SPI.begin(RFM95_SCK, RFM95_MISO, RFM95_MOSI, RFM95_SS); 

    // Initialize LoRa
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

    // Reset LoRa
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    while (!mesh.init()) {
        printf("[%s] LoRa init failed\n", ID);
        delay(1000);
    }

    if (!rf95.setFrequency(RF95_FREQ)) {
        printf("[%s] Frequency set failed\n", ID);
        while (1);
    }

    rf95.setTxPower(23, false);
    rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);

    printf("[%s] init serial console\n", ID);
}

void loop(void) {
    static data rs485_data_struct;
    data* rs485_data = &rs485_data_struct;

    if (readSensor(rs485_data)) {
        // Send the raw data array to bridge
        // Note: Using address 1 for bridge, you might want to define this in config.h
        if (mesh.sendtoWait((uint8_t*)rs485_data, sizeof(data), 0x00000001)) {
            // Wait for acknowledgment
            uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t len = sizeof(buf);
            uint8_t from;
            
            if (mesh.recvfromAckTimeout(buf, &len, 2000, &from)) {
                printf("[%s] Got acknowledgment from bridge\n", ID);
            } else {
                printf("[%s] No acknowledgment from bridge\n", ID);
            }
        } else {
            printf("[%s] Send to bridge failed\n", ID);
        }

        // Print locally
        printf("Humidity: %.1f\n", rs485_data->humidity);
        printf("Temperature: %.1f\n", rs485_data->temperature);
        printf("Conductivity: %d\n", rs485_data->conductivity);
        printf("pH: %.1f\n", rs485_data->ph);
        printf("Nitrogen: %d\n", rs485_data->nitrogen);
        printf("Phosphorus: %d\n", rs485_data->phosphorus);
        printf("Potassium: %d\n", rs485_data->potassium);
    } else {
        printf("Failed to read sensor\n");
    }
    
    delay(60000); // Wait 1 minute before next reading
}