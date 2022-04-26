#include <ArduinoJson.h>
#include <ESP8266WiFi.h>              
#include <WiFiClient.h>
#include <TimeLib.h>
#include <Key.h>
#include <Keypad.h>

#define LED_BUILTIN 2
volatile unsigned long globalTimeBufferMillis = 0;

// WiFi, HTTP
const char* ssid = "Ttk 139";
const char* password = "631790355";

WiFiClient client;
IPAddress local(192, 168, 0, 14);
uint16_t port = 8080;

DynamicJsonDocument doc(2048);
unsigned long openerId;

String enteringPassword = "12345";

void setup() {

    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED)
        delay(500);

    Serial.begin(9600);
}

boolean checkPassword(String enteringPassword, String enteringType) {
    getPasswords();

    for (JsonObject item : doc.as<JsonArray>()) {
        openerId = item["openerId"]; 
        String type = item["type"]; 
        String value = item["value"]; 

        if (type == enteringType) {
            if (value ==  enteringPassword) {
                return true;
            }
        }
    }

    return false;
}

void loop() {
    if (Serial.available()) {

        String expression = Serial.readStringUntil('-');

        String method = expression.substring(0, expression.indexOf(':'));
        expression = expression.substring(expression.indexOf(':') + 1, expression.length());

        if (method == "check") {

            String type = expression.substring(0, expression.indexOf(':'));
            expression = expression.substring(expression.indexOf(':') + 1, expression.length());
            
            enteringPassword = expression.substring(0, expression.length());

            if (checkPassword(enteringPassword, type)) {
                Serial.write("1-");
                sendAction(true);
            } else {
                Serial.write("0-");
            }

        } 
    }
}

void sendAction(boolean isActionAuthorized) {

    if (!isActionAuthorized) {
        opener
    }

}

void getPasswords() {
    if (client.connect(local, 8080)) {
        
        // Send HTTP request
        client.println("GET /esp HTTP/1.0\r\n");
        client.println("Host: 192.168.0.14:8080\r\n\r\n");
        delay(200);

        // Skip HTTP headers
        char endOfHeaders[] = "\r\n\r\n";
        if (!client.find(endOfHeaders)) {                      
            Serial.println("Invalid response");
            return;
        }

        deserializeJson(doc, client);
        client.stop();
        // serializeJson(doc, Serial);
    }
}

void improvedDelay(unsigned int waitTime) {
    globalTimeBufferMillis = millis();
    boolean cooldownState = true;

    while (cooldownState) {
        if (millis() - globalTimeBufferMillis > waitTime) 
            cooldownState = false;
    }
}