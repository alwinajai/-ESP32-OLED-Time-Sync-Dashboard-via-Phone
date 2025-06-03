#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WebServer.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wi-Fi Access Point credentials
const char* ssid = "ESP32_HOTSPOT";
const char* password = "esp32password";

// Web server for handling phone input
WebServer server(80);  // HTTP server on port 80

// Time variables
int currentHour = 0;
int currentMinute = 0;
int currentSecond = 0;
bool timeReceived = false;  // Flag to check if the time was received from the phone

// HTML page to be served to phone
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head><title>Send Time</title></head>
  <body style="text-align:center; font-family:sans-serif;">
    <h2>Send Phone Time to ESP32</h2>
    <button onclick="sendTime()">Send Time</button>
    <script>
      function sendTime() {
        let now = new Date();
        let h = now.getHours().toString().padStart(2,'0');
        let m = now.getMinutes().toString().padStart(2,'0');
        let s = now.getSeconds().toString().padStart(2,'0');
        let timeStr = h + ':' + m + ':' + s;
        fetch("/setTime?value=" + timeStr)
          .then(() => alert("Time Sent: " + timeStr));
      }
    </script>
  </body>
</html>
)rawliteral";

// Wi-Fi Icon (16x16)
const unsigned char wifi_logo [] PROGMEM = {
  0x00, 0x00, 0x03, 0xC0, 0x0F, 0xF0, 0x1C, 0x38,
  0x30, 0x0C, 0x67, 0xE6, 0x4F, 0xF2, 0x0C, 0x30,
  0x0C, 0x30, 0x18, 0x18, 0x13, 0xC8, 0x10, 0x08,
  0x00, 0x00, 0x06, 0x60, 0x0F, 0xF0, 0x0F, 0xF0
};

// Handle root: serve the webpage
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Handle receiving time from phone
void handleSetTime() {
  if (server.hasArg("value")) {
    String receivedTime = server.arg("value");
    // Parse received time string into hour, minute, and second
    currentHour = receivedTime.substring(0, 2).toInt();
    currentMinute = receivedTime.substring(3, 5).toInt();
    currentSecond = receivedTime.substring(6, 8).toInt();
    timeReceived = true;  // Mark time as received
    Serial.println("Phone Time Received: " + receivedTime);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // GPIO21 = SDA, GPIO22 = SCL

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Start Access Point
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  // Draw static screen layout
  drawScreenStatic(myIP);

  // Web server routes
  server.on("/", handleRoot);
  server.on("/setTime", handleSetTime);
  server.begin();
}

void loop() {
  server.handleClient();

  // Update the time every second once it has been received
  if (timeReceived) {
    // Increment the second
    currentSecond++;

    // Handle overflow (60 seconds -> 1 minute)
    if (currentSecond >= 60) {
      currentSecond = 0;
      currentMinute++;
    }

    // Handle overflow (60 minutes -> 1 hour)
    if (currentMinute >= 60) {
      currentMinute = 0;
      currentHour++;
    }

    // Handle 24-hour overflow (24 hours -> 0 hour)
    if (currentHour >= 24) {
      currentHour = 0;
    }

    // Clear the time area and display the new time
    display.fillRect(1, 46, 126, 18, SSD1306_BLACK); // Adjusted height for time display
    display.setTextSize(2); // Increased text size for better visibility
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 50);  // Adjusted position for centered time
    display.print(padZero(currentHour) + ":" + padZero(currentMinute) + ":" + padZero(currentSecond));
    display.display();

    delay(1000);  // Update time every second
  }
}

// Function to pad single-digit numbers with a leading zero
String padZero(int number) {
  if (number < 10) {
    return "0" + String(number);
  } else {
    return String(number);
  }
}

void drawScreenStatic(IPAddress ip) {
  display.clearDisplay();

  // Top banner
  display.fillRect(0, 0, 128, 14, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(10, 2);
  display.setTextSize(1);
  display.print("ESP32 OLED DASHBOARD");

  // Wi-Fi Icon + signal bars
  display.drawBitmap(5, 18, wifi_logo, 16, 16, SSD1306_WHITE);
  display.fillRect(25, 22, 3, 4, SSD1306_WHITE);
  display.fillRect(30, 20, 3, 6, SSD1306_WHITE);
  display.fillRect(35, 18, 3, 8, SSD1306_WHITE);

  // Rounded box around IP
  display.drawRoundRect(50, 18, 70, 14, 4, SSD1306_WHITE);
  display.setCursor(55, 20);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.print(ip);

  // Time display area
  display.setCursor(55, 38);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.print("Time");

  display.display();
}
