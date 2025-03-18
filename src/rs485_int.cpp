#include <Arduino.h>
#include "rs485_int.h"
#include "config.h"
#include "utils.h"

/**
 * @brief Converts two uint8_t's to a uint16_t
 * @param sensor The rs485 pins for a given sensor type
*/
#define READ_SENSOR_DATA(sensor) ((rs485_data[sensor##_L] << 8) | (rs485_data[sensor##_H]))

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

#define RS485_MSG_POLL_RES_LEN 7
#define RS485_MSG_READ_RES_LEN 19

#define NUM_DATA_READS 5

typedef struct rs485_data_ts
{
    uint8_t moisture;
    uint8_t temp;
    uint8_t con;
    uint8_t ph;
    uint8_t nit;
    uint8_t phos;
    uint8_t pot;
};

int is_rs485_alive = 0;
SemaphoreHandle_t rs485_mutex;


/* RS485 Modbus RTU Frame (Read NPK)*/
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

/* RS485 Modbus RTU Frame (Enquire SlaveID)*/
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
// for further info see -> docs/rs485_comms_datasheet.pdf or README.md

void process_rs485_msg(uint8_t rs485_data[], uint8_t lora_data_rx[]);
void open_rs485_port(void);
void close_rs485_port(void);


/**
 * @brief Open's the rs485 connection. By setting the GPIO pin to HIGH.
 */
void open_rs485_port(void)
{
    digitalWrite(RS485_RTS, HIGH); //open comms
    delayMicroseconds(1000);

    while (Serial2.available())
    {
        Serial2.read(); // Clear buffer
    }
}

/**
 * @brief Closes the rs485 connection. By setting the GPIO pin to LOW.
 */
void close_rs485_port(void)
{
    digitalWrite(RS485_RTS, LOW); //close comms
    delayMicroseconds(1000);
}

/**
 * @brief Sends a polling request over RS485 to check if the device is alive.
 * 
 * @param parameter Unused parameter for FreeRTOS task compatibility.
 */
void rs485_poll(void *parameter)
{
    int bytes_recv;
    unsigned long start_time;
    uint8_t poll_result[RS485_MSG_POLL_RES_LEN]; //TODO: change this to has define

    PRINT_STR("init rs485 poll thread");
    while (1)
    {
        bytes_recv = 0;
        
        if (xSemaphoreTake(rs485_mutex, portMAX_DELAY) == pdTRUE)
        {
            start_time = millis();
            
            open_rs485_port();
    
            Serial2.write(poll_rs485_int, sizeof(poll_rs485_int));
            Serial2.flush();
    
            close_rs485_port();
    
            while (((millis() - start_time) < 1000) && (bytes_recv < sizeof(poll_result)))
            {
                if (Serial2.available())
                {
                    poll_result[bytes_recv] = Serial2.read();
                    bytes_recv++;
                }
            }

            if (!compute_crc16(poll_result, bytes_recv))
            {
                PRINT_STR("crc invalid");
            }
            
            if (bytes_recv == sizeof(poll_result))
            {
                is_rs485_alive = 1;
            }
            
            memset(poll_result, 0, sizeof(poll_result)); //clear buffer
            xSemaphoreGive(rs485_mutex); // Release mutex
        }
        
        vTaskDelay(pdMS_TO_TICKS(60000)); // Prevent excessive polling
    }
}

/**
 * @brief Reads sensor data over RS485 and prepares it for LoRa transmission.
 * does 5 reads for each sensor type. Then calculates the median for each sensor type. 
 * calculate_median is used.
 * 
 * @param lora_data_rx Buffer to store processed sensor data for LoRa transmission.
 */
void read_sensor(uint8_t lora_data_rx[])
{
    int bytes_recv = 0;
    unsigned long start_time;
    rs485_data_ts data_field;
    uint8_t rs485_data[RS485_MSG_READ_RES_LEN];

    // 2D array for storing sensor data for each read (5 reads, NUM_SENSORS)
    uint8_t sensor_data[NUM_DATA_READS][7];  // 7 sensors (moisture, temp, con, ph, nit, phos, pot)

    if (xSemaphoreTake(rs485_mutex, portMAX_DELAY) == pdTRUE)
    {
        for (int i = 0; i < NUM_DATA_READS; i++)  // NUM_DATA_READS iterations
        {
            bytes_recv = 0;  // Reset bytes_recv for each read
            start_time = millis();

            open_rs485_port();

            // Send read request
            Serial2.write(read_data_msg, sizeof(read_data_msg));
            Serial2.flush();

            close_rs485_port();

            // Wait for data with a timeout of 1000ms
            while (((millis() - start_time) < 1000) && (bytes_recv < sizeof(rs485_data)))
            {
                if (Serial2.available())
                {
                    rs485_data[bytes_recv] = Serial2.read();
                    bytes_recv++;
                }
            }

            // Check if the received data is valid (CRC check)
            if ((!compute_crc16(rs485_data, bytes_recv)) && (bytes_recv == sizeof(rs485_data)))
            {
                PRINT_STR("CRC invalid");
                break;  // Exit the loop if CRC is invalid
            }

            // Store sensor data for the current read
            sensor_data[i][0] = READ_SENSOR_DATA(HUMIDITY);        
            sensor_data[i][1] = READ_SENSOR_DATA(TEMPERATURE);
            sensor_data[i][2] = READ_SENSOR_DATA(CONDUCTIVITY);
            sensor_data[i][3] = READ_SENSOR_DATA(PH);
            sensor_data[i][4] = READ_SENSOR_DATA(NITROGEN);
            sensor_data[i][5] = READ_SENSOR_DATA(PHOSPHORUS);
            sensor_data[i][6] = READ_SENSOR_DATA(POTASSIUM);

            sleep(2000);
        }

        // Calculate median for each sensor after collecting all 5 values
        data_field.moisture = calculate_median(&sensor_data[0][0]);
        data_field.temp = calculate_median(&sensor_data[0][1]);
        data_field.con = calculate_median(&sensor_data[0][2]);
        data_field.ph = calculate_median(&sensor_data[0][3]);
        data_field.nit = calculate_median(&sensor_data[0][4]);
        data_field.phos = calculate_median(&sensor_data[0][5]);
        data_field.pot = calculate_median(&sensor_data[0][6]);

        process_rs485_msg(data_field, lora_data_rx);

        xSemaphoreGive(rs485_mutex);
    }
}


/**
 * @brief Processes the received RS485 data and extracts sensor values.
 * 
 * @param rs485_data Buffer containing the raw RS485 data response.
 * @param lora_data_rx Buffer where processed sensor values will be stored for LoRa transmission.
 */
void process_rs485_msg(rs485_data_ts data_field, uint8_t lora_data_rx[])
{
    PRINT_STR("processing rs485 data");

    //data conversions
    data_field.moisture = data_field.moisture * 0.1;
    data_field.temp = data_field.temp * 0.1;
    data_field.ph = data_field.ph * 0.1;

    // Write the data to the 7 bytes in the lora data that correspond to the values
    lora_data_rx[2 * ADDRESS_SIZE + 0] = data_field.moisture;
    lora_data_rx[2 * ADDRESS_SIZE + 1] = data_field.temp;
    lora_data_rx[2 * ADDRESS_SIZE + 2] = data_field.con;
    lora_data_rx[2 * ADDRESS_SIZE + 3] = data_field.ph;
    lora_data_rx[2 * ADDRESS_SIZE + 4] = data_field.nit;
    lora_data_rx[2 * ADDRESS_SIZE + 5] = data_field.phos;
    lora_data_rx[2 * ADDRESS_SIZE + 6] = data_field.pot;

    // Print out the final values being stored into the LoRa packet
    printf("Final Data to send: Humidity: %d, Temperature: %d, Conductivity: %d, PH: %d, Nitrogen: %d, Phosphorus: %d, Potassium: %d\n",
           lora_data_rx[2 * ADDRESS_SIZE + 0], 
           lora_data_rx[2 * ADDRESS_SIZE + 1], 
           lora_data_rx[2 * ADDRESS_SIZE + 2],
           lora_data_rx[2 * ADDRESS_SIZE + 3], 
           lora_data_rx[2 * ADDRESS_SIZE + 4], 
           lora_data_rx[2 * ADDRESS_SIZE + 5], 
           lora_data_rx[2 * ADDRESS_SIZE + 6]);
}

/**
 * @brief Initializes the RS485 communication interface.
 */
void init_rs485()
{
    Serial2.begin(RS485_BAUD, SERIAL_8N1, RS485_RX, RS485_TX);
    pinMode(RS485_RTS, OUTPUT);
    digitalWrite(RS485_RTS, LOW); // SET PIN NORMALLY LOW
    rs485_mutex = xSemaphoreCreateMutex();
}