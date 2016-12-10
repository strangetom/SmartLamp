#include <Arduino.h>

// These libraries are required for the webserver
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// This library is used to control the NeoPixel ring
#include <Adafruit_NeoPixel.h>


// Set up the LED ring
#define PIN D1
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
// Set up variables for setting the colour
uint16_t j = 0; // counter for iterating through cycle
uint8_t red, green, blue; // hold the rgb values for a static colour
String hexColour = "#3d34b9"; // hex representation of static colour sent by http
char* lampMode = "rainbow"; // mode the lamp is in

// Set up webserver and connect to wifi
const char* ssid = "ssid";
const char* pass = "pass";  
int status = WL_IDLE_STATUS;
ESP8266WebServer server(80);


void setup() {
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
      server.send(200, "text/html", buildWebpage(hexColour, lampMode));
    });

  server.on("/rainbow", [](){
    lampMode = "rainbow";
    server.send(200, "text/html", buildWebpage(hexColour, lampMode));
  });

  server.on("/warm", [](){
    lampMode = "warm";
    server.send(200, "text/html", buildWebpage(hexColour, lampMode));
  });

  server.on("/cold", [](){
    lampMode = "cold";
    server.send(200, "text/html", buildWebpage(hexColour, lampMode));
  });

  server.on("/off", [](){
    lampMode = "off";
    server.send(200, "text/html", buildWebpage(hexColour, lampMode));
  });

  server.on("/colour", [](){
    hexColour = server.arg(0);
    lampMode = "static";
    server.send(200, "text/html", buildWebpage(hexColour, lampMode));;

    // Convert the hex number to rgb values
    int number = (int)strtol( &hexColour[1], NULL, 16);
    red = number >> 16;
    green = number >> 8 & 0xFF;
    blue = number & 0xFF;
  });

  // Begin websever
  server.begin();
  Serial.println("HTTP server started");
  
  // Initialise LEDs
  ring.begin();
  ring.show();
}

void loop() {
  // When client connects, serve requested webpage
  server.handleClient();
  
  // Set lamp colour according to counter variable and mode
  if (strcmp(lampMode, "rainbow") == 0){ // rainbow mode
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel(j));
    }
    ring.show();

    Serial.println("LED rainbow: "  + String(j));
    
  } else if (strcmp(lampMode, "static") == 0){ //static mode
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(red, green, blue));
    }
    ring.show(); 
    
    Serial.println("LED static: "  + String(j));
    
  } else if (strcmp(lampMode, "warm") == 0){ // warm mode
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, warmWheel(j));
    }
    ring.show();
  
    Serial.println("LED warm: "  + String(j));
    
  } else if (strcmp(lampMode, "cold") == 0){ // cold mode
    for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, coldWheel(j));
    }
    ring.show();

    Serial.println("LED cold: "  + String(j));
    
  } else if (strcmp(lampMode, "off") == 0){ // off mode
    for(uint16_t i=0; i<ring.numPixels(); i++){
      ring.setPixelColor(i, ring.Color(0, 0, 0));
    }
    ring.show(); 

    Serial.println("LED off: " + String(j));
    
  } else { // This should never happen
    Serial.println("Unrecognised LED state");
    return;
  }

  // Increment or reset counter as appropriate
  if(j == 255){
    j = 0;
  }else{
    j++;
  }

  // Wait 1000 ms before looping again, so the lamp colour doesn't change too quickly
  delay(1000);
  
}

// Build webpage showing last selected colour for colour picker and current lamp mode
String buildWebpage(String hexColour, char* lampMode){
  String webPage = "";
  webPage += "<!DOCTYPE html>\
  <html>\
    <head>\
      <title>ESP8266 Lamp</title>\
      <meta name='viewport' content='width=device-width, initial-scale=1'>\
      <style type='text/css'>html{background-color:#fff;}body{margin:0 auto;max-width:50em;line-height:1.5;padding:2em 1em;border:40px solid #fff;border-radius:45px;background:#F0F0F0;color:#566b78;font-family:sans-serif;-moz-font-smoothing:grayscale;-webkit-font-smoothing:antialiased}h1,strong{color:#333;font-weight:500}a{color:#e81c4f}</style>\
    </head>\
    <body>\
    <center><h1>ESP8266 LED LAMP CONTROL</h1></center>\
    <p><b>The lamp is currently <a>";
  webPage += lampMode;    
  webPage += "</a>.</b></p>\
    There are a few different modes for the lamp.\
    <ul>\
      <li> <a href=\"rainbow\">Rainbow mode</a>, the lamp will cycle through all colours of the rainbow.</li>\
      <li> <a href=\"warm\">Warm mode</a>, the lamp will cycle yellow, red, purple and back again.</li>\
      <li> <a href=\"cold\">Cold mode</a>, the lamp will cycle green, blue, purple and back again.</li>\
      <li> <form id='static' action='colour' method='post'>\
        Static mode:\
        <input type='color' name='colour' value='";
   webPage += hexColour;
   webPage += "' style='width:15%' onchange=\"document.getElementById('static').submit();\">\
       , the lamp will have a single colour.\
      </form> </li>\
      <li> Or finally, we can turn the lamp <a href=\"off\">off.</a> </li>\
    </ul>\
    </body>\
  </html>";
  return webPage;
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
