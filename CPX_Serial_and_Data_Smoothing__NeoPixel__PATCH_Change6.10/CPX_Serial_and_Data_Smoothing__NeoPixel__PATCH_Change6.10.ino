///tried adding in averaging
#include <Adafruit_CircuitPlayground.h>

//const int numReadings = 20;
//int xReadings[numReadings];      // the readings from the analog input, added x b/c there was a "readings" in Heep's scaling code
//int readIndex = 0;              // the index of the current reading
//int total = 0;                  // the running total
//int averageLight = 0;                // the average

//for button counter
int buttonState      = 0;     // current state of the button
int lastButtonState  = 0;     // previous state of the button
int ledState         = 0;     // remember current led state
int patchNum = 1;

void setup() {
  CircuitPlayground.begin();
  Serial1.begin(115200);
  //    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
  //    xReadings[thisReading] = 0;
  //    }
}

bool leftButton;
bool rightButton;
bool slideSwitch;
byte bytes[10];

int loopDelay = 10;

byte scaleReading(float reading, int offset, int divide) {
  byte byteReading = byte((reading + offset) / divide * 0xff);

  if (byteReading <= 0) {
    return 0;
  }
  if (byteReading > 254) {
    return 254;
  }
  return byteReading;

}

void ledReadings(byte x, byte y, byte z, byte sound, byte light) {
    CircuitPlayground.clearPixels();
    CircuitPlayground.setBrightness(1);
    CircuitPlayground.setPixelColor(0, x * 5,  100, 100);
    CircuitPlayground.setPixelColor(1, x * 5,  100, 100);
    CircuitPlayground.setPixelColor(2, x,  y * 5,  0);
    CircuitPlayground.setPixelColor(3, 100,  y * 5,  0);
    CircuitPlayground.setPixelColor(4, 100,  100,  z);
    CircuitPlayground.setPixelColor(5, 0,  sound,  0);
    CircuitPlayground.setPixelColor(6, 0, sound, 0);
    CircuitPlayground.setPixelColor(7, light, 0,  50);
    CircuitPlayground.setPixelColor(8, light,  0,  50);
    CircuitPlayground.setPixelColor(9, 125,  125,  255);
}

void loop() {

  //
  // // subtract the last reading:
  //  total = total - xReadings[readIndex];
  //  // read from the sensor:
  //  xReadings[readIndex] = CircuitPlayground.lightSensor();
  //  // add the reading to the total:
  //  total = total + xReadings[readIndex];
  //  // advance to the next position in the array:
  //  readIndex = readIndex + 1;
  //
  //  // if we're at the end of the array...
  //  if (readIndex >= numReadings) {
  //    // ...wrap around to the beginning:
  //    readIndex = 0;
  //  }
  //
  //  // calculate the average:
  //  averageLight = total / numReadings;

  byte light = CircuitPlayground.lightSensor();
  byte x = scaleReading(CircuitPlayground.motionX(), 10, 20);
  byte y = scaleReading(CircuitPlayground.motionY(), 10, 20);
  byte z = scaleReading(CircuitPlayground.motionZ(), 10, 20);
  byte sound = scaleReading(CircuitPlayground.mic.soundPressureLevel(10), -50, 50);
  byte temp = scaleReading(CircuitPlayground.temperatureF(), 60, 100);
  leftButton = CircuitPlayground.leftButton();
  rightButton = CircuitPlayground.rightButton();
  slideSwitch = CircuitPlayground.slideSwitch();

  bytes[0] = 0xff;
  bytes[1] = light;
  bytes[2] = x;
  bytes[3] = y;
  bytes[4] = z;
  bytes[5] = sound;
  bytes[6] = leftButton;
  bytes[7] = rightButton;
  bytes[8] = slideSwitch;
  bytes[9] = temp;
  bytes[10] = patchNum;
  Serial1.write(bytes, 11);

  buttonState = leftButton;

  // check if the button is pressed or released
  // by comparing the buttonState to its previous state
  if (buttonState != lastButtonState) {

    // change the state of the led when someone pressed the button
    if (slideSwitch == 1 && buttonState == 1) {
      patchNum++;
      if (patchNum > 9) {
        (patchNum = 0);
      }
    }

    // remember the current state of the button
    lastButtonState = buttonState;
  }

  if (slideSwitch == 1) {
    CircuitPlayground.clearPixels();
    for (int i = 0; i < 9; i++) {
      if (i < patchNum) {
        CircuitPlayground.setPixelColor(i, 255, 0, 0);
      }
    }
    CircuitPlayground.setPixelColor(patchNum, 0, 255, 0);
  }
  
  if (slideSwitch == 0) {
    ledReadings(x, y, z, sound, light);
  }
  
  delay(loopDelay);
}
