/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13
#define RED D5
#define GREEN D6
#define BLUE D8

const int COLOUR_TIME_LENGTH = 3000;
float r = 255;
float g = 0;
float b = 0;
float r2;
float g2;
float b2;

// function converts a regular RGB number to one in the 0 - 1023 range (roughly)
int rgbConverter(float number) {
  int result;
  result = 1023 - (number * 4);
  return result;
}

// function sends the colour to the LEDs
void doColour(float r, float g, float b) {
  analogWrite(RED, rgbConverter(r));
  analogWrite(GREEN, rgbConverter(g));
  analogWrite(BLUE, rgbConverter(b));
}

// function makes sure numbers never overshoot 255 and never go negative
float correctNumber(float number) {
  if (number >= 255) {
    number = 255;
  } else if (number <= 0) {
    number = 0;
  }
  return number;
}

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Serial.begin(9600);

}

void loop()
{

  // Target colour
  r2 = round(random(0, 255));
  g2 = round(random(0, 255));
  b2 = round(random(0, 255));
  
  doColour(r, g, b);
  delay(COLOUR_TIME_LENGTH);

  // for(int counter = g; counter <= g2;counter++) {
  do {
    float changeBy = 0.5;
    if (r2 > r) {
      r = correctNumber(r + changeBy);
    } else if (r2 < r) {
      r = correctNumber(r - changeBy);
    }

    if (g2 > g) {
      g = correctNumber(g + changeBy);
    } else if (g2 < g) {
      g = correctNumber(g - changeBy);
    }

    if (b2 > b) {
      b = correctNumber(b + changeBy);
    } else if (b2 < b) {
      b = correctNumber(b - changeBy);
    }
    doColour(r, g, b);
    // Serial.println(r);
    // Serial.println(g);
    // Serial.println(b);
    delay(3);
  } while ( (r != r2) || (g != g2) || (b != b2) );

}

