/*
 * LoRa Sender DEMO
 * board used: LILYGO® TTGO LoRa32 V2.1_1.6
 * Thanks to Digitspace for the boards
 * Code by Bernardo Giovanni (https://www.settorezero.com)
 * Read the full article here:
 * https://www.settorezero.com/wordpress/lilygo_ttgo_lora32_esp32_point_to_point
 * if you want to share the code please mention: Giovanni Bernardo of settorezero.com
 */

#include <WiFi.h>
#include <SD.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h> 
#include <SSD1306Wire.h>
#include "board_defs.h" // in this header I've defined the pins used by the module LILYGO® TTGO LoRa32 V2.1_1.6



String packet; // packet to be sent over LoRa
uint32_t counter=0; // a demo variable to be included in the packet
float battery=0; // battery voltage

SSD1306Wire display(OLED_ADDRESS, OLED_SDA, OLED_SCL);
SPIClass sdSPI(HSPI); // we'll use the SD card on the HSPI (SPI2) module
SPIClass loraSPI(VSPI); // we'll use the LoRa module on the VSPI (SPI3) module

void setup() 
  {
  // Begins some pins
  pinMode(LED,OUTPUT); // Green Led
  pinMode(VBAT,INPUT); // Input for Battery Voltage

  // Begins UART
  Serial.begin(115200);
  while (!Serial);
  delay(500);
  Serial.println();
  Serial.println("LoRa Sender Test");

  // Begins OLED
  display.init();
  // following setting will allow to read the display frontal keeping the antenna on top
  display.flipScreenVertically(); 
  display.clear();
  delay(100); 

  // Begins LoRa Module
  loraSPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DI0);
  LoRa.setSPI(loraSPI);
  while (!LoRa.begin(LORA_BAND)) 
    {
    Serial.println("LoRa failed");
    display.drawString(0,0,"LoRa failed");
    display.display();
    delay(15000);
    }
  Serial.println("LoRa OK");
  display.drawString(0,0,"LoRa OK");
  display.display();
  // set a sync word to be used with both modules for synchronization
  LoRa.setSyncWord(0xF3); // ranges from 0-0xFF, default 0x34, see API docs
  delay(1000);
  // check battery
  battery=checkBattery(); // first battery check at start-up
  Serial.print("Battery Voltage: ");
  Serial.print(battery);
  Serial.println("V");
  display.drawString(0,22,"Battery: "+String(battery)+"V");
  display.display();
  delay(1000);
  
  Serial.println("Init OK");
  display.drawString(0,33,"Init OK");
  display.display();
  delay(1000);
  display.clear();

  }

void loop() 
  {
  // variables used for checking the battery and give an average value
  static uint8_t chkbat=0;
  static float tempbatt=0;

  LedOn();
  display.clear();

  display.drawString(8,31, "LoRa SENDER DEMO");
  display.drawString(0,42, "Battery: "+String(battery)+"V");
  
  display.drawString(0, 52,"Packet: ");
  display.drawString(48,52, String(counter));
  Serial.println(String(counter));
  display.display();

  // send packet over LoRa
  packet="@"+String(counter)+"#";
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
  Serial.print("Packet: ");
  Serial.println(packet);
 
  counter++;
 
  delay(500);
  LedOff();
  delay(1000);
  }

void LedOn()
    {
    digitalWrite(LED, HIGH);
    }

void LedOff()
    {
    digitalWrite(LED, LOW);
    }
    float checkBattery(void)
  {
  // The ADC value is a 12-bit number (values from 0 to 4095)
  // To convert the ADC integer value to a real voltage you’ll need to divide it by the maximum value of 4095
  // then double it since there is an 1:2 voltage divider on the battery
  // then you must multiply that by the reference voltage of the ESP32 which is 3.3V 
  // and then multiply that again by the ADC Reference Voltage of 1100mV.
  return (float)(analogRead(VBAT))/4095*2*3.3*1.1;
  }
