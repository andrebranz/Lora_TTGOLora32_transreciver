# Lora_TTGOLora32_transreciver
This repository contains the scripts to send and reciv data using LoRa module on ESP32.
This scripts use no encryption.
This scripts allow end-to-end comunication using LoRa module on the hardware.


## Description
In both directory there is "board_def.h"; this file contains the data of pin connnection.

I've done this step because using <a href = "https://github.com/sandeepmistry/arduino-LoRa">this library</a> I had some problem calling LoRa.begin(band_standard).

## Useful Link
<a href = "https://lora-developers.semtech.com/documentation/tech-papers-and-guides/building-a-lora-based-device-end-to-end-with-arduino/"> Building a LoRa® Device End-to-End with Arduino </a>
