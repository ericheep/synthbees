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
byte bytes[9];

int loopDelay = 0;

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
  
	byte lightReading = CircuitPlayground.lightSensor();
	byte xReading = scaleReading(CircuitPlayground.motionX(), 10, 20);
	byte yReading = scaleReading(CircuitPlayground.motionY(), 10, 20);
	byte zReading = scaleReading(CircuitPlayground.motionZ(), 10, 20);
	byte soundReading = scaleReading(CircuitPlayground.mic.soundPressureLevel(10), -50, 50);
  leftButton = CircuitPlayground.leftButton();
  rightButton = CircuitPlayground.rightButton();
  slideSwitch = CircuitPlayground.slideSwitch();

	bytes[0] = 0xff;
	bytes[1] = lightReading;
	bytes[2] = xReading;
	bytes[3] = yReading;
	bytes[4] = zReading;
	bytes[5] = soundReading;
  bytes[6] = leftButton;
  bytes[7] = rightButton;
  bytes[8] = slideSwitch;
  bytes[9] = patchNum;
	Serial1.write(bytes, 10);

 buttonState = leftButton;
  
  // check if the button is pressed or released
  // by comparing the buttonState to its previous state 
  if (buttonState != lastButtonState) {
    
    // change the state of the led when someone pressed the button
  if (slideSwitch == 1 && buttonState == 1) { 
      patchNum++; 
      Serial.print("patchNum");
      Serial.println(patchNum);
      if(patchNum > 9) {(patchNum=0);}      
}
    
    // remember the current state of the button
    lastButtonState = buttonState;
  }
  
  
if (patchNum == 1 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 0, 255, 0);
}
   
else if (patchNum == 2 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 0, 255,   0);
}

else if (patchNum == 3 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 255, 0, 0);
CircuitPlayground.setPixelColor(2, 0, 255, 0);
}

else if (patchNum == 4 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 255, 0, 0);
CircuitPlayground.setPixelColor(2, 255, 0, 0);
CircuitPlayground.setPixelColor(3, 0, 255, 0);
}

else if (patchNum == 5 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 255, 0, 0);
CircuitPlayground.setPixelColor(2, 255, 0, 0);
CircuitPlayground.setPixelColor(3, 255, 0, 0);
CircuitPlayground.setPixelColor(4, 0, 255, 0);
}

else if (patchNum == 6 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 255, 0, 0);
CircuitPlayground.setPixelColor(2, 255, 0, 0);
CircuitPlayground.setPixelColor(3, 255, 0, 0);
CircuitPlayground.setPixelColor(4, 255, 0, 0);
CircuitPlayground.setPixelColor(5, 0, 255, 0);
}

else if (patchNum == 7 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 255, 0, 0);
CircuitPlayground.setPixelColor(2, 255, 0, 0);
CircuitPlayground.setPixelColor(3, 255, 0, 0);
CircuitPlayground.setPixelColor(4, 255, 0, 0);
CircuitPlayground.setPixelColor(5, 255, 0, 0);
CircuitPlayground.setPixelColor(6, 0, 255, 0);
}

else if (patchNum == 8 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 255, 0, 0);
CircuitPlayground.setPixelColor(2, 255, 0, 0);
CircuitPlayground.setPixelColor(3, 255, 0, 0);
CircuitPlayground.setPixelColor(4, 255, 0, 0);
CircuitPlayground.setPixelColor(5, 255, 0, 0);
CircuitPlayground.setPixelColor(6, 255, 0, 0);
CircuitPlayground.setPixelColor(7, 0, 255, 0);
}

else if (patchNum == 9 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 255, 0, 0);
CircuitPlayground.setPixelColor(2, 255, 0, 0);
CircuitPlayground.setPixelColor(3, 255, 0, 0);
CircuitPlayground.setPixelColor(4, 255, 0, 0);
CircuitPlayground.setPixelColor(5, 255, 0, 0);
CircuitPlayground.setPixelColor(6, 255, 0, 0);
CircuitPlayground.setPixelColor(7, 255, 0, 0);
CircuitPlayground.setPixelColor(8, 0, 255, 0);
}


else if (patchNum == 10 && slideSwitch == 1) {
CircuitPlayground.clearPixels();
CircuitPlayground.setPixelColor(0, 255, 0, 0);
CircuitPlayground.setPixelColor(1, 255, 0, 0);
CircuitPlayground.setPixelColor(2, 255, 0, 0);
CircuitPlayground.setPixelColor(3, 255, 0, 0);
CircuitPlayground.setPixelColor(4, 255, 0, 0);
CircuitPlayground.setPixelColor(5, 255, 0, 0);
CircuitPlayground.setPixelColor(6, 255, 0, 0);
CircuitPlayground.setPixelColor(7, 255, 0, 0);
CircuitPlayground.setPixelColor(8, 255, 0, 0);
CircuitPlayground.setPixelColor(9, 0, 255, 0);
}

if (slideSwitch == 0) {
  CircuitPlayground.clearPixels();
CircuitPlayground.setBrightness(1);
  CircuitPlayground.setPixelColor(0, xReading * 5,  100, 100);
    CircuitPlayground.setPixelColor(1, xReading * 5,  100, 100);
    CircuitPlayground.setPixelColor(2, xReading,  yReading * 5,  0);
        CircuitPlayground.setPixelColor(3, 100,  yReading * 5,  0);
      CircuitPlayground.setPixelColor(4, 100,  100,  zReading);
          CircuitPlayground.setPixelColor(5, 0,  soundReading,  0);
           CircuitPlayground.setPixelColor(6, 0, soundReading, 0);
               CircuitPlayground.setPixelColor(7, lightReading, 0,  50);
                CircuitPlayground.setPixelColor(8, lightReading,  0,  50);
                    CircuitPlayground.setPixelColor(9, 125,  125,  255);

};
	delay(loopDelay);
}
