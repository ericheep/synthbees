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

void loop() {
  if (HWSERIAL.available()) {
    if (HWSERIAL.read() == 0xff) {
      HWSERIAL.readBytes(bytes, 9);

      byte lightReading = byte(bytes[0]);
      byte xReading = byte(bytes[1]);
      byte yReading = byte(bytes[2]);
      byte zReading = byte(bytes[3]);
      byte soundReading = byte(bytes[4]);
      byte leftButton = byte(bytes[5]);
      byte rightButton = byte(bytes[6]);
      byte slideSwitch = byte(bytes[7]);
      byte patchNum = byte(bytes[8]);

      //not sure if we still need the DEC after the named sensor readings....
      Serial.print("UART received: ");
      Serial.print(lightReading, DEC);
      Serial.print(", ");
      Serial.print(xReading, DEC);
      Serial.print(", ");
      Serial.print(yReading, DEC);
      Serial.print(", ");
      Serial.print(zReading, DEC);
      Serial.print(", ");
      Serial.print(soundReading, DEC);
      Serial.print(", ");
      Serial.print(leftButton, DEC);
      Serial.print(", ");
      Serial.print(rightButton, DEC);
      Serial.print(", ");
      Serial.print(slideSwitch, DEC);
      Serial.print(", ");
      Serial.println(patchNum, DEC);

      // insert audio stuff here
      int lightReadingScaled;
      lightReadingScaled = map(lightReading, 0, 255, 200, 250);
      waveform1.frequency(lightReadingScaled); // scale so it can go above or below base frew
      waveform1.amplitude(0.5);
      noise1.amplitude(0.5);

      if (patchNum == 1) {
        mixer1234.gain(0, 1);
        mixer1234.gain(1, 0);
      };

      if (patchNum == 2) {
        mixer1234.gain(1, 1);
        mixer1234.gain(0, 0);
      };

      if (patchNum >= 3) {
        mixer1234.gain(1, 0);
        mixer1234.gain(0, 0);
      };

      // could start with 1, pass it, bring more in
    }
  }
}
