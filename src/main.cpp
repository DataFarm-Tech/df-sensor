#include <SPI.h>
#include <RHMesh.h>
#include <RH_RF95.h>

#define RH_MESH_MAX_MESSAGE_LEN 50

// Define your node address
#define MY_NODE_ADDRESS 1

// RFM95 pin configuration for ESP32
#define RFM95_CS 5    // GPIO5
#define RFM95_RST 14   // GPIO14
#define RFM95_INT 26   // GPIO26

RH_RF95 rf95(RFM95_CS, RFM95_INT);  // cs pin, int pin
RHMesh manager(rf95, MY_NODE_ADDRESS);

const int potPin = 34; // Analog pin for the potentiometer

void setup() {
  Serial.begin(115200);
  Serial.print("Mesh Node ");
  Serial.println(MY_NODE_ADDRESS);

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!manager.init()) {
    Serial.println("init failed");
  } else {
    Serial.println("Mesh network initialized");
  }

  rf95.setTxPower(5);  // set transmitter power to 14dbm
  rf95.setFrequency(915.0);    // 915MHz
  rf95.setCADTimeout(500);

  Serial.println("RF95 ready");
}

void loop() {

  if (millis() % 5000 == 0) { // every 5000 ms send data
    int potValue = analogRead(potPin); // read potentiometer value
    Serial.print("Sending potentiometer value to any node: ");
    Serial.println(potValue);

    uint8_t data[3];
    data[0] = 'P';
    data[1] = highByte(potValue);
    data[2] = lowByte(potValue);

    // send message to closest node
    manager.sendtoWait(data, sizeof(data), RH_BROADCAST_ADDRESS);
  }

  // check for incoming messages
  uint8_t len = RH_MESH_MAX_MESSAGE_LEN;
  uint8_t from;
  uint8_t buf[RH_MESH_MAX_MESSAGE_LEN]; // Buffer for incoming messages
  if (manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
    Serial.print("Message received from Node ");
    Serial.print(from);
    Serial.print(": ");
    int potValue = (buf[1] << 8) | buf[2];  
    Serial.print("Potentiometer value: ");
    Serial.println(potValue);
    Serial.println();
  }
}