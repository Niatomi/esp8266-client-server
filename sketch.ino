#include <ArduinoJson.h>
#include <ESP8266WiFi.h>              
#include <WiFiClient.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>
#include <Key.h>
#include <Keypad.h>

#define LED_BUILTIN 2
volatile unsigned long globalTimeBufferMillis = 0;

// Lcd
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

// WiFi, HTTP
const char* ssid = "Ttk 139";
const char* password = "631790355";

WiFiClient client;
IPAddress local(192, 168, 0, 25);
uint16_t port = 8080;

// Keypad
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns

char keymap[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {D1, D2, D3, D4}; 
byte colPins[COLS] = {D5, D6, D7}; 
Keypad keypad = Keypad( makeKeymap(keymap), rowPins, colPins, ROWS, COLS); 


void setup() {
    // pinMode(A0, INPUT);

    // lcd.init();
    // lcd.backlight();
    
    // lcd.setCursor(0, 0);
    // lcd.print("Connecting to:");
    // lcd.setCursor(0, 1);
    // lcd.print(ssid);

                      
           

    // WiFi.begin(ssid, password);
  
    // while (WiFi.status() != WL_CONNECTED)
    //     delay(500);

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Connection success");
    // lcd.clear();
//     Serial.setCursor(0, 0);

    // pingController();

     Serial.begin(9600);
}

void loop() {

    char key = keypad.getKey();

  if (key){
    Serial.println(key);
  }
}

void pingController() {
    if (client.connect(local, 8080)) {
        
        // Send HTTP request
        client.println("GET /esp HTTP/1.0\r\n");
        client.println("Host: 192.168.0.25:8080\r\n\r\n");
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