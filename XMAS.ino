#include <FastLED.h>

#define DATA_PIN 2
#define MOTION_PIN 7
#define SPEAKER_PIN 8
#define STATUS_LED 13

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
#define RIGHTB_START 48

// Speed in ms to alternate flash
#define SPEED 500

// Brightness
#define BRIGHT 10

// Tick period (in MS)
#define PERIOD 20

// Time in MS to stay in Action states
#define ACTION_TIMEOUT 14500
#define PATTERN_TIMEOUT 500

CRGBArray<NUM_LEDS> leds;

// shortcuts for each set
CRGBSet lefta(leds(LEFTA_START, LEFTA_START+PER_RING-1));
CRGBSet righta(leds(RIGHTA_START, RIGHTA_START+PER_RING-1));
CRGBSet leftb(leds(LEFTB_START, LEFTB_START+PER_RING-1));
CRGBSet rightb(leds(RIGHTB_START, RIGHTB_START+PER_RING-1));

enum States { WAIT, ACTION } state;

bool pattern = true;

// counters/timers
uint16_t pattern_count;
uint16_t action_count;

CRGB get_color(bool color) {
  return color ? CRGB::Red : CRGB::Green;
}

void init_sketch() {
  speaker_on(false);
  action_count = 0;
  pattern_count = 0;
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void speaker_on(bool enable) {
  // turn on relay pin 8
  if (enable) {
    digitalWrite(SPEAKER_PIN, HIGH);
    digitalWrite(STATUS_LED, HIGH);
  } else {
    digitalWrite(SPEAKER_PIN, LOW);
    digitalWrite(STATUS_LED, LOW);
  }
}

void tick() {
  // Transitions
  switch(state) {
    case WAIT:
      if (digitalRead(MOTION_PIN)) {
        speaker_on(true);
        state = ACTION;
      };
      break;
    case ACTION:
      if (action_count > (ACTION_TIMEOUT/PERIOD)) {
        // Action timed out, transition to wait
        init_sketch();
        state = WAIT;
      } else if (pattern_count > (PATTERN_TIMEOUT/PERIOD)) {
        // Pattern timed out, change pattern
        pattern = !pattern;
        pattern_count = 0;
      }
      break;
    default:
      state = WAIT;
      break;
  }
  // Actions
  switch(state) {
    case WAIT:
      break;  // noop
    case ACTION:
      fill_solid(lefta, NUM_LEDS, get_color(pattern));
      fill_solid(righta, NUM_LEDS, get_color(!pattern));
      fill_solid(leftb, NUM_LEDS, get_color(!pattern));
      fill_solid(rightb, NUM_LEDS, get_color(pattern));
      FastLED.show();

      // Advance counters
      action_count++;
      pattern_count++;
      break;
    default:
      break;  // noop
  }
}

void setup() {
    // init
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(MOTION_PIN, INPUT);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    // set brightness
    FastLED.setBrightness(BRIGHT);

    init_sketch();
}

void loop() {
    tick();
    delay(PERIOD);
}