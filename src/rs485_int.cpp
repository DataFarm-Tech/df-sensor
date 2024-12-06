#include "config.h"
#include "rs485_int.h"
#include "crc.h"

/*
RS485 Read Message Definition
*/

u_int8_t rec_data_buffer[REC_DATA_LEN];
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
    0x01, //Address
    0x03, //Function Code
    0x00, //Start Address (Hi) 
    0x00, //Start Address (Lo)
    0x00, //Number of Points (Hi)
    0x07, //Number of Points (Lo)
    0x04, //Error Check (Lo)
    0x08  //Error Check (Hi)
};
//for further info see -> docs/rs485_comms_datasheet.pdf



/*
int process_rs485_msg(int rec_data[REC_DATA_LEN], data * rs485_data)

reads humidity and ph from buffer, buffer holds data from rs485 read.
prepare data to send as LoRa packet.
*/
bool process_rs485_msg(u_int8_t rec_data[REC_DATA_LEN], data * rs485_data)
{
    printf("[%s] processing rs485_data\n", ID);
    //  ///////////////////////////////////////////////////////
    //  the code below doesnt do anything, it needs to be fixed
    // ////////////////////////////////////////////////////////
    // if (!validate_crc(rec_data))
    // {
    //     printf("[%s] invalid rs485_data message\n", ID);
    //     return 0;
    // }

    int raw_humidity = (rec_data[4] << 8) | (rec_data[5]);
    int raw_temperature = (rec_data[6] << 8) | (rec_data[7]);
    int raw_conductivity = (rec_data[8] << 8) | (rec_data[9]);
    int raw_ph = (rec_data[10] << 8) | (rec_data[11]);
    int raw_nitrogen = (rec_data[12] << 8) | (rec_data[13]);
    int raw_phosphorus = (rec_data[14] << 8) | (rec_data[15]);
    int raw_potassium = (rec_data[16] << 8) | (rec_data[17]);

    rs485_data->humidity = raw_humidity * 0.1;
    rs485_data->temperature = raw_temperature * 0.1;
    rs485_data->conductivity = raw_conductivity;
    rs485_data->ph = raw_ph * 0.1;
    rs485_data->nitrogen = raw_nitrogen;
    rs485_data->phosphorus = raw_phosphorus;
    rs485_data->potassium = raw_potassium;


    // //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // i commented out below because its kinda redundant especially if we are doing a CRC check - lawind
    // //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // if (humidity > 1000)
    // {
    //     printf("[%s] warning: humidity calculated value above threshold.\n", ID);
    //     humidity = 1000;
    // }
    

    // humidity = humidity / 10;
    // ph = ph / 10;
    // //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // //////////////////////////////////////////////////////////////////////////////////////////////////////////


    return true;
}

bool readSensor(data* rs485_data, int maxRetries) {
    while (Serial2.available()) {
        Serial2.read(); // Clear buffer
    }

    digitalWrite(RS485_RTS, HIGH);
    delayMicroseconds(1000);
    
    Serial2.write(read_data_msg, READ_DATA_LEN);
    Serial2.flush();
    
    digitalWrite(RS485_RTS, LOW);

    int bytesRead = 0;
    unsigned long startTime = millis();
    
    while ((millis() - startTime) < 1000 && bytesRead < REC_DATA_LEN) {
        if (Serial2.available()) {
            rec_data_buffer[bytesRead] = Serial2.read();
            bytesRead++;
        }
    }

    if (bytesRead == REC_DATA_LEN) {
        return process_rs485_msg(rec_data_buffer, rs485_data);

    }
    
    return false;
}
