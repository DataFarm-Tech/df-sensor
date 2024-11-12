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
u_int8_t read_data_msg[] = {
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
int process_rs485_msg(u_int8_t rec_data[], data * rs485_data)
{
    printf("[%s] processing rs485_data\n", ID);

    if (!validate_crc(rec_data))
    {
        printf("[%s] invalid rs485_data message\n", ID);
        return 0;
    }


    int raw_humidity = (rec_data[3] << 8) | (rec_data[4]);
    int raw_temperature = (rec_data[5] << 8) | (rec_data[6]);
    int raw_conductivity = (rec_data[7] << 8) | (rec_data[8]);
    int raw_ph = (rec_data[9] << 8) | (rec_data[10]);
    int raw_nitrogen = (rec_data[11] << 8) | (rec_data[12]);
    int raw_phosphorus = (rec_data[13] << 8) | (rec_data[14]);
    int raw_potassium = (rec_data[15] << 8) | (rec_data[16]);

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


    return 1;
}