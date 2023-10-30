// Controllino defintions
#include <Controllino.h>

// CAN controller lib (https://github.com/autowp/arduino-mcp2515)
#include "mcp2515.h"

// Set bit rate for your CAN bus
#define BIT_RATE CAN_500KBPS

// NODE ID
#define NODE_ID 0x0A // Node-ID = 10

// Function code
#define FUNCTION_CODE 0x180 // CANopen PDO1_TX

// Setup MCP2515 lib with SPI chip-select pin
MCP2515 can(CONTROLLINO_PIN_HEADER_SS);

void setup() {
  Serial.begin(115200);

  // Important! Wont work without this line!
  Controllino_RTC_init();

  // Check if CAN controller is working
  if (can.reset() != MCP2515::ERROR_OK) {
    Serial.println("Reset error");
  }

  // Check and set bitrate
  if (can.setBitrate(BIT_RATE, MCP_8MHZ) != MCP2515::ERROR_OK) {
    Serial.println("Bitrate error");
  }

  // Check if normal mode can be enabled
  if (can.setNormalMode() != MCP2515::ERROR_OK) {
    Serial.println("Mode error");
  }
}

// Send a frame every 5000 ms (5 sec)
void loop() {
  can_frame frame{};
  frame.can_dlc = 8;
  frame.can_id = FUNCTION_CODE | NODE_ID; // Create COB-ID (`can_id`)
  if (can.sendMessage(&frame) == MCP2515::ERROR_OK) {
    Serial.println("TX");
  } else {
    Serial.println("ERR");
  }
  delay(5000);
}