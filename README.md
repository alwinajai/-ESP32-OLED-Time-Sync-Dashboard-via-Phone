# -ESP32-OLED-Time-Sync-Dashboard-via-Phone
 ESP32 OLED Time Sync Dashboard via Phone
# ESP32 OLED Dashboard with Phone Time Sync

## Overview

This project creates a standalone **ESP32-based Wi-Fi Access Point** that serves a web page to sync the **phone's current time** with the ESP32. The synchronized time is displayed on a **128x64 SSD1306 OLED screen** along with the device IP and Wi-Fi signal icon. The system is ideal for offline environments and demonstrates real-time interaction between a smartphone and an ESP32 using HTTP requests.

## Features

- Sets up ESP32 as a local Wi-Fi hotspot (Access Point)
- Hosts a lightweight HTML web interface for phones
- Captures the current phone time with a single tap
- Parses and displays time on an SSD1306 OLED display
- Displays Wi-Fi signal icon and IP address
- Maintains and updates time every second once received

## Hardware Used

- **ESP32 Dev Board**
- **0.96" SSD1306 OLED Display (128x64, I2C)**
- Jumper wires, breadboard (if prototyping)

## Circuit Connections

| OLED Pin | ESP32 Pin |
|----------|-----------|
| VCC      | 3.3V      |
| GND      | GND       |
| SDA      | GPIO21    |
| SCL      | GPIO22    |

> *Note: Adjust the `Wire.begin(SDA, SCL)` pins in code if using different GPIOs.*

## Installation & Setup

1. **Prerequisites:**
   - Arduino IDE with ESP32 board support
   - Install libraries:
     - `Adafruit SSD1306`
     - `Adafruit GFX`
     - `WebServer` (included with ESP32 core)

2. **Upload the Code:**
   - Connect ESP32 to PC
   - Open the `.ino` file
   - Select correct board and COM port
   - Upload to the ESP32

3. **Access the Dashboard:**
   - On your phone or PC, connect to Wi-Fi:  
     **SSID**: `ESP32_HOTSPOT`  
     **Password**: `esp32password`
   - Open browser and visit: `http://192.168.4.1`
   - Tap **Send Time** to sync your device's time with the ESP32

## Output Preview

- OLED shows:
  - Top banner: "ESP32 OLED DASHBOARD"
  - Wi-Fi signal icon
  - IP address in rounded box
  - Time display updated every second

## Screenshots

*(Add images of OLED display and web interface here)*

## Project Structure

- `info.ino`: Main Arduino sketch
- Embedded HTML page for browser interface
- Custom 16x16 Wi-Fi bitmap icon
- OLED display handled using Adafruit libraries

## Possible Improvements

- Add RTC (Real-Time Clock) module for backup timekeeping
- Sync date in addition to time
- Enable editing of SSID/password from UI
- Add NTP support for automatic time sync

## License

MIT License

## Author

**ALWIN**  
MSc Digital Forensics and Information Security  
Email: your.email@example.com  
GitHub: [yourgithub](https://github.com/alwinajai/-ESP32-OLED-Time-Sync-Dashboard-via-Phone/upload/main)

