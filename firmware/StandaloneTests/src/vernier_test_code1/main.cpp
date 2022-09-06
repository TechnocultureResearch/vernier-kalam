#include <Arduino.h>


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library

// ST7789 TFT module connections
#define TFT_CS    PB9  // define chip select pin
#define TFT_DC     PB8  // define data/command pin
#define TFT_RST    8  // define reset pin, or set to -1 and connect to Arduino RESET pin
#define BAUDRATE  115200
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 135

#define VERNIER_DATA_PIN PA11
#define VERNIER_CLOCK_PIN PA12


// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 vernier_oled = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

int i;

int sign;

long value;

float result;

unsigned long tempmicros;

float reading=0;


void oledPrint();
float decode();

void setup() {
  Serial.begin(BAUDRATE);
   pinMode(VERNIER_CLOCK_PIN, INPUT);
   pinMode(VERNIER_DATA_PIN, INPUT);
  Serial.print(F("Hello! ST77xx TFT Test"));

  // if the display has CS pin try with SPI_MODE0
  vernier_oled.init(DISPLAY_HEIGHT, DISPLAY_WIDTH, SPI_MODE0);    // Init ST7789 display 240x240 pixel

  // if the screen is flipped, remove this command
  vernier_oled.setRotation(3);

  Serial.println(F("Initialized"));
// vernier_oled print function!
//   oledPrint();
//   delay(4000);
//  vernier_oled.fillScreen(ST77XX_WHITE);
 
vernier_oled.fillScreen(ST77XX_WHITE);

  Serial.println("done");
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
 
  //   float vernier_reading = decode();
    oledPrint();
                   // wait for a second
}

void oledPrint() {
  vernier_oled.setTextWrap(false);
  
  vernier_oled.setCursor(0, 30);
  vernier_oled.setTextColor(ST77XX_BLUE);
  vernier_oled.setTextSize(3);
  vernier_oled.print(decode());
 delay(1500);
 
}

float decode() {

  sign=1;

  value=0;


  for (i=0;i<23;i++) {

    while (digitalRead(VERNIER_CLOCK_PIN)==HIGH) { } //wait until clock returns to HIGH- the first bit is not needed

    while (digitalRead(VERNIER_CLOCK_PIN)==LOW) {} //wait until clock returns to LOW

    if (digitalRead(VERNIER_DATA_PIN)==LOW) {

      if (i<20) {

        value|= 1<<i;

      }

      if (i==20) {

        sign=-1;

      }

    }

  }

  result=(value*sign)/100.00;    

 // Serial.println(result,2); //print result with 2 decimals
return result;

}     
