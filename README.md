# 🛰️ Project Pulvis

**Repository for code used in Project Pulvis — a student-built satellite developed for the 2025/26 European CanSat Competition.**

## 🚀 Overview
Project Pulvis is a CanSat mission focused on environmental data collection, sensor integration, and autonomous telemetry transmission during descent.  
The payload is designed and programmed by students, following ESA CanSat competition guidelines.

## 🧩 Features
- Telemetry system for live transmission (LoRa / RF)
- Environmental sensors (temperature, pressure, humidity, acceleration)
- Data logging to SD card
- Onboard orientation and motion tracking
- Ground Station interface for data visualization

## 🔧 Hardware
- **Main MCU:** [e.g., ESP32 / Arduino Nano / STM32]  
- **Sensors:**
  - SHT31-F (temperature & humidity)
  - BMP280 (pressure)
  - ADXL345 (3-axis accelerometer)
- **Communication:** LoRa SX1278
- **Storage:** microSD module

## 🖥️ Software
- Language: **C++ / Arduino**
- Libraries: `Wire.h`, `Adafruit_Sensor`, `LoRa`, `SD`, `SPI`
- Build system: Arduino IDE / PlatformIO

## 📡 Data Format (example)