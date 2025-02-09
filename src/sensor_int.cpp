#include <Arduino.h>
#include "sensor_int.h"
#include "config.h"
/*
RS485 Read Message Definition
*/


uint8_t rec_data_buffer[REC_DATA_LEN];
/*
i

0  -> 0x01 (Address)
1  -> 0x03 (Function Code)
2  -> 0x0E (Number of Byte)
3  -> 0x01 (Humidity) L
4  -> 0xD0 (Humidity) H
5  -> 0x01 (Temp) L
6  -> 0x4C (Temp) H
7  -> 0x00 (Conductivity) L
8  -> 0x2C (Conductivity) H
9  -> 0x00 (PH) L
10 -> 0x5A (PH) H
11 -> 0x00 (N) L
12 -> 0x20 (N) H
13 -> 0x00 (P) L
14 -> 0x58 (P) H
15 -> 0x00 (K) L
16 -> 0x68 (K) H
17 -> 0x70 (CRC) L
18 -> 0x29 (CRC) H
*/

/* RS485 Modbus RTU Frame*/
byte read_data_msg[] = {
    0x01, // Address
    0x03, // Function Code
    0x00, // Start Address (Hi)
    0x00, // Start Address (Lo)
    0x00, // Number of Points (Hi)
    0x07, // Number of Points (Lo)
    0x04, // Error Check (Lo)
    0x08  // Error Check (Hi)
};
// for further info see -> docs/rs485_comms_datasheet.pdf

/*
int process_rs485_msg(int rec_data[REC_DATA_LEN], data * rs485_data)

reads humidity and ph from buffer, buffer holds data from rs485 read.
prepare data to send as LoRa packet.
*/
void process_rs485_msg(uint8_t rec_data[REC_DATA_LEN], uint8_t des_buf[13])
{
    printf("[%s] processing rs485_data\n", NODE_ID);

    char *nodeId = NODE_ID;

    // Parse the raw data from rec_data
    int raw_humidity = (rec_data[4] << 8) | (rec_data[5]);
    int raw_temperature = (rec_data[6] << 8) | (rec_data[7]);
    int raw_conductivity = (rec_data[8] << 8) | (rec_data[9]);
    int raw_ph = (rec_data[10] << 8) | (rec_data[11]);
    int raw_nitrogen = (rec_data[12] << 8) | (rec_data[13]);
    int raw_phosphorus = (rec_data[14] << 8) | (rec_data[15]);
    int raw_potassium = (rec_data[16] << 8) | (rec_data[17]);

    // Scale the numerical data accordingly
    raw_humidity = raw_humidity * 0.1; // Scale by 0.1
    raw_temperature = raw_temperature * 0.1;
    raw_ph = raw_ph * 0.1;

    // What does des_buf actually do? ~Mohnish
    // Store the numerical data into des_buf
    des_buf[0] = raw_humidity;     // Humidity
    des_buf[1] = raw_temperature;  // Temperature
    des_buf[2] = raw_conductivity; // Conductivity
    des_buf[3] = raw_ph;           // pH
    des_buf[4] = raw_nitrogen;     // Nitrogen
    des_buf[5] = raw_phosphorus;   // Phosphorus
    des_buf[6] = raw_potassium;    // Potassium

    int start_pos = 7;

    for (int i = 0; i < ADDRESS_SIZE; i++)
    {
        if (start_pos + i < 13)
        {
            des_buf[start_pos + i] = (uint8_t)nodeId[i];
        }
        else
        {
            break;
        }
    }

    // Print out the full des_buf
    printf("Full des_buf content: ");
    for (int i = 0; i < 13; i++)
    {
        // Print each byte in hexadecimal format
        printf("0x%02X ", des_buf[i]);
    }
}

bool read_sensor(uint8_t rs485_data[])
{
    while (Serial2.available())
    {
        Serial2.read(); // Clear buffer
    }
    digitalWrite(RS485_RTS, HIGH);
    delayMicroseconds(1000);

    Serial2.write(read_data_msg, READ_DATA_LEN);
    Serial2.flush();

    digitalWrite(RS485_RTS, LOW);

    int bytesRead = 0;
    unsigned long startTime = millis();

    while ((millis() - startTime) < 1000 && bytesRead < REC_DATA_LEN)
    {
        if (Serial2.available())
        {
            rec_data_buffer[bytesRead] = Serial2.read();
            bytesRead++;
        }
    }

    if (bytesRead == REC_DATA_LEN)
    {
        process_rs485_msg(rec_data_buffer, rs485_data);
        return true;
    }

    return false;
}

void init_rs485()
{
    Serial2.begin(RS485_BAUD, SERIAL_8N1, RS485_RX, RS485_TX);
    pinMode(RS485_RTS, OUTPUT);
    digitalWrite(RS485_RTS, LOW); // SET PIN NORMALLY LOW
}