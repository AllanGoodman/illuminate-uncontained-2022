/*
 * UNSW Illuminate 2022 - [Un]contained - LED Control
 * Written by:
 *    - Allan Goodman
 *    - Mitchell Wallace
 *    - Iniyan Vigneswaran
 *    - Leonard Chiang
 *    - Janice Nyoto
 *    - Nila Riahi
 */

 #include <FastLED.h>

//Initialise pins numbers
#define LED_DATA                    5 //Data pin on digital pin 5

#define ULTRASONIC_SENSOR_1_TRIG    0
#define ULTRASONIC_SENSOR_1_ECHO    0
#define ULTRASONIC_SENSOR_2_TRIG    0
#define ULTRASONIC_SENSOR_2_ECHO    0
#define ULTRASONIC_SENSOR_3_TRIG    0
#define ULTRASONIC_SENSOR_3_ECHO    0

//Misc LED initialisation stuff
#define COLOUR_ORDER       GRB
#define LED_TYPE           WS2811
#define BRIGHTNESS         64
#define NUM_LEDS           120 //120 LEDs on the strip we'll be using
#define UPDATES_PER_SECOND 100
CRGB leds[NUM_LEDS];
CRGBPalette16              currentPalette; //sets what colours we can use
/*
 * can also use the following preset palettes for other colours:
 * RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
 */
TBlendType                 currentBlending;

//Misc Ultrasonic sensor stuff
#define NUM_ULTRA_SENSORS   3
#define DISTANCE_THRESHOLD  10 //change this as per your measurements :))

// Global Variables
int brightness = 0;

//This function returns an array containing how far away an object is from each sensor
int * distanceDetected(){
  int buf[NUM_ULTRA_SENSORS] = {0};
  for (int i = 0; i<NUM_ULTRA_SENSORS; i++){
    buf[i] = 0; //Replace this with sensing the distance from the ultrasonic sensor
  }

  return buf;
}

//This function writes colour to the LEDs 
//Root determines the type of LED lighting
void generateLED(int root){
  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND);
}

void setup() {
  delay(3000); //power-up safely delay
  //Initialise pins

  //Initialise sensors

  //Initialise LEDs
  FastLED.addLeds<LED_TYPE, LED_DATA, COLOUR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  
}

void loop() {
  static uint8_t startIndex = 0;
  startIndex += 1;
  //FillLEDsFromPaletteColors(startIndex, brightness);
  //Check sensors
  int * buf = distanceDetected();
  bool isClose = false;
  for (int i; i<NUM_ULTRA_SENSORS; i++) {
    if (buf[i] < DISTANCE_THRESHOLD) { 
      isClose = true;
      break;
    }
  }

  if (isClose) { // pulse
    
  } else {
    
  }
  
  // PULSE CODE 
  for (int brightness=0; brightness < 256;  brightness+= 2) {
    //FastLED.setBrightness(brightness);
    FillLEDsFromPaletteColors(startIndex, brightness);
    FastLED.show();
    delay(2);
  }
  //test
  for (int brightness = 255; brightness > 2; brightness -= 2) {
    //FastLED.setBrightness(brightness);
    FillLEDsFromPaletteColors(startIndex, brightness);
    FastLED.show();
    delay(2);
  }  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex, int updated_brightness)
{
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, updated_brightness, currentBlending);
        colorIndex += 3;
    }
}

// NOT USED YET 
void pulse() {
  // to detect distance use distanceDetected()
  for (int brightness = 0; brightness < 256;  brightness+= 8) {
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(300);
  }
  
  for (int brightness = 255; brightness >= 0; brightness -= 8) {
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(300);
  }
}

// NOT USED YET 
// Turns on one LED at a time chronologically then loops back to the start
void chase() {
  for (int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++) { 
    leds[ledIndex] = CRGB(0, 255, 0); // green colour
    FastLED.show();
    leds[ledIndex] = CRGB(0, 0, 0); // turn off 
    delayMicroseconds(500);
  }

}
 
