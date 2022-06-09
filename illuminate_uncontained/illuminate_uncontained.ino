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
#define LED_DATA_HAND               2 //Data pin on digital pin 5
#define LED_DATA_RIBBON_0           3
#define LED_DATA_RIBBON_1           4
#define LED_DATA_RIBBON_2           5
#define LED_DATA_RIBBON_3           6
#define LED_DATA_RIBBON_4           7
#define LED_DATA_RIBBON_5           8

#define ULTRASONIC_SENSOR_1_TRIG    22
#define ULTRASONIC_SENSOR_1_ECHO    23
#define ULTRASONIC_SENSOR_2_TRIG    24
#define ULTRASONIC_SENSOR_2_ECHO    25
#define ULTRASONIC_SENSOR_3_TRIG    26
#define ULTRASONIC_SENSOR_3_ECHO    27

//Misc LED initialisation stuff
#define COLOUR_ORDER       GRB
#define LED_TYPE           WS2811
#define BRIGHTNESS         64
#define NUM_LEDS_HAND      120 //120 LEDs on the strip we'll be using
#define NUM_LEDS_RIBBON    150      
#define UPDATES_PER_SECOND 100
#define PULSE_LENGTH       30
#define PULSE_DELAY        10
#define NUM_RIBBONS        1
CRGB leds_hand[NUM_LEDS_HAND];
CRGB leds_ribbons[NUM_RIBBONS][NUM_LEDS_RIBBON];
CRGBPalette16              currentPalette; //sets what colours we can use
/*
 * can also use the following preset palettes for other colours:
 * RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
 */
TBlendType                 currentBlending;

//Misc Ultrasonic sensor stuff
#define NUM_ULTRA_SENSORS   3
#define DISTANCE_THRESHOLD  400 //change this as per your measurements :))
#define DIST_TO_DELAY       80 //the number distance is multiplied to to get the delay in microseconds

struct Sensor {
  int trig;
  int echo;
};

struct Sensor sensors[NUM_ULTRA_SENSORS];

// Global Variables
int brightness = 0;
int ribbon_i = 1 - PULSE_LENGTH;
int hand_i = 0;
int ribbon_time = 0;
int hand_time = 0;

/*
//This function returns an array containing how far away an object is from each sensor
int * distanceDetected(){
  int buf[NUM_ULTRA_SENSORS] = {0};
  for (int i = 0; i<NUM_ULTRA_SENSORS; i++){
    buf[i] = 0; //Replace this with sensing the distance from the ultrasonic sensor
  }

  return buf;
}
*/

//This function returns the closest distance detected by the ultrasonic sensors
int getClosest() {
  int closest = 401;

  for (int i = 0; i < NUM_ULTRA_SENSORS; i++) {
    int dist = getDistance(sensors[i].trig, sensors[i].echo);
    if (dist < closest) closest = dist;
  }
  
  return closest;
}

//This function writes colour to the LEDs 
//Root determines the type of LED lighting
void generateLED(int root){
  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND);
}

int getDistance(int trigPin, int echoPin){
  // Clear the sending pin of Arduino
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the sendPin HIGH (ACTIVE) for 10 microseconds (actual sending of pulse)
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the receivePin, returns the sound wave travel time in microseconds (reception of reflection)
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance (half duration for go and back)
  // 0.034 is speed of sound at room temperature
  // temperature is inputted from temperature sensor
  // temperature / 25 is ratio of current temp to room temp
  int distance = duration / 2 * 0.034;
  return distance;
}

void setup() {
  delay(3000); //power-up safely delay
  
  Sensor tempSensor;

  tempSensor.trig = ULTRASONIC_SENSOR_1_TRIG;
  tempSensor.echo = ULTRASONIC_SENSOR_1_ECHO;

  sensors[0] = tempSensor;

  tempSensor.trig = ULTRASONIC_SENSOR_2_TRIG;
  tempSensor.echo = ULTRASONIC_SENSOR_2_ECHO;

  sensors[1] = tempSensor;

  tempSensor.trig = ULTRASONIC_SENSOR_3_TRIG;
  tempSensor.echo = ULTRASONIC_SENSOR_3_ECHO;

  sensors[2] = tempSensor;

  //Initialise pins
  pinMode(ULTRASONIC_SENSOR_1_TRIG, OUTPUT); // Sets the sendPin as an OUTPUT
  pinMode(ULTRASONIC_SENSOR_1_ECHO, INPUT); // Sets the receivePin as an INPUT
  //pinMode(sendPin2, OUTPUT); // Sets the sendPin as an OUTPUT
  //pinMode(receivePin2, INPUT); // Sets the receivePin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  
  //Initialise sensors
  

  //Initialise LEDs
  FastLED.addLeds<LED_TYPE, LED_DATA_HAND, COLOUR_ORDER>(leds_hand, NUM_LEDS_HAND).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_DATA_RIBBON_0, COLOUR_ORDER>(leds_ribbons[0], NUM_LEDS_RIBBON).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_DATA_RIBBON_1, COLOUR_ORDER>(leds_ribbons[0], NUM_LEDS_RIBBON).setCorrection( TypicalLEDStrip );
/*  FastLED.addLeds<LED_TYPE, LED_DATA_RIBBON_2, COLOUR_ORDER>(leds_ribbons[2], NUM_LEDS_RIBBON).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_DATA_RIBBON_3, COLOUR_ORDER>(leds_ribbons[3], NUM_LEDS_RIBBON).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_DATA_RIBBON_4, COLOUR_ORDER>(leds_ribbons[4], NUM_LEDS_RIBBON).setCorrection( TypicalLEDStrip );
*/


  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

}

void loop() {
  static uint8_t startIndex = 0;
  startIndex += 1;
  //chase(5, CRGB(0, 255, 0));
  //hand_i++;

  ribbon();
  //Check sensors
  /*
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
  */
  
  // PULSE CODE 
  /*
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
  */
}

void FillLEDsFromPaletteColors( uint8_t colorIndex, int updated_brightness)
{
    
    for( int i = 0; i < NUM_LEDS_HAND; i++) {
        leds_hand[i] = ColorFromPalette( currentPalette, colorIndex, updated_brightness, currentBlending);
        colorIndex += 3;
    }
}

void ribbon() {
  if (millis() < ribbon_time) {
    return;
  }

  ribbon_time = millis() + PULSE_DELAY;
  
  for (int i = 0; i < NUM_RIBBONS; i++) {
    
    /*if (ribbon_i == 0) {
      leds_ribbons[i][NUM_LEDS_HAND - 1] = CRGB(0,0,0);
      light_seg(ribbon_i, 15, CRGB(255,0,0), i); // turn on
      
    } else {
      leds_ribbons[i][ribbon_i - 1] = CRGB(0, 0, 0); // turn off unused LED from previous sequence
      leds_ribbons[i][(ribbon_i + PULSE_LENGTH - 1) % 360] = CRGB(255,0,0); // turn on newly required LED
    */  
      for (int i_trail = 0; i_trail < PULSE_LENGTH; i_trail++) {
          if (i_trail + ribbon_i >= 0) {
            leds_ribbons[i][ribbon_i + i_trail] = CRGB((int)(255 * ((float)i_trail/(float)PULSE_LENGTH)), 0, 0);
          }
      }

    //}


  }

  
  FastLED.show();
  ribbon_i++;
  if (ribbon_i >= NUM_LEDS_HAND) {
    ribbon_i = 1 - PULSE_LENGTH; 
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
// Turns on len LEDs at a time chronologically with colour col then loops back to the start
void chase(int len, CRGB col) {
  if (millis() < hand_time) {
    return;
  }
  
  int distance = getClosest();
  delayMicroseconds(2);
  Serial.println(distance);
  delayMicroseconds(2);
  int distDelay;
  // int tempDistance = getDistance(sendPin2, receivePin2);
  
  if (distance < DISTANCE_THRESHOLD) {
    distDelay = distance * DIST_TO_DELAY;
  } else {
    distDelay = DISTANCE_THRESHOLD * DIST_TO_DELAY; // or we could just have it turned off?
  }

  hand_time = millis() + distDelay;
  
  /*
  if (distance < 10) {
    distDelay = 500;
  } else if (distance > 9 && distance < DISTANCE_THRESHOLD){ 
    distDelay = 100000;
  } else {
    distDelay = 10000000;
  } 
  */


  if (hand_i == 0) {
    light_seg(hand_i, len, col, -1); // turn on
  } else {
    leds_hand[hand_i - 1] = CRGB(0, 0, 0); // turn off unused LED from previous sequence
    leds_hand[(hand_i + len - 1) % 360] = col; // turn on newly required LED
  }

  /*
  leds_hand[ledIndex] = CRGB(0, 255, 0); // green colour
  leds_hand[(ledIndex + 1) % 360] = CRGB(0, 255, 0); // green colour
  leds_hand[(ledIndex + 2) % 360] = CRGB(0, 255, 0); // green colour
  leds_hand[(ledIndex + 3) % 360] = CRGB(0, 255, 0); // green colour
  leds_hand[(ledIndex + 4) % 360] = CRGB(0, 255, 0); // green colour
  */
  FastLED.show();
  /*
  leds_hand[ledIndex] = CRGB(0, 0, 0); // turn off
  leds_hand[(ledIndex + 1) % 360] = CRGB(0, 0, 0); // turn off
  leds_hand[(ledIndex + 2) % 360] = CRGB(0, 0, 0); // turn off
  leds_hand[(ledIndex + 3) % 360] = CRGB(0, 0, 0); // turn off
  leds_hand[(ledIndex + 4) % 360] = CRGB(0, 0, 0); // turn off
  */
  delayMicroseconds(distDelay);

}

// Sets len LEDs starting at start to colour col
// Wraps around
void light_seg(int start, int len, CRGB col, int ribbon_num) {
  if (ribbon_num != -1){
    for (int i = 0; i < len; i++) {
      leds_ribbons[ribbon_num][(start + i) % NUM_LEDS_HAND] = col;
    }
  } else {
    for (int i = 0; i < len; i++) {
      leds_hand[(start + i) % NUM_LEDS_HAND] = col;
    }
  }
}

void light_all() {
  for (int i = 0; i < 120; i++) {
    leds_hand[i] = CRGB(0, 255, 0); // green colour
    FastLED.show();
  }
}
