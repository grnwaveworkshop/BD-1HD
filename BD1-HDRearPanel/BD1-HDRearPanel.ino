// BD-1 HD test program
// Requires FastLED library
// For Arduino Nano

#include <FastLED.h>


#define LED_PIN     3
#define COLOR_ORDER GRB
#define LED_TYPE    WS2812
#define NUM_LEDS    337

#define BRIGHTNESS  30
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];

CRGBPalette16 gPal;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// setup() function -- runs once at startup --------------------------------

void setup() {

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(1);

  Serial.begin(9600);
  randomSeed(analogRead(0));

}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
  //  confetti,
  //  juggle,
  twinkleFastBlue,
  twinkleFastRed
};

uint8_t gCurrentPatternNumber = 1; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS(20) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_SECONDS(15) {
    nextPattern();  // change patterns periodically
  }

}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void twinkleFastBlue() {
  twinkleFast(0x0000ff, 0x8850ff, 1, 10);
}

void twinkleFastRed() {
  twinkleFast(0xff0000, 0xaabbff, 1, 10);
}
/*
  Twinkle function:
  Slim down for faster operation with lots of LEDs
  main color, secondary color, number of cycles, delay
*/
void twinkleFast(CRGB color, CRGB color2, int cycles, int wait) {
  fadeToBlackBy( leds, NUM_LEDS, 20);

  for ( int i = 0; i < 12; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] = color;//CHSV(dothue, 200, 255);

  }
 // for ( int i = 0; i < 2; i++) {
    leds[beatsin16( 0 + 7, 0, NUM_LEDS - 1 )] = color2;
 // }
  addGlitter(40);
}

void addGlitter(uint8_t chanceOfGlitter)
{
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for ( int i = 0; i < 12; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] = 0x0000aa;//CHSV(dothue, 200, 255);
    dothue += 32;
  }
  for ( int i = 0; i < 4; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] = CHSV(dothue, 200, 255);
    dothue += 32;
  }

}
