#include "config.h"
#include "rs485_int.h"

/*
RS485 Read Message Definition
*/

u_int8_t read_data_msg[READ_DATA_LEN] = {
    0x01, //Address
    0x03, //Function Code
    0x00, //Start Address (Hi) 
    0x00, //Start Address (Lo)
    0x00, //Number of Points (Hi)
    0x07, //Number of Points (Lo)
    0x04, //Error Check (Lo)
    0x08  //Error Check (Hi)
};

int rec_data_buffer[REC_DATA_LEN];


/*
int process_rs485_msg(int rec_data[REC_DATA_LEN], data * rs485_data)

reads humidity and ph from buffer, buffer holds data from rs485 read.
prepare data to send as LoRa packet.
*/
int process_rs485_msg(int rec_data[REC_DATA_LEN], data * rs485_data)
{
    printf("[%s] processing rs485_data\n", ID);

    int humidity = (rec_data[3] << 8) | (rec_data[4]);
    int ph = (rec_data[9] << 8 ) | (rec_data[10]);

    if (humidity > 1000)
    {
        printf("[%s] warning: humidity calculated value above threshold.\n", ID);
        humidity = 1000;
    }

    humidity = humidity / 10;
    ph = ph / 10;

    rs485_data->humidity = humidity;
    rs485_data->ph = ph;

    return 0;
}