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

void loop(void)
{
    int humidity;
    int ph;
    data * rs485_data;

    delay(60000);
    
    printf("[%s] switch to sending\n", ID);
    digitalWrite(RS485_RTS, HIGH); // Sending mode
    delay(100);


    Serial2.write(read_data_msg, READ_DATA_LEN); // send command
    Serial2.flush();

    printf("[%s] switch to recieving\n", ID);
    digitalWrite(RS485_RTS, LOW); // recieivng mode

    while (Serial2.available())
    {
        for (int i = 0; i < REC_DATA_LEN - 1; i++)
        {
            rec_data_buffer[i] = Serial2.read();
        }
    }

    if (process_rs485_msg(rec_data_buffer, rs485_data))
    {
        printf("Humidity: %d", rs485_data->humidity);
        printf("pH: %d", rs485_data->ph);
    }
    
    memset(rec_data_buffer, sizeof(rec_data_buffer), 0);
}