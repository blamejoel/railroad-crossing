#include <FastLED.h>

#define DATA_PIN 2

// Number of rings
#define NUM_RINGS 4

// LEDs per ring
#define PER_RING 16

// Total LEDs
#define NUM_LEDS PER_RING*NUM_RINGS

// First LED in each series
#define LEFTA_START 0
#define RIGHTA_START 16
#define LEFTB_START 32
#define RIGHTB_START 64

// Speed in ms to alternate flash
#define SPEED 500

// Brightness
#define BRIGHT 10

CRGBArray<NUM_LEDS> leds;

// shortcuts for each set
CRGBSet lefta(leds(LEFTA_START, LEFTA_START+PER_RING-1));
CRGBSet righta(leds(RIGHTA_START, RIGHTA_START+PER_RING-1));
CRGBSet leftb(leds(LEFTB_START, LEFTB_START+PER_RING-1));
CRGBSet rightb(leds(RIGHTB_START, RIGHTB_START+PER_RING-1));

bool pattern = true;

void setup() {
    // init
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    // set brightness
    FastLED.setBrightness(BRIGHT);
}

void loop() {
    fill_solid(lefta, NUM_LEDS, get_color(pattern));
    fill_solid(righta, NUM_LEDS, get_color(!pattern));
    fill_solid(leftb, NUM_LEDS, get_color(pattern));
    fill_solid(rightb, NUM_LEDS, get_color(!pattern));
    FastLED.show();
    delay(SPEED);
    pattern = !pattern;
}

CRGB get_color(bool color) {
  return color ? CRGB::Red : CRGB::Green;
}