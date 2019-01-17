#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 4

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  //Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() { 

  // previous state dimmer code (red and green colors)
  FadeInExistingAll2Color(strip.Color(255, 0, 0), strip.Color(0, 255, 0));
  colorAll2Color(strip.Color(255, 0, 0), strip.Color(0, 255, 0), 150); // Red Green
  FadeOutExistingAll2Color(strip.Color(255, 0, 0), strip.Color(0, 255, 0));
  delay(1000);  // wait 1sec

  // goes to black, issue that all bulbs have to be same color
  //FadeInOut(0xff, 0x77, 0x00);
  //delay(1000);


  // white to dark, issue that this code never goes to black
  //brighten();
  //darken();
  //delay(1000);

}



void FadeInExistingAll2Color(uint32_t c1, uint32_t c2)
{
  uint32_t stripArray [ strip.numPixels() ];
  uint32_t currentcolor;
  uint32_t dimmedcolor;
  //int numerator;
  //float percent;

  // capture end result
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    if(i % 2 == 0) //even
        stripArray[i] = c1;
    else // odd   
      stripArray[i] = c2;
  } 


  // slowly turn up brightness
  for(int dim = 1; dim < 256 ; dim++ ) 
  {

    //dim each color on th strip
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
      // get current color
      currentcolor = stripArray[i];

      // change brightness color
      dimmedcolor = ColorBrightness(currentcolor, dim);

      // set color on strip
      strip.setPixelColor(i, dimmedcolor);
      //delay(10);
      //strip.show();
    } 
    //percent = ( (255-dim) / 256);
    //Serial.println(dim);
    //Serial.println(percent);
    delay(10);
    strip.show();
  }

}


void FadeOutExistingAll2Color(uint32_t c1, uint32_t c2)
{
  uint32_t stripArray [ strip.numPixels() ];
  uint32_t currentcolor;
  uint32_t dimmedcolor;

  // capture end result
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    if(i % 2 == 0) //even
        stripArray[i] = c1;
    else // odd   
      stripArray[i] = c2;
  } 


  // slowly turn down brightness
  for(int dim = 255; dim >= 0 ; dim-- ) 
  {

    //dim each color on th strip
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
      // get current color
      currentcolor = stripArray[i];

      // change brightness color
      dimmedcolor = ColorBrightness(currentcolor, dim);

      // set color on strip
      strip.setPixelColor(i, dimmedcolor);
      //delay(10);
      //strip.show();
    } 

    delay(10);
    strip.show();
  }

}



uint32_t ColorBrightness(uint32_t color, uint32_t k)
{
    float  r = (k/256.0) * Red(color);
    float  g = (k/256.0) * Green(color);
    float  b = (k/256.0) * Blue(color);

    uint32_t dimColor = strip.Color(r, g, b);
    return dimColor;
}


uint32_t DimColorBitShift(uint32_t color)
{
    uint32_t dimColor = strip.Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
}

// Returns the Red component of a 32-bit color
uint8_t Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
{
    return color & 0xFF;
}



void FadeOutExistingState()
{
  for(int i = 255; i > 0 ; i-- ) {
    strip.setBrightness(i);  
    delay(10);
    strip.show();
  }

}


void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;

  
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    delay(10);
    strip.show();
  }

  delay(1000);

  for(int k = 255; k >= 0; k=k-1) 
  {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    strip.show();
  }
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < strip.numPixels() ; i++ ) {
    strip.setPixelColor(i, red, green, blue); 
  }
  strip.show();
}

// 0 to 255
void brighten() {
  uint16_t i, j;

  for (j = 45; j < 255; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, j, j, j);
    }
    strip.show();
    delay(10);
  }
  //delay(1500);
}

// 255 to 0
void darken() {
  Serial.begin(9600);
  uint16_t i, j; 
  for (j = 255; j > 45; j--) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, j, j, j);
    }
    strip.show();
    delay(10);
    Serial.println(j);
  }
  delay(1500);
}

// Fill the dots one after the other with a color
void colorAll2Color(uint32_t c1, uint32_t c2, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(i % 2 == 0) //even
        strip.setPixelColor(i, c1);
    else // odd   
      strip.setPixelColor(i, c2);
  } 
    strip.show();
    delay(wait*10);
}
