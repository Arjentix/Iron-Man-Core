/**
 * 2020 Polyakov Daniil
 */

#include <FastLED.h>

#define DATA_PIN            5
#define NUM_LEDS            24
#define MAX_POWER_MILLIAMPS 500
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define NUM_LINES           3
#define MIN_DELAY           20
#define MAX_DELAY           150
#define SPEEDUP_HOLDING     40

CRGB leds[NUM_LEDS];


class Line {
public:
  Line(int start, unsigned length) : start_(Round(start)), length_(length) {
    for (int i = start_; i < start_ + length; ++i) {
      leds[Round(i)] = CRGB::White;
    }
  };

  void Move(int step = 1) {
    int end = start_ + length_ - 1;
    if (step >= 0) {
      for (int i = end; i >= start_; --i) {
        leds[Round(i + step)] = leds[Round(i)];
        leds[Round(i)] = CRGB::Black;
      }
    }
    else {
      for (int i = start_; i <= end; ++i) {
        leds[Round(i + step)] = leds[Round(i)];
        leds[Round(i)] = CRGB::Black;
      }
    }
    start_ = Round(start_ + step);
  }

private:
  int start_;
  unsigned length_;

  int Round(int n) {
    if (n < 0) {
      n = NUM_LEDS + n;
    }

    return n % NUM_LEDS;
  }
};


Line lines[NUM_LINES] = {Line(0, 4), Line(8, 4), Line(16, 4)};

void SpeedUp(int delay_start, int delay_end, int step) {
  int counter = 0;

  for (int d = delay_start; d > delay_end; --d){
    if (d == delay_end + 1 && counter < SPEEDUP_HOLDING) {
      ++d;
      ++counter;
    }
    for (int j = 0; j < NUM_LINES; ++j) {
      lines[j].Move(step);
    }
    delay(d);
    FastLED.show();
  }
}

void SlowDown(int delay_start, int delay_end, int step) {
  int counter = 0;

  for (int d = delay_start; d < delay_end; ++d){
    for (int j = 0; j < NUM_LINES; ++j) {
      lines[j].Move(step);
    }
    delay(d);
    FastLED.show();
  }
}

void Spin(int step) {
  SpeedUp(MAX_DELAY, MIN_DELAY, step);
  SlowDown(MIN_DELAY, MAX_DELAY, step);
  delay(300);
}

void setup() {
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
}

void loop()
{
  Spin(1);
  Spin(-1);
}
