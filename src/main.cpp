#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#include "config.h"
#include "rs485_int.h"

RH_RF95 rf95(RFM95_NSS, RFM95_DIO0);

void setup(void)
{
    Serial.begin(115200); //start serial
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

    while (!Serial) 
    {
        // wait for serial
    }

    printf("[%s] init serial console\n", ID);
}

void loop(void) {
    static data rs485_data_struct;
    data* rs485_data = &rs485_data_struct;

    if (readSensor(rs485_data)) 
    {
        printf("Humidity: %.1f\n", rs485_data->humidity);
        printf("Temperature: %.1f\n", rs485_data->temperature);
        printf("Conductivity: %d\n", rs485_data->conductivity);
        printf("pH: %.1f\n", rs485_data->ph);
        printf("Nitrogen: %d\n", rs485_data->nitrogen);
        printf("Phosphorus: %d\n", rs485_data->phosphorus);
        printf("Potassium: %d\n", rs485_data->potassium);
    } 
    else 
    {
        printf("Failed to read sensor\n");
    }

    // Send the string data
    if (rf95.send(rec_data_buffer, sizeof(rec_data_buffer)))  // Use strlen to get the actual length of the string
    {
        printf("SUCCESS\n");
    }
    else 
    {
        printf("error\n");
    }
    
    
    delay(5000);
}