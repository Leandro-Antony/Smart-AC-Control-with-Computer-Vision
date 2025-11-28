#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
#include <IRsend.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Pinos para LED RGB
int red = 25;
int green = 26;
int blue = 27;

uint64_t codes[10] = {};

int temp_atual = 0;

int length = sizeof(codes) / sizeof(codes[0]);

#ifdef ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 10;
#else
const uint16_t kRecvPin = 14;
#endif

const uint16_t kIrLedPin = 2;
IRsend irsend(kIrLedPin);


const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;

#if DECODE_AC
const uint8_t kTimeout = 50;
#else
const uint8_t kTimeout = 15;
#endif

const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;

#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;

// Último valor recebido para comparar repetições
uint64_t lastValue = 0;

// Função: verifica se número de bits é múltiplo de 8 ou é LG2 de 28 bits
bool isBitsMultipleOf8(decode_results* r) {
  return (r->bits % 8 == 0) || (r->bits == 28);
}

void setup() {
  Serial.begin(9600);

  while (!Serial) delay(50);

  assert(irutils::lowLevelSanityCheck() == 0);

  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);

#if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif
  irrecv.setTolerance(kTolerancePercentage);
  irrecv.enableIRIn();

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(32, OUTPUT_OPEN_DRAIN);
  digitalWrite(32, HIGH);

  irsend.begin();
  dht.begin();

  for (int i = 0; i < length; i++) {
    while (true) {
      yield();
      vTaskDelay(1);
      roxo();
      if (irrecv.decode(&results)) {

        // aqui ignora pacotes cujo tamanho não é múltiplo de 8 (exceto LG2 28 bits)
        if (!isBitsMultipleOf8(&results)) {
          vermelho();
          delay(1000);
          irrecv.resume();
        } else {
          // mostra timestamp
          uint32_t now = millis();


          String description = IRAcUtils::resultAcToString(&results);
          if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);

          // Protocolos de AC com checksum embutido
          if (hasACState(results.decode_type)) {
            for (uint16_t j = 0; j < results.bits / 8; j++) {
              if (j > 0) Serial.print(" ");
              Serial.printf("%02X", results.state[j]);
            }
            verde();
            delay(1000);
            codes[i] = results.value;
            break;

            // Protocolo LG2 com 28 bits
          } else if (results.decode_type == LG2 && results.bits == 28) {
            verde();
            delay(1000);
            lastValue = results.value;

            codes[i] = results.value;
            break;

            // Protocolos simples (verificação por repetição)
          } else {
            if (results.value == lastValue && results.bits > 0) {
              verde();
              delay(1000);

              codes[i] = results.value;
              break;

            } else {
              vermelho();
              delay(1000);
            }
            lastValue = results.value;
          }

#if LEGACY_TIMING_INFO
#endif


          irrecv.resume();
        }
      }
    }
  }

  digitalWrite(32, LOW);
  delay(500);
  digitalWrite(32, HIGH);

}

void loop() {
  branco();

  float t = dht.readTemperature();

  int qtd = Serial.parseInt();

  if (isnan(t)) {
    vermelho();
    delay(500);
    off();
    delay(500);
  } else {
    branco();
    Serial.print(t);
    Serial.println("*C");

    if (Serial.available()) {
      Serial.print("Recebido: ");
      Serial.println(qtd);
    }

    if (qtd > 0 && temp_atual != 0) {
      if (t > 23 && temp_atual != 18) {
        temp18();
      } else if (t < 21 && temp_atual != 25) {
        temp25();
      }
    } else if (qtd > 0 && temp_atual == 0) {
      ligar();
    } else if (qtd == 0 && temp_atual != 0) {
      desligar();
    }

    delay(3000);
  }
}


void desligar() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[0]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[0]);
  }
  Serial.println("OFF");
  temp_atual = 0;
  vermelho();
}

void ligar() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[1]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[1]);
  }
  Serial.println("ON");
  temp_atual = 18;
  verde();
}

void temp18() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[2]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[2]);
  }
  Serial.println("18*C");
  temp_atual = 18;
}

void temp19() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[3]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[3]);
  }
  Serial.println("19*C");
  temp_atual = 19;
}

void temp20() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[4]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[4]);
  }
  Serial.println("20*C");
  temp_atual = 20;
}

void temp21() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[5]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[5]);
  }
  Serial.println("21*C");
  temp_atual = 21;
}

void temp22() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[6]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[6]);
  }
  Serial.println("22*C");
  temp_atual = 22;
}

void temp23() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[7]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[7]);
  }
  Serial.println("23*C");
  temp_atual = 23;
}

void temp24() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[8]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[8]);
  }
  Serial.println("24*C");
  temp_atual = 24;
}

void temp25() {
  if (results.decode_type == LG2 && results.bits == 28) {
    irsend.sendLG2(codes[9]);
  } else if (results.decode_type == GREE) {
    irsend.sendGree(codes[9]);
  }
  Serial.println("25*C");
  temp_atual = 25;
}

void off() {
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}
void branco() {
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
}

void vermelho() {
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}

void verde() {
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
  digitalWrite(blue, LOW);
}

void roxo() {
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
  digitalWrite(blue, HIGH);
}
