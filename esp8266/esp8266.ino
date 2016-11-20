#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include<Adafruit_NeoPixel.h>

// Note that LOW turns the LED is on; this is because it is active LOW

// Set LED ring
#define PIN D1
uint16_t j = 0; // determine colour of LEDs
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
// Colours for static mode
uint8_t red, green, blue;

const char* ssid = "ssid";
const char* pass = "pass";  
int status = WL_IDLE_STATUS;
ESP8266WebServer server(80);

String webPage = "";
char* lampMode = "rainbow";

void setup() {
  // Set webpage to display
  webPage += "<!DOCTYPE html>\
  <html>\
    <head>\
      <title>ESP8266 Server</title>\
      <meta name='viewport' content='width=device-width, initial-scale=1'>\
      <style type='text/css'>html{background-color:#fff;}body{margin:0 auto;max-width:50em;line-height:1.5;padding:2em 1em;border:40px solid #fff;border-radius:45px;background:#F0F0F0;color:#566b78;font-family:sans-serif;-moz-font-smoothing:grayscale;-webkit-font-smoothing:antialiased}h1,strong{color:#333;font-weight:500}a{color:#e81c4f}</style>\
    </head>\
    <body>\
    <center><h1>ESP8266 LED LAMP CONTROL</h1></center>\
    <p> Put the lamp in <a href=\"rainbow\">rainbow</a> mode, or</p>\
    <p> put the lamp in <a href=\"warm\">warm</a> mode, or</p>\
    <p> put the lamp in <a href=\"cold\">cold</a> mode, or maybe try</p>\
    <p> <form id='static' action='rgb' method='post' oninput=\"rgb.value = Red.value+' '+Green.value+' '+Blue.value\">\
      <a onclick=\"document.getElementById('static').submit();\" style='cursor: pointer;text-decoration: underline;'>static</a> mode:\
      R <input type='text' name='R' size='1' value='255'>\ 
      G <input type='text' name='G' size='1' value='255'>\ 
      B <input type='text' name='B' size='1' value='255'>\ 
      <output name='rgb'> </output>\
    </form> </p>\
    <p> Finally, we can turn the lamp <a href=\"off\">off.</a> </p>\
    </body>\
  </html>";

  // Initialise serial for debugging
  Serial.begin(115200);
  Serial.println("Attempting to connect to network...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  // Connect to WiFi
  status = WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Functions for lamp settings
  server.on("/", [](){
      server.send(200, "text/html", webPage);
    });

  server.on("/rainbow", [](){
    server.send(200, "text/html", webPage);
    lampMode = "rainbow";
  });

  server.on("/warm", [](){
    server.send(200, "text/html", webPage);
    lampMode = "warm";
  });

  server.on("/cold", [](){
    server.send(200, "text/html", webPage);
    lampMode = "cold";
  });

  server.on("/off", [](){
    server.send(200, "text/html", webPage);
    lampMode = "off";
  });

  server.on("/rgb", [](){
    server.send(200, "text/html", webPage);;
    red = server.arg(0).toInt();
    green = server.arg(1).toInt();
    blue = server.arg(2).toInt();
    lampMode = "static";
  });

  server.begin();
  Serial.println("HTTP server started");
  
  // Initialise LEDs
  ring.begin();
  ring.show();
}

void loop() {
  server.handleClient();
  
  // Change LED
  if (strcmp(lampMode, "rainbow") == 0){
   
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel(j));
    }
    ring.show();
    if(j == 255){
      j = 0;
    }else{
      j++;
    }
    Serial.println("LED rainbow: "  + String(j));
    
  } else if (strcmp(lampMode, "static") == 0){
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(red, green, blue));
    }
    ring.show();       
    Serial.println("LED static: "  + String(j));
    
  } else if (strcmp(lampMode, "warm") == 0){
    
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, warmWheel(j));
    }
    ring.show();
    if(j == 255){
      j = 0;
    }else{
      j++;
    }    
    Serial.println("LED warm: "  + String(j));
    
  } else if (strcmp(lampMode, "cold") == 0){
    
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, coldWheel(j));
    }
    ring.show();
    if(j == 255){
      j = 0;
    }else{
      j++;
    }  
    Serial.println("LED cold: "  + String(j));
    
  } else if (strcmp(lampMode, "off") == 0){
    LEDs_off();
    Serial.println("LED off");
    
  } else {
    Serial.println("Unrecognised LED state");
    return;
  }

  delay(1000);
  
}

// Turn LEDs off
void LEDs_off(){
  for(uint16_t i=0; i<ring.numPixels(); i++){
    ring.setPixelColor(i, ring.Color(0, 0, 0));
  }
  ring.show();  
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