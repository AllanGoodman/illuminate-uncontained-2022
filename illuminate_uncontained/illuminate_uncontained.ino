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

//This function returns boolean of if an object has been detected
int isDetected(){
  
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
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop() {
  //Check sensors
  if(isDetected()){
    
  }
}
