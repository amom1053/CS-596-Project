#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Wire.h>
#include <time.h>
// library specific for our LED
#include <Adafruit_DotStar.h>
#include <SPI.h>


// defining pins for LED
#define MOSI 15
#define SCK 22


#define NUMPIXELS 1
Adafruit_DotStar strip(NUMPIXELS, MOSI, SCK, DOTSTAR_BGR);


// 4 Photoresistor Pins
#define PIN36 36
#define PIN39 39 
#define PIN37 37
#define PIN38 38


// defining green and red button pins (INC - Green, DEC - Red)
#define INCBrightness 32
#define DECBrightness 2


// assign the control pins for the brightness inc and dec
const int incButtonPin = INCBrightness; 
const int decButtonPin = DECBrightness;
int normalLightLvl = 1245;
// defining a tolerance for the range of average light level
const int tolerance = 200;       
int brightness = 0;


char ssid[] = "NETGEAR34";
char pass[] = "Cost4Mes4,S4nt44n4,S4nDiego";


const char kHostname[] = "3.147.36.137";
const int kPort = 5000;
const char kPath[] = "/";


const int kNetworkTimeout = 30 * 1000;
const int kNetworkDelay = 1000;


// create object representing display
TFT_eSPI display = TFT_eSPI();


unsigned long getCurrentTime() {
  time_t now;
  time(&now);
  return (unsigned long)now;
}


void setup() {
  Serial.begin(9600);
  // initialize the LED light
  strip.begin();
  strip.show();


  // initialize the wifi connection
  WiFi.begin(ssid, pass);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());




  // synching the systems time using network time protocol
  configTime(0, 0, "pool.ntp.org");
  Serial.println("Waiting for time sync...");
  while (getCurrentTime() < 1000) {
    delay(100);
  }
  Serial.println("Time synchronized.");


  // declaring the photoresistor pins to be inputs
  pinMode(PIN36, INPUT);
  pinMode(PIN39, INPUT);
  pinMode(PIN37, INPUT);
  pinMode(PIN38, INPUT);


  // initialize brightness to 0
  brightness = 0;


  // declaring the button pins to be input with pull-up resistors
  pinMode(incButtonPin, INPUT_PULLUP);
  pinMode(decButtonPin, INPUT_PULLUP);
}


// the head is the start of the LED strip (in our case the singular LED)
int head = 0;
uint32_t color = 0xFFFFFF; // color of LED, white in this case




void loop() {
  // LED code
  // setting the LED to its proper color (white)
  strip.setPixelColor(head, color);


  // reading in the data from the photoresistors
  int reading36 = analogRead(PIN36);
  display.print("Pin 36: ");
  display.setTextColor(TFT_ORANGE);
  display.println(reading36);
  display.println();


  Serial.print("Pin 36: ");
  Serial.println(reading36);


  int reading37 = analogRead(PIN37);
  display.print("Pin 37: ");
  display.setTextColor(TFT_ORANGE);
  display.println(reading37);
  display.println();


  Serial.print("Pin 37: ");
  Serial.println(reading37);


  int reading38 = analogRead(PIN38);
  display.print("Pin 38: ");
  display.setTextColor(TFT_ORANGE);
  display.println(reading38);
  display.println();


  Serial.print("Pin 38: ");
  Serial.println(reading38);


  int reading39 = analogRead(PIN39);
  display.print("Pin 39: ");
  display.setTextColor(TFT_ORANGE);
  display.println(reading39);
  display.println();


  Serial.print("Pin 39: ");
  Serial.println(reading39);


  // Calculate the average light level that the photoresistors are capturing. Print the total average
  int averageLightLvl = (reading36 + reading39 + reading37 + reading38) / 4;


  display.print("Average Light Level: ");
  display.setTextColor(TFT_ORANGE);
  display.println(averageLightLvl);
  display.println();


  Serial.print("Average Light Level: ");
  Serial.println(averageLightLvl);


  // Printing the brightness of the LED lights
  Serial.print("LED Brightness: ");
  Serial.println(brightness);


  // Printing the light level set by the user (the default will always be 1245, but the user can adjust manually to brighten or lower the light)
  Serial.print("Normal Light Level: ");
  Serial.println(normalLightLvl);


  // Calculating if the average light level being captured by the photoresistors is within the light level boundary.
  // If the avg is not within the 1245+-200, the LED will brighten/dim to reach within the boundary
  if (averageLightLvl < 100) {
    strip.setBrightness(255);
    strip.show();
  }
  else if (averageLightLvl < (normalLightLvl - tolerance))
  {
    // If the average light level is below the tolerance, brighten the LED (gradually in increments of 10)
    brightness = constrain(brightness + 10, 0, 255); // Increase by 10, the range is always going to be 0-255
   
    strip.setBrightness(brightness);
    strip.show();


  }
  else if (averageLightLvl > (normalLightLvl + tolerance))
  {
    // If the average light level is above the normal tolerance, dim the LED (gradually decrementing by 10)
    brightness = constrain(brightness - 10, 0, 255); 
    
    strip.setBrightness(brightness);
    // display the brightness of the LED
    strip.show();
  }
  Serial.println();




  // If the GREEN button was pressed...
  if (digitalRead(incButtonPin) == LOW) {
    // increment the user defined normal light level by 15
    normalLightLvl += 15;
    Serial.print("Increased Normal Light Level to: ");
    Serial.println(normalLightLvl);
    delay(200); // Debounce delay
  }


  // If the RED button was pressed...
  if (digitalRead(decButtonPin) == LOW) {
    // decrement the user defined normal light level by 15
    normalLightLvl -= 15;
    Serial.print("Decreased Normal Light Level to: ");
    Serial.println(normalLightLvl);
    delay(200); // Debounce delay
  }


  // if the brightness of the LED hits 255 (its maximum), let the user know!
  if (brightness == 255) {
    Serial.println("LED is at maximum brightness!");
  }
  // if the average light level in the room is above 3000, automatically shut the LED off since the room is too bright
  if (averageLightLvl > 3000) {
    brightness = 0;
    Serial.println("LED automatically turned off due to high average light level!");
  }

  unsigned long currentTime = getCurrentTime();

  // createthe data string to send to our AWS cloud server
  String data = "reading36=" + String(reading36) +
                "&reading37=" + String(reading37) +
                "&reading38=" + String(reading38) +
                "&reading39=" + String(reading39) +
                "&averageLightLvl=" + String(averageLightLvl) +
                "&brightness=" + String(brightness) +
                "&normalLightLvl=" + String(normalLightLvl) +
                "&timestamp=" + String(currentTime);


  // assign dataPath to the data string holding all values of photoresistors, average light level, LED brightness, normal light level, and timestamp (timestamp needed for the html site)
  String dataPath = String(kPath) + "?" + data;



  // code retrieved from lab 3
  int err = 0;
  WiFiClient c;
  HttpClient http(c);


  err = http.get(kHostname, kPort, dataPath.c_str());
  if (err == 0) {
    err = http.responseStatusCode();
    if (err >= 0) {
      err = http.skipResponseHeaders();
      if (err >= 0) {
        int bodyLen = http.contentLength();
        unsigned long timeoutStart = millis();
        char character;
        while ((http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout)) {
          if (http.available()) {
            character = http.read();
            bodyLen--;
            timeoutStart = millis();
          } else {
            delay(kNetworkDelay);
          }
        }
        Serial.println();
      } else {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    } else {
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  } else {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();



  delay(5000);
}
