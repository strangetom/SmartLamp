#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include<Adafruit_NeoPixel.h>

// Note that LOW turns the LED is on; this is because it is active LOW

// Set LED ring
#define PIN D1
uint16_t j = 0; // determine colour of LEDs
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

char ssid[] = "ssid";        
char pass[] = "pass";   

int status = WL_IDLE_STATUS;
IPAddress server(192, 168, 0, 104);

// parameters for changing colour in notification mode
bool switch_colour = false;
uint32_t app_colour;


void setup() {
  // Initialise serial for debugging
  Serial.begin(115200);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  // Initialise LEDs
  ring.begin();
  ring.show();

  // Connect to WiFi
  status = WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to wifi");
}

void loop() {
  // Connect to server
  WiFiClient client;
  Serial.println("\nStarting connection...");
  // If you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("Connected");
    // Make a HTTP request:
    client.print(String("GET /light.json") + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
    delay(50);
  } else {
    Serial.println("Failed to connect to server");
    return;
  }

  // Read request
  String line;
  while (client.available()) {
    line = client.readStringUntil('\r');
    if (line == "\n"){
      line = client.readStringUntil('\r');
      //Serial.println(line);
      break;
    }
  }


  // Parse JSON
  int size = line.length() + 1;
  char json[size];
  line.toCharArray(json, size);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json_parsed = jsonBuffer.parseObject(json);
  if (json_parsed.success()){
    const char* status = json_parsed["mode"];
    Serial.println("Light status: " + String(status));
  } else {
    Serial.println("parseObject() failed");
    return;
  }

  // Change LED
  if (strcmp(json_parsed["mode"], "rainbow") == 0){
   
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel(j));
    }
    ring.show();
    if(j == 255){
      j = 0;
    }else{
      j++;
    }
    Serial.println("LED on: "  + String(j));
    
  } else if (strcmp(json_parsed["mode"], "static") == 0){
    uint8_t red = json_parsed["red"];
    uint8_t green = json_parsed["green"];
    uint8_t blue = json_parsed["blue"];
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(red, green, blue));
    }
    ring.show();       
    
  } else if (strcmp(json_parsed["mode"], "warm") == 0){
    
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, warmWheel(j));
    }
    ring.show();
    if(j == 255){
      j = 0;
    }else{
      j++;
    }    
    Serial.println(j);
    
  } else if (strcmp(json_parsed["mode"], "cold") == 0){
    
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, coldWheel(j));
    }
    ring.show();
    if(j == 255){
      j = 0;
    }else{
      j++;
    }
    
  } else if (strcmp(json_parsed["mode"], "notification") == 0){
    uint8_t red = json_parsed["red"];
    uint8_t green = json_parsed["green"];
    uint8_t blue = json_parsed["blue"];

    const char* app = json_parsed["app"];  
    if (strcmp(app, "pushbullet") == 0){
      app_colour = ring.Color(0, 255, 0);
    } else if(strcmp(app, "whatsapp") == 0){
      app_colour = ring.Color(255, 0, 255);
    } else if(strcmp(app, "sms") == 0){
      app_colour = ring.Color(0, 0, 255);
    } else if(strcmp(app, "gmail") == 0){
      app_colour = ring.Color(158, 26, 0);
    } else {
      app_colour = ring.Color(red, green, blue);
    }

    // Determine which colour to show
    if(j < 3){
      switch_colour = true;
      j++;
    } else {
      switch_colour = false;
      j++;
    }
    if(j % 10 == 0){
      j = 0;
    }

    // Show colour
    if(switch_colour == false){
      for(uint16_t i=0; i<ring.numPixels(); i++) {
        ring.setPixelColor(i, ring.Color(red, green, blue));
      }
    }else{
      for(uint16_t i=0; i<ring.numPixels(); i++) {
        ring.setPixelColor(i, app_colour);
      }
    }
    ring.show();  
    Serial.println(j);       
         
  } else if (strcmp(json_parsed["mode"], "off") == 0){
    LEDs_off();
    Serial.println("LED off");
    
  } else {
    Serial.println("Unrecognised LED state");
    return;
  }

  // Close connection to server
  client.stop();
}

// Turn LEDs off
void LEDs_off(){
  for(uint16_t i=0; i<ring.numPixels(); i++){
    ring.setPixelColor(i, ring.Color(0, 0, 0));
  }
  ring.show();  
}

void rainbow(uint8_t wait) {
  // Cycle through colours
  for(uint16_t j=0; j<256; j++) {
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel(j));
    }
    ring.show();
    delay(wait);     
   }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t warmWheel(byte WheelPos) {
  if (WheelPos < 63) {
    return ring.Color(255, 0, 255 - WheelPos * 4);
  }
  if (WheelPos < 66){
    // Hack because 255/4 is not an integer
    return ring.Color(255, 0, 0);
  }
  if (WheelPos < 129) {
    WheelPos -= 65;
    return ring.Color(255, WheelPos * 4, 0);
  }
  if (WheelPos < 192){
    WheelPos -= 128;
    return ring.Color(255, 255 - WheelPos * 4, 0);
  }
  WheelPos -= 192;
  return ring.Color(255, 0, WheelPos * 4);
}

uint32_t coldWheel(byte WheelPos) {
  if (WheelPos < 42) {
    return ring.Color(0, 255, WheelPos * 6);
  }
  if (WheelPos < 85) {
    WheelPos -= 42;
    return ring.Color(0, 255 - WheelPos * 6, 255);
  }
  if (WheelPos < 127){
    WheelPos -= 85;
    return ring.Color(WheelPos * 6, 0, 255);
  }
    if (WheelPos < 170){
    WheelPos -= 127;
    return ring.Color(255 - WheelPos * 6, 0, 255);
  }
    if (WheelPos < 213){
    WheelPos -= 170;
    return ring.Color(0, WheelPos * 6, 255);
  }
  WheelPos -= 213;
  return ring.Color(0, 255, 255 - WheelPos * 6);
}
