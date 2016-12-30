#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <SPI.h>
#include <Wire.h>
#include <stdio.h>

#include <ArduinoSTL.h>
#include <math.h>
#include <VirtualWire.h>

// Übertragungszeug
const int led_pin = 13;
const int transmit_pin = 12;
const int receive_pin = 2;
const int transmit_en_pin = 3;

// Berechnungszeug
std::vector<double> adrian;


void setup() {
  Serial.begin(115200);
 vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(1000);       // Bits per sec
  pinMode(led_pin, OUTPUT);
}
double Thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
  double voltage;
  int sensorValue = analogRead(A0);
  voltage = sensorValue * (5.15 / 1023.00) * (100+10)/10;
  return voltage;
}
void loop() {

adrian.push_back(Thermister(analogRead(0)));



  if ( adrian.size() > 10) {
    adrian.erase(adrian.begin());
  }

  // sum up all values in adrian
  double sum = 0;
  for (int i = 0; i < adrian.size(); i++) {
    sum += adrian[i];
  }
  char msg[6]; // string which will contain the number
  char helper[6];

  /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
  dtostrf(sum / adrian.size(), 4, 2, helper);
  sprintf(msg, "%s", helper);
  

  Serial.println(msg  );



  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, 5);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(10000);
}

