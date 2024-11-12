#include <Arduino.h>
#include <SPI.h>

#include "config.h"
#include "rs485_int.h"

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

    printf("[%s] init serial console\n", ID);
}

void loop(void) {
    static data rs485_data_struct;
    data* rs485_data = &rs485_data_struct;

    if (readSensor(rs485_data)) {
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
    
    delay(1000);
}