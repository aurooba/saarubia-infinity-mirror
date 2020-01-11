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

const int COLOUR_TIME_LENGTH = 2000;
const int changeBy = 4;
int r = 1023;
int g = 0;
int b = 0;
int r2;
int g2;
int b2;

// function sends the colour to the LEDs
void doColour(int r, int g, int b) {
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

// function makes sure numbers never overshoot 255 and never go negative
int correctNumber(int number) {
  if (number >= 1023) {
    number = 1023;
  } else if (number <= 0) {
    number = 0;
  }
  return number;
}

// If current number is less than target number, increase current number by changeBy. 
// If current number is greater than target number, decrease current number by changeBy.
// If current number is less than or greater than target number by the value of changeBy, 
// make it so current number equals target number
int transitionNumber(int n, int n2) {
  if (n2 > n) {
    n = correctNumber(n + changeBy);
  } else if (n2 < n) {
    n = correctNumber(n - changeBy);
  }
  if ((n >= n2-changeBy) && (n <= n2+changeBy)) {
    n = n2;
  }
  return n;
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
  r2 = random(0, 1023);
  g2 = random(0, 1023);
  b2 = random(0, 1023);
  
  doColour(r, g, b);
  delay(COLOUR_TIME_LENGTH);

  do {

    r = transitionNumber(r, r2);
    g = transitionNumber(g, g2);
    b = transitionNumber(b, b2);

    doColour(r, g, b);

    Serial.println(r);
    Serial.println(g);
    Serial.println(b);

    delay(3);

  } while ( (r != r2) || (g != g2) || (b != b2) );

}

