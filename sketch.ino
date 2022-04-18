#include <ArduinoJson.h>

#include <ESP8266WiFi.h>              
#include <WiFiClient.h>
#include <TimeLib.h>

#define LED_BUILTIN 2

volatile unsigned long globalTimeBufferMillis = 0;

const char* ssid = "Ttk 139";
const char* password = "631790355";

WiFiClient client;
IPAddress local(192, 168, 0, 25);
uint16_t port = 8080;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);                                       
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED)
        delay(500);

    pingController();
    
}

void loop() {}

void pingController() {
    if (client.connect(local, 8080)) {
        
        // Send HTTP request
        client.println("GET /esp HTTP/1.0\r\nHost: 192.168.0.25:8080\r\n\r\n");
        delay(200);

        // Skip HTTP headers
        char endOfHeaders[] = "\r\n\r\n";
        if (!client.find(endOfHeaders)) {                      
            Serial.println("Invalid response");
            return;
        }

        const size_t capacity = 750;
        DynamicJsonBuffer jsonBuffer(capacity);

    
        JsonObject& root = jsonBuffer.parseObject(client);
        client.stop();   

        Serial.println(F("Response:"));
        Serial.println(root["id"].as<int>());
        Serial.println(root["name"].as<String>());
        Serial.println(root["name2"].as<String>());

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