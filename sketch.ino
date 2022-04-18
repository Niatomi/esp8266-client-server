#include <ESP8266WiFi.h>              
#include <WiFiClient.h>
#include <TimeLib.h>
#include <ArduinoJson.h>

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
        // client.println("GET /esp HTTP/1.1\r\nHost: 192.168.0.25:8080");
        
        client.println("GET /esp HTTP/1.1");
        client.println("Host: localhost:8080");
        client.println(F("Connection: close"));

        if (client.println() == 0) {
            Serial.println(F("Failed to send request"));
            client.stop();
            return;
        } 
        
        delay(200);
        char endOfHeaders[] = "\r\n\r\n"; 

        if (!client.find(endOfHeaders)) {                                       
            Serial.println("Invalid response");                                   
        }

        const size_t capacity = 750;
        DynamicJsonBuffer jsonBuffer(capacity);
        JsonObject& root = jsonBuffer.parseObject(client);

        client.stop();

        String name = root["name"].as<String>();
        Serial.println("");
        Serial.println("Name is: " + name);

    }
}

void improvedDelay(unsigned int waitTime) {
    globalTimeBufferMillis = millis();
    boolean cooldownState = true;

    while (cooldownState) {
        if (millis() - globalTimeBufferMillis > waitTime) 
            cooldownState = false;
    }

    const size_t capacity = 750;
    DynamicJsonBuffer jsonBuffer(capacity); 

    JsonObject& root = jsonBuffer.parseObject(client);                      
    client.stop();  
}

// GET /esp HTTP/1.1
// Host: localhost:8080
