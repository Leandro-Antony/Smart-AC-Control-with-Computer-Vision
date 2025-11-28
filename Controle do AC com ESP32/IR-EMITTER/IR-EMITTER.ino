#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLedPin = 2;
IRsend irsend(kIrLedPin);


void setup() {
  Serial.begin(115200);
  irsend.begin();
}

void loop() {
  //REMEMBER TO USE YOUR PROTOCOL SENDER
  irsend.sendLG2(0x880834F, 28);  //18°C //PUT YOUR COMMANDS FROM IR-RECORDER IN THE PARAMETER
  Serial.println("18°C");

  delay(1000);

  irsend.sendLG2(0x8808844, 28);  //23°C
  Serial.println("23°C");

  delay(1000);
}
