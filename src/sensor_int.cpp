#include <Arduino.h>
#include "sensor_int.h"
#include "config.h"

#define HUMIDITY_L 4
#define HUMIDITY_H 5
#define TEMPERATURE_L 6
#define TEMPERATURE_H 7
#define CONDUCTIVITY_L 8
#define CONDUCTIVITY_H 9
#define PH_L 10
#define PH_H 11
#define NITROGEN_L 12
#define NITROGEN_H 13
#define PHOSPHORUS_L 14
#define PHOSPHORUS_H 15
#define POTASSIUM_L 16
#define POTASSIUM_H 17

#define RS485_POLL_LEN 7

int is_rs485_alive = 0;


/* RS485 Modbus RTU Frame*/
byte read_data_msg[] = {
    0x01,         // Address
    0x03,    // Function Code
    0x00, // Start Address (Hi)
    0x00, // Start Address (Lo)
    0x00, // Number of Points (Hi)
    0x07, // Number of Points (Lo)
    0x04,        // Error Check (Lo)
    0x08         // Error Check (Hi)
};
// for further info see -> docs/rs485_comms_datasheet.pdf or README.md


byte poll_rs485_int[] = {
    0xFF,         // Address
    0x03,    // Function Code
    0x07, // Start Address (Hi)
    0xD0, // Start Address (Lo)
    0x00, // Number of Points (Hi)
    0x01, // Number of Points (Lo)
    0x91,        // Error Check (Lo)
    0x59         // Error Check (Hi)
};


void rs485_poll(void *parameter)
{
    int bytes_recv;
    unsigned long start_time;
    uint8_t poll_result[7];

    while (1)
    {
        bytes_recv = 0;
        start_time = millis();

        digitalWrite(RS485_RTS, HIGH); //open comms
        delayMicroseconds(1000);

        while (Serial2.available())
        {
            Serial2.read(); // Clear buffer
        }

        Serial2.write(poll_rs485_int, 8);
        Serial2.flush();

        digitalWrite(RS485_RTS, LOW); //close comms
        delayMicroseconds(1000);

        while (((millis() - start_time) < 1000) && (bytes_recv < sizeof(poll_result)))
        {
            if (Serial2.available())
            {
                poll_result[bytes_recv] = Serial2.read();
                bytes_recv++;
            }
        }

        if (bytes_recv == sizeof(poll_result))
        {
            //Set is_rs485_alive to true/0
            is_rs485_alive = 1;
        }
    }
}


void read_sensor(uint8_t lora_data_rx[])
{  
    uint8_t rs485_data[RS485_DATA_LEN];
    int bytes_recv = 0;
    unsigned long start_time = millis();


    while (Serial2.available())
    {
        Serial2.read(); // Clear buffer
    }


    Serial2.write(poll_rs485_int, READ_DATA_LEN);
    Serial2.flush();
    
    digitalWrite(RS485_RTS, LOW);
    delayMicroseconds(1000);
    
    while (((millis() - start_time) < 1000) && (bytes_recv < READ_DATA_LEN))
    {
        if (Serial2.available())
        {
            rs485_data[bytes_recv] = Serial2.read();
            bytes_recv++;
        }
    }

    // process
    if (bytes_recv == RS485_DATA_LEN)
    {
        process_rs485_msg(rs485_data, lora_data_rx);
        return;
    }
}


/*
int process_rs485_msg(int rs485_data[RS485_DATA_LEN], data * rs485_data)

reads humidity and ph from buffer, buffer holds data from rs485 read.
prepare data to send as LoRa packet.
*/
void process_rs485_msg(uint8_t rs485_data[], uint8_t lora_data_rx[])
{
    printf("[%s] processing rs485_data\n", NODE_ID);

    // Parse the raw data from rs485_data
    int raw_humidity = (rs485_data[HUMIDITY_L] << 8) | (rs485_data[HUMIDITY_H]);
    int raw_temperature = (rs485_data[TEMPERATURE_L] << 8) | (rs485_data[TEMPERATURE_H]);
    int raw_conductivity = (rs485_data[CONDUCTIVITY_L] << 8) | (rs485_data[CONDUCTIVITY_H]);
    int raw_ph = (rs485_data[PH_L] << 8) | (rs485_data[PH_H]);
    int raw_nitrogen = (rs485_data[NITROGEN_L] << 8) | (rs485_data[NITROGEN_H]);
    int raw_phosphorus = (rs485_data[PHOSPHORUS_L] << 8) | (rs485_data[PHOSPHORUS_H]);
    int raw_potassium = (rs485_data[POTASSIUM_L] << 8) | (rs485_data[POTASSIUM_H]);

    // Print out the raw sensor data
    printf("Raw Data - Humidity: %d, Temperature: %d, Conductivity: %d, PH: %d, Nitrogen: %d, Phosphorus: %d, Potassium: %d\n",
           raw_humidity, raw_temperature, raw_conductivity, raw_ph, raw_nitrogen, raw_phosphorus, raw_potassium);

    // Scale the numerical data accordingly
    raw_humidity = raw_humidity * 0.1; // Scale by 0.1
    raw_temperature = raw_temperature * 0.1;
    raw_ph = raw_ph * 0.1;

    // Write the data to the 7 bytes in the lora data that correspond to the values
    lora_data_rx[2 * ADDRESS_SIZE + 0] = raw_humidity;
    lora_data_rx[2 * ADDRESS_SIZE + 1] = raw_temperature;
    lora_data_rx[2 * ADDRESS_SIZE + 2] = raw_conductivity;
    lora_data_rx[2 * ADDRESS_SIZE + 3] = raw_ph;
    lora_data_rx[2 * ADDRESS_SIZE + 4] = raw_nitrogen;
    lora_data_rx[2 * ADDRESS_SIZE + 5] = raw_phosphorus;
    lora_data_rx[2 * ADDRESS_SIZE + 6] = raw_potassium;

    // Print out the final values being stored into the LoRa packet
    printf("Final Data to send: Humidity: %d, Temperature: %d, Conductivity: %d, PH: %d, Nitrogen: %d, Phosphorus: %d, Potassium: %d\n",
           lora_data_rx[2 * ADDRESS_SIZE + 0], lora_data_rx[2 * ADDRESS_SIZE + 1], lora_data_rx[2 * ADDRESS_SIZE + 2],
           lora_data_rx[2 * ADDRESS_SIZE + 3], lora_data_rx[2 * ADDRESS_SIZE + 4], lora_data_rx[2 * ADDRESS_SIZE + 5], lora_data_rx[2 * ADDRESS_SIZE + 6]);
}

void init_rs485()
{
    Serial2.begin(RS485_BAUD, SERIAL_8N1, RS485_RX, RS485_TX);
    pinMode(RS485_RTS, OUTPUT);
    digitalWrite(RS485_RTS, LOW); // SET PIN NORMALLY LOW
}