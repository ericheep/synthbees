#define HWSERIAL Serial1

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthNoiseWhite     noise1;         //xy=178,261
AudioSynthWaveform       waveform1;      //xy=185,224
AudioMixer4              mixer1234;         //xy=339,243
AudioMixer4              mixer5678;         //xy=339,309
AudioMixer4              masterMixer;         //xy=501.4545593261719,275.45452880859375
AudioOutputAnalog        dac1;           //xy=641.0000610351562,275.0000305175781
AudioConnection          patchCord1(noise1, 0, mixer1234, 1);
AudioConnection          patchCord2(waveform1, 0, mixer1234, 0);
AudioConnection          patchCord3(mixer1234, 0, masterMixer, 0);
AudioConnection          patchCord4(mixer5678, 0, masterMixer, 1);
AudioConnection          patchCord5(masterMixer, dac1);
// GUItool: end automatically generated code

//number of bytes to read
char bytes[9];

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(115200);
  Serial.print("hello");
  AudioMemory(20);
  dac1.analogReference(EXTERNAL); // make it louder
  waveform1.begin(WAVEFORM_SAWTOOTH);
  waveform1.amplitude(0.5);
  noise1.amplitude(0.5);
}

void printReadings(byte light, byte x, byte y, byte z, byte sound, byte left, byte right, byte slide, byte patch) {
  Serial.print("LIGHT: ");
  Serial.print(light, DEC);
  Serial.print("X: ");
  Serial.print(x, DEC);
  Serial.print("Y: ");
  Serial.print(y, DEC);
  Serial.print("Z: ");
  Serial.print(z, DEC);
  Serial.print("SOUND: ");
  Serial.print(sound, DEC);
  Serial.print("LEFT: ");
  Serial.print(left, DEC);
  Serial.print("RIGHT: ");
  Serial.print(right, DEC);
  Serial.print("SLIDE: ");
  Serial.print(slide, DEC);
  Serial.print("PATCH: ");
  Serial.println(patch, DEC);
}

void loop() {
  if (HWSERIAL.available()) {
    if (HWSERIAL.read() == 0xff) {
      HWSERIAL.readBytes(bytes, 9);

      byte light = byte(bytes[0]);
      byte x = byte(bytes[1]);
      byte y = byte(bytes[2]);
      byte z = byte(bytes[3]);
      byte sound = byte(bytes[4]);
      byte left = byte(bytes[5]);
      byte right = byte(bytes[6]);
      byte slide = byte(bytes[7]);
      byte patch = byte(bytes[8]);

      printReadings(light, x, y, z, sound, left right, slide, patch);

      switch (patch) {
        case 1:
          patchOne(light);
        case 2:
          patchTwo();
        case 3:
          patchThree();
        case 4:
          patchFour();
        case 5:
          patchFive();
        default:
          //
      }
    }
  }
}

void patchOne(byte light) {
  int lightScaled;
  lightScaled = map(lightReading, 0, 255, 200, 250);
  waveform1.frequency(lightReadingScaled); // scale so it can go above or below base frew
  waveform1.amplitude(0.5);
  noise1.amplitude(0.5);
}

void patchTwo() {

}

void patchThree() {

}

void patchFour() {

}

void patchFive() {

}
}
