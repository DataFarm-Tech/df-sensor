#include <Arduino.h>
#include "sensor_int.h"
#include "config.h"
/*
RS485 Read Message Definition
*/

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

#define HUMIDITY_L 3
#define HUMIDITY_H 4
#define TEMPERATURE_L 5
#define TEMPERATURE_H 6
#define CONDUCTIVITY_L 7
#define CONDUCTIVITY_H 8
#define PH_L 9
#define PH_H 10
#define NITROGEN_L 11
#define NITROGEN_H 12
#define PHOSPHORUS_L 13
#define PHOSPHORUS_H 14
#define POTASSIUM_L 15
#define POTASSIUM_H 16

#define ADDR 0x01
#define FUNC_CODE 0x03
#define START_ADDR_H 0x00
#define START_ADDR_L 0x00
#define NUM_POINTS_H 0x00
#define NUM_POINTS_L 0x07
#define CRC_L 0x04
#define CRC_H 0x08

/* RS485 Modbus RTU Frame*/
byte read_data_msg[] = {
    ADDR,         // Address
    FUNC_CODE,    // Function Code
    START_ADDR_H, // Start Address (Hi)
    START_ADDR_L, // Start Address (Lo)
    NUM_POINTS_H, // Number of Points (Hi)
    NUM_POINTS_L, // Number of Points (Lo)
    CRC_L,        // Error Check (Lo)
    CRC_H         // Error Check (Hi)
};
// for further info see -> docs/rs485_comms_datasheet.pdf

/*
int process_rs485_msg(int rs485_data[RS485_DATA_LEN], data * rs485_data)

reads humidity and ph from buffer, buffer holds data from rs485 read.
prepare data to send as LoRa packet.
*/
void process_rs485_msg(uint8_t rs485_data[], uint8_t lora_data_rx[])
{
    Serial.printf("[%s] Humidity: %d\n", NODE_ID, rs485_data[0]);
    Serial.printf("[%s] Temperature: %d\n", NODE_ID, rs485_data[1]);
    Serial.printf("[%s] Conductivity: %d\n", NODE_ID, rs485_data[2]);
    Serial.printf("[%s] pH: %d\n", NODE_ID, rs485_data[3]);
    Serial.printf("[%s] Nitrogen: %d\n", NODE_ID, rs485_data[4]);
    Serial.printf("[%s] Phosphorus: %d\n", NODE_ID, rs485_data[5]);
    Serial.printf("[%s] Potassium: %d\n", NODE_ID, rs485_data[6]);

    printf("[%s] processing rs485_data\n", NODE_ID);

    // Parse the raw data from rs485_data
    int raw_humidity = (rs485_data[4] << 8) | (rs485_data[5]);
    int raw_temperature = (rs485_data[6] << 8) | (rs485_data[7]);
    int raw_conductivity = (rs485_data[8] << 8) | (rs485_data[9]);
    int raw_ph = (rs485_data[10] << 8) | (rs485_data[11]);
    int raw_nitrogen = (rs485_data[12] << 8) | (rs485_data[13]);
    int raw_phosphorus = (rs485_data[14] << 8) | (rs485_data[15]);
    int raw_potassium = (rs485_data[16] << 8) | (rs485_data[17]);
    // int raw_humidity = (rs485_data[HUMIDITY_L] << 8) | (rs485_data[HUMIDITY_H]);
    // int raw_temperature = (rs485_data[TEMPERATURE_L] << 8) | (rs485_data[TEMPERATURE_H]);
    // int raw_conductivity = (rs485_data[CONDUCTIVITY_L] << 8) | (rs485_data[CONDUCTIVITY_H]);
    // int raw_ph = (rs485_data[PH_L] << 8) | (rs485_data[PH_H]);
    // int raw_nitrogen = (rs485_data[NITROGEN_L] << 8) | (rs485_data[NITROGEN_H]);
    // int raw_phosphorus = (rs485_data[PHOSPHORUS_L] << 8) | (rs485_data[PHOSPHORUS_H]);
    // int raw_potassium = (rs485_data[POTASSIUM_L] << 8) | (rs485_data[POTASSIUM_H]);

    // Scale the numerical data accordingly
    raw_humidity = raw_humidity * 0.1; // Scale by 0.1
    raw_temperature = raw_temperature * 0.1;
    raw_ph = raw_ph * 0.1;

    // write the data to the 7 bytes in the lora data that correspond to the values
    lora_data_rx[2 * ADDRESS_SIZE + 0] = raw_humidity;
    lora_data_rx[2 * ADDRESS_SIZE + 1] = raw_temperature;
    lora_data_rx[2 * ADDRESS_SIZE + 2] = raw_conductivity;
    lora_data_rx[2 * ADDRESS_SIZE + 3] = raw_ph;
    lora_data_rx[2 * ADDRESS_SIZE + 4] = raw_nitrogen;
    lora_data_rx[2 * ADDRESS_SIZE + 5] = raw_phosphorus;
    lora_data_rx[2 * ADDRESS_SIZE + 6] = raw_potassium;
}

bool read_sensor(uint8_t lora_data_rx[])
{
    uint8_t rs485_data[RS485_DATA_LEN];

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

    while ((millis() - startTime) < 1000 && bytesRead < RS485_DATA_LEN)
    {
        if (Serial2.available())
        {
            printf("Serial2.read()\n");
            rs485_data[bytesRead] = Serial2.read();
            bytesRead++;
        }
    }

    printf("Bytes read: %d\n", bytesRead);
    // process
    if (bytesRead == RS485_DATA_LEN)
    {
        process_rs485_msg(rs485_data, lora_data_rx);
        return true;
    }

    printf("Printing the values in read sensor\n");
    Serial.printf("[%s] Humidity: %d\n", NODE_ID, rs485_data[0]);
    Serial.printf("[%s] Temperature: %d\n", NODE_ID, rs485_data[1]);
    Serial.printf("[%s] Conductivity: %d\n", NODE_ID, rs485_data[2]);
    Serial.printf("[%s] pH: %d\n", NODE_ID, rs485_data[3]);
    Serial.printf("[%s] Nitrogen: %d\n", NODE_ID, rs485_data[4]);
    Serial.printf("[%s] Phosphorus: %d\n", NODE_ID, rs485_data[5]);
    Serial.printf("[%s] Potassium: %d\n", NODE_ID, rs485_data[6]);

    return false;
}

void init_rs485()
{
    Serial2.begin(RS485_BAUD, SERIAL_8N1, RS485_RX, RS485_TX);
    pinMode(RS485_RTS, OUTPUT);
    digitalWrite(RS485_RTS, LOW); // SET PIN NORMALLY LOW
}