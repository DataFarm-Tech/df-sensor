#include <SPI.h>
#include <RHMesh.h>
#include <RH_RF95.h>

#define RH_MESH_MAX_MESSAGE_LEN 50

// in this small artificial network of 4 nodes
#define BRIDGE_ADDRESS 1
#define NODE_ADDRESS 3

// esp32 with rfm95w pin configuration
#define RFM95_CS 5     // gpio5
#define RFM95_RST 14   // gpio14
#define RFM95_INT 2    // gpio2

#define TXINTERVAL 3000
unsigned long nextTxTime;

RH_RF95 rf95(RFM95_CS, RFM95_INT);  // cs pin, int pin

RHMesh manager(rf95, NODE_ADDRESS);

void setup() {
  Serial.begin(115200);
  Serial.print(F("initializing node "));
  Serial.println(NODE_ADDRESS);

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
    Serial.println("done");
  }

  rf95.setTxPower(5);  // set transmitter power to 5dbm
  rf95.setFrequency(915.0);    // 915mhz
  rf95.setCADTimeout(500);

  Serial.println("rf95 ready");
  nextTxTime = millis();
}

uint8_t data[] = "hello world!";
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t res;

void loop() {
  if (millis() > nextTxTime) {
    nextTxTime += TXINTERVAL;
    Serial.print("sending to bridge n.");
    Serial.print(BRIDGE_ADDRESS);
    Serial.print(" res=");

    res = manager.sendtoWait(data, sizeof(data), BRIDGE_ADDRESS);
    Serial.println(res);

    if (res == RH_ROUTER_ERROR_NONE) {
      // data has been reliably delivered to the next node
      // handle as needed
    } else {
      Serial.println("sendtowait failed. are the bridge/intermediate mesh nodes running?");
    }
  }

  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from)) {
    Serial.print("message from node n.");
    Serial.print(from);
    Serial.print(": ");
    Serial.print((char*)buf);
    Serial.print(" rssi: ");
    Serial.println(rf95.lastRssi());
  }
}
