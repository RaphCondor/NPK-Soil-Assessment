#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define redpin 3
#define greenpin 5
#define bluepin 6

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


int pin4 = 0;
int pin5 = 0;
int pin6 = 0;
int pin4State = 0;
int pin5State = 0;
int pin6State = 0;

#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);

  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);

  if (tcs.begin()) {
    //Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

} 

void loop() {
  float red, green, blue;
  
  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRGB(&red, &green, &blue);
  
  tcs.setInterrupt(true);  // turn off LED

  Serial.print("R:\t"); Serial.print(int(red)); 
  Serial.print("\tG:\t"); Serial.print(int(green)); 
  Serial.print("\tB:\t"); Serial.print(int(blue));

//  Serial.print("\t");
//  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\n");

//  uint16_t red, green, blue, clear;
//  
//  tcs.setInterrupt(false);  // turn on LED
//
//  delay(60);  // takes 50ms to read
//
//  tcs.getRawData(&red, &green, &blue, &clear);
//  
//  tcs.setInterrupt(true);  // turn off LED
//
//  Serial.print("C:\t"); Serial.print(int(clear)); 
//  Serial.print("R:\t"); Serial.print(int(red)); 
//  Serial.print("\tG:\t"); Serial.print(int(green)); 
//  Serial.print("\tB:\t"); Serial.print(int(blue));
//  Serial.println();

    float hue = 0;
    float saturation = 0;
    float lightness = 0;
    float r, g ,b, maxim, minim, delta;

    r = red/255.0;
    g = green/255.0;
    b = blue/255.0;


    maxim = max(max(r,g),b);
    minim = min(min(r,g),b);
    delta = maxim - minim;
    lightness = (maxim + minim)/2.0;
  
    
    if (delta != 0)
        {
            if (lightness < 0.5)
            {
                saturation = (delta / (maxim + minim));
            }
            else
            {
                saturation = (delta / (2.0 - maxim - minim));
            }


            if (r == maxim)
            {
                hue = (g - b) / delta;
            }
            else if (g == maxim)
            {
                hue = 2 + (b - r) / delta;
            }
            else if (b == maxim)
            {
                hue = 4 + (r - g) / delta;
            }
        }

        Serial.print("HUE: ");
        Serial.print(hue);
        
        Serial.print("||");
        
        Serial.print("SATURATION: ");
        Serial.print(saturation);
        
        Serial.print("||");
        
        Serial.print("LIGHTNESS: ");
        Serial.println(lightness);
        delay (1000);

  
  float concentration = 0.0;
  
    pin4 = digitalRead(4);
    pin5 = digitalRead(5);
    pin6 = digitalRead(6);

    if(pin4 == HIGH){
      pin4State = HIGH;
      pin5State = LOW;
      pin6State = LOW;
    }
    
    else if(pin5 == HIGH){
      pin4State = LOW;
      pin5State = HIGH;
      pin6State = LOW;
    }
    
    else if(pin6 == HIGH){
      pin4State = LOW;
      pin5State = LOW;
      pin6State = HIGH;
    }

    if(pin4State == HIGH){
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(20, 0);
      // Display static text
      display.println("NITROGEN");
      display.setTextSize(1);
      if(hue <= 0.44){
      display.setCursor(30,50);
      display.println("Surplus");
      }
      else if(hue >0.44 && hue <= 0.55) {
        display.setCursor(30,50);
        display.println("Sufficient");
      }
      else if(hue >0.55 && hue <= 0.73) {
        display.setCursor(30,50);
        display.println("Adequate");
      }
      
      else if(hue >0.73 && hue <= 0.93) {
        display.setCursor(30,50);
        display.println("Sufficient");
      }
      
      else if(hue >0.93) {
        display.setCursor(30,50);
        display.println("Surplus");
      }
      display.setTextSize(2);
      display.setCursor(0,20);
      concentration = (hue-0.442)/0.0021;
      display.print(concentration);
      display.println("PPM");


      
      display.display(); 
    }
    
    else if(pin6State == HIGH){
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(5, 0);
      // Display static text
      display.println("PHOSPHORUS");

      display.setTextSize(1);
      if(hue <= 1.29){
      display.setCursor(30,50);
      display.println("Depleted");
      }
      else if(hue >1.29 && hue <= 1.34) {
        display.setCursor(30,50);
        display.println("Deficient");
      }
      else if(hue >1.34 && hue <= 1.47) {
        display.setCursor(30,50);
        display.println("Adequate");
      }
      
      else if(hue >1.40 && hue <= 1.60) {
        display.setCursor(30,50);
        display.println("Sufficient");
      }
      
      else if(hue >1.60) {
        display.setCursor(30,50);
        display.println("Surplus");
      }

      display.setTextSize(2);
      display.setCursor(0,20);
      concentration = (hue-1.286)/0.0027;
      display.print(concentration);
      display.println("PPM");
      display.display(); 
    }
    
    else if(pin5State == HIGH){
      
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(10, 0);
      // Display static text
      display.println("POTASSIUM");

      display.setTextSize(1);
      if(hue <= 0.70){
      display.setCursor(30,50);
      display.println("Adequate");
      }
      else if(hue >0.70 && hue <= 0.74) {
        display.setCursor(30,50);
        display.println("Sufficient");
      }
      else if(hue >0.74 && hue <= 0.85) {
        display.setCursor(30,50);
        display.println("Surplus");
      }
      else if(hue >0.85 && hue <= 1.00) {
        display.setCursor(30,50);
        display.println("Deficient");
      }
      else if (hue >= 1.00){
        display.setCursor(30,50);
        display.println("Depleted");
      }


      display.setTextSize(2);
      display.setCursor(0,20);
      concentration = (hue-0.388)/0.0016;
      display.print(concentration);
      display.println("PPM");
      display.display(); 
    }
  
}
