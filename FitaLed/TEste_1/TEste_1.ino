// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            31

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      50

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel pixelss you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
int n;
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  Serial.begin(9600);
  pinMode(sensorPin, INPUT);

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();

}

void loop() {

  PiscaMusica();

}

void PiscaMusica() {
 /* sensorValue = analogRead(sensorPin);
  Serial.print("Sensor = ");
  Serial.println(sensorValue);*/
  
  sensorValue = Leitura(sensorPin)*2;

  if(sensorValue <= 10)Blue(sensorValue);
  if(sensorValue > 10 and sensorValue <= 30)Green(sensorValue);
  if(sensorValue > 30)Red(sensorValue);
  

  pixels.show();
}

void Blue(int sensor){

  for (int i = 0; i < 50; i++) {
    pixels.setPixelColor(i, 0, sensorValue, 0);
  }
}

void Green(int sensor){

  for (int i = 0; i < 50; i++) {
    pixels.setPixelColor(i, 0, 0, sensorValue);
  }
}
void Red(int sensor){

  for (int i = 0; i < 50; i++) {
    pixels.setPixelColor(i, sensorValue, 0, 0);
  }
}

float Leitura(int analogInPin)
{
  float sensorValue = 0;
  float outputValue = 0;
  float ReadValue = 0;
  int ReadTimes = 0;
  for (int i = 0; i < 10; i++)
  {
    ReadValue = analogRead(analogInPin);
    Serial.println(ReadValue);
    if (ReadValue > 0)
    {
      sensorValue = sensorValue + ReadValue;
      ReadTimes++;
      //delay(1);
    }
  }
  outputValue = sensorValue / ReadTimes;
  return outputValue;
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void meioLado()
{
  n = 29;
  for (int i = 30; i < 50; i++) {



    pixels.setPixelColor(i, 0, 255, 0);
    pixels.setPixelColor(n, 0, 255, 0);

    pixels.show();
    n--;
    delay(100);

  }

  n = 10;
  for (int i = 50; i >= 20; i--) {



    pixels.setPixelColor(i, 0, 0, 0);
    pixels.setPixelColor(n, 0, 0, 0);

    pixels.show();
    n++;
    delay(100);

  }
}

