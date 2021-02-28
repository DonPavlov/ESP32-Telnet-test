#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Wifi.h>

#include "TelnetSpy.cpp"
#include "secrets.h"

#define SERIAL SerialAndTelnet

TelnetSpy SerialAndTelnet;

IPAddress local_IP(192, 168, 100, 124);
// Set your Gateway IP address
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);    // optional
IPAddress secondaryDNS(8, 8, 4, 4);  // optional

void waitForConnection();
void waitForDisconnection();
void telnetConnected();
void telnetDisconnected();

void waitForConnection() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    SERIAL.print(".");
  }
  SERIAL.println(" Connected!");
}

void waitForDisconnection() {
  while (WiFi.status() == WL_CONNECTED) {
    delay(500);
    SERIAL.print(".");
  }
  SERIAL.println(" Disconnected!");
}

void telnetConnected() { SERIAL.println("Telnet connection established."); }

void telnetDisconnected() { SERIAL.println("Telnet connection closed."); }

void setup() {
  SerialAndTelnet.setWelcomeMsg("Welcome to the TelnetSpy example\n\n");
  SerialAndTelnet.setCallbackOnConnect(telnetConnected);
  SerialAndTelnet.setCallbackOnDisconnect(telnetDisconnected);
  SERIAL.begin(115200);
  delay(100);  // Wait for serial port
  SERIAL.setDebugOutput(false);
  SERIAL.print("\n\nConnecting to WiFi ");
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  waitForConnection();

  SERIAL.println("Ready");
  SERIAL.print("IP address: ");
  SERIAL.println(WiFi.localIP());

  SERIAL.println(
      "\nType 'C' for WiFi connect.\nType 'D' for WiFi disconnect.\nType 'R' "
      "for WiFi reconnect.");
  SERIAL.println("All other chars will be echoed. Play around...\n");
  SERIAL.println(
      "\nType 'C' for WiFi connect.\nType 'D' for WiFi disconnect.\nType 'R' "
      "for WiFi reconnect.");
  SERIAL.println("All other chars will be echoed. Play around...\n");
}

void loop() {
  SerialAndTelnet.handle();
  if (SERIAL.available() > 0) {
    char c = SERIAL.read();
    switch (c) {
      case '\r':
        SERIAL.println();
        break;
      case 'C':
        SERIAL.print("\nConnecting ");
        WiFi.begin(ssid, password);
        waitForConnection();
        break;
      case 'D':
        SERIAL.print("\nDisconnecting ...");
        WiFi.disconnect();
        waitForDisconnection();
        break;
      case 'R':
        SERIAL.print("\nReconnecting ");
        WiFi.reconnect();
        waitForDisconnection();
        waitForConnection();
        break;
      default:
        SERIAL.print(c);
        break;
    }
  }
}