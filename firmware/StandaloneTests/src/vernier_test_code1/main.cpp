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

#define DATA_PIN PB6
#define CLOCK_PIN PB7


// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 vernier_oled = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int bit_array[25];        // For storing the data bit. bit_array[0] = data bit 1 (LSB), bit_array[23] = data bit 24 (MSB).
unsigned long time_now;   // For storing the time when the clock signal is changed from HIGH to LOW (falling edge trigger of data output).
 

void decode();

void setup() {
  Serial.begin(BAUDRATE);
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
//  Serial.print(F("Hello! ST77xx TFT Test"));

  // if the display has CS pin try with SPI_MODE0
  vernier_oled.init(DISPLAY_HEIGHT, DISPLAY_WIDTH, SPI_MODE0);    // Init ST7789 display 240x240 pixel
  // if the screen is flipped, remove this command
  vernier_oled.setRotation(3);
  vernier_oled.fillScreen(ST77XX_WHITE);

  Serial.println("done");
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
 
   while (digitalRead(CLOCK_PIN) == LOW) {}  // If clock is LOW wait until it turns to HIGH
  time_now = micros();
  while (digitalRead(CLOCK_PIN) == HIGH) {} // Wait for the end of the HIGH pulse
  if ((micros() - time_now) > 500)
   {        // If the HIGH pulse was longer than 500 micros we are at the start of a new bit sequence
    decode(); //decode the bit sequence
   }
                   // wait for a second
}

void decode()
  {
    int sign = 1;
    int i = 0;
    float value = 0.0;
    float result = 0.0;
  
    bit_array[i] = digitalRead(DATA_PIN);       // Store the 1st bit (start bit) which is always 1.
    while (digitalRead(CLOCK_PIN) == HIGH) {};
  
    for (i = 1; i <= 24; i++)
     {
      while (digitalRead(CLOCK_PIN) == LOW) { } // Wait until clock returns to HIGH
      bit_array[i] = digitalRead(DATA_PIN);  
      while (digitalRead(CLOCK_PIN) == HIGH) {} // Wait until clock returns to LOW
     }
  
    // for (i = 0; i <= 24; i++) {                 // Show the content of the bit array. This is for verification only.
    //   Serial.print(bit_array[i]);
    //   Serial.print(" ");
    // }
   //   Serial.println();
  
    for (i = 1; i <= 20; i++) {                 // Turning the value in the bit array from binary to decimal.
        value = value + (pow(2, i-1) * bit_array[i]);
    }
  
    if (bit_array[21] == 1) sign = -1;          // Bit 21 is the sign bit. 0 -> +, 1 => -
  
    if (bit_array[24] == 1) 
    {                   // Bit 24 tells the measuring unit (1 -> in, 0 -> mm)
      result = (value*sign) / 2000.00;
    //  Serial.print(result,3);                   // Print result with 3 decimals
    //  Serial.println(" in");
      vernier_oled.fillScreen(ST77XX_WHITE);
      vernier_oled.setTextWrap(true);
      vernier_oled.setCursor(10, 50);
      vernier_oled.setTextColor(ST77XX_BLUE);
      vernier_oled.setTextSize(4);
      vernier_oled.print(result,3);
      vernier_oled.setTextSize(3);
      vernier_oled.print("in");
    } 
    else 
      {
        result = (value*sign) / 100.00;  
     //   Serial.print(result,2);                   // Print result with 2 decimals
     //  Serial.println(" mm"); 
        vernier_oled.fillScreen(ST77XX_WHITE);
        vernier_oled.setTextWrap(true);
        vernier_oled.setCursor(10, 50);
        vernier_oled.setTextColor(ST77XX_BLUE);
        vernier_oled.setTextSize(6);
        vernier_oled.print(result,2); 
        vernier_oled.setTextSize(4);
        vernier_oled.print("mm");
      }
    delay(5);
}