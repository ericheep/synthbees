#define HWSERIAL Serial1
#define FILTER_SIZE 100
#define SMALL_FILTER_SIZE 30

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
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.5);
  noise1.amplitude(0.5);
}

void printReadings(byte light, byte x, byte y, byte z, byte sound, byte left, byte right, byte slide, byte temp, byte patch) {
  Serial.print("\tLIGHT: ");
  Serial.print(light, DEC);
  Serial.print("\tX: ");
  Serial.print(x, DEC);
  Serial.print("\tY: ");
  Serial.print(y, DEC);
  Serial.print("\tZ: ");
  Serial.print(z, DEC);
  Serial.print("\tSOUND: ");
  Serial.print(sound, DEC);
  Serial.print("\tLEFT: ");
  Serial.print(left, DEC);
  Serial.print("\tRIGHT: ");
  Serial.print(right, DEC);
  Serial.print("\tSLIDE: ");
  Serial.print(slide, DEC);
  Serial.print("\tTEMP: ");
  Serial.print(temp, DEC);
  Serial.print("\tPATCH: ");
  Serial.println(patch, DEC);
}

bool settleActive = false;
float cycle = 0.0;
float dPreviousReading = 0.0;
int settleEnd, settleTime, settleStart;
int xReadings[FILTER_SIZE];
int yReadings[FILTER_SIZE];
int zReadings[FILTER_SIZE];
int displacementXReadings[SMALL_FILTER_SIZE];
int displacementYReadings[SMALL_FILTER_SIZE];
int displacementZReadings[SMALL_FILTER_SIZE];

float averageReadings(byte val, int readings[], int filterSize) {
  readings[0] = val;

  int i;
  for (i = filterSize - 1; i > 0; i--) {
    readings[i] = readings[i - 1];
  }

  float sum = 0.0;
  for (i = 0; i < filterSize; i++) {
    sum += readings[i];
  }

  return sum / filterSize;
}

void loop() {
  if (HWSERIAL.available()) {
    if (HWSERIAL.read() == 0xff) {
      HWSERIAL.readBytes(bytes, 10);

      byte light = byte(bytes[0]);
      byte x = byte(bytes[1]);
      byte y = byte(bytes[2]);
      byte z = byte(bytes[3]);
      byte sound = byte(bytes[4]);
      byte left = byte(bytes[5]);
      byte right = byte(bytes[6]);
      byte slide = byte(bytes[7]);
      byte temp = byte(bytes[8]);
      byte patch = byte(bytes[9]);

      float xMean = averageReadings(x, xReadings, FILTER_SIZE);
      float yMean = averageReadings(y, yReadings, FILTER_SIZE);
      float zMean = averageReadings(z, zReadings, FILTER_SIZE);

      patch = 1;

      switch (patch) {
        case 1:
          patchOne(x, y, z, xMean, yMean, zMean);
          break;
        case 2:
          patchTwo();
          break;
        case 3:
          patchThree();
          break;
        case 4:
          patchFour();
          break;
        case 5:
          patchFive();
          break;
        default:
          break;
      }
    }
  }
}

void updateReadings(byte val, int readings[], int filterSize) {
  for (int i = filterSize - 1; i > 0; i--) {
    readings[i] = readings[i - 1];
  }
  readings[0] = val;
}

float meanSquaredDisplacement(int displacementReadings[], float ref, int readingsSize) {
  float sum = 0.0;
  for (int i = 0; i < readingsSize; i++) {
    sum += pow(displacementReadings[i] - ref, 2);
  }
  return sum / readingsSize;
}

float trixAxisDisplacement(byte x, byte y, byte z, float xRef, float yRef, float zRef) {
  float dX = meanSquaredDisplacement(displacementXReadings, xRef, SMALL_FILTER_SIZE);
  float dY = meanSquaredDisplacement(displacementYReadings, yRef, SMALL_FILTER_SIZE);
  // float dZ = meanSquaredDisplacement(displacementZReadings, zRef, SMALL_FILTER_SIZE);
  return (dX + dY) / 2;
}

void settleFrequency(int settleStart, int settleEnd,int settleTime, int maxSettleTime) {
  float scalar = (millis() - settleStart) / float(settleEnd - settleStart);
  float reverseScalar = 1.0 - scalar;
  float displacement = (settleEnd - settleStart) / float(maxSettleTime);
  
  float lfoAmplitude = pow(reverseScalar, 2) * 100 * displacement;
  float lfoFrequency = 0.5;
  float cycleIncrement = scalar;
  
  cycle = fmod(cycle + cycleIncrement, TWO_PI);  
  float lfo = sin(cycle) * lfoAmplitude;

  waveform1.frequency(880 + lfo); 
  waveform1.amplitude(0.5);
  noise1.amplitude(0.0);
}

int calculateSettleTime(float d, float dMin, int maxSettleTime) {
  float dMax = 40.0;
  if (d > dMax) {
    d = dMax;
  }

  float settleScalar = (d - dMin) / dMax;
  return (pow(settleScalar, 0.5) * maxSettleTime);
}

void patchOne(byte x, byte y, byte z, float xMean, float yMean, float zMean) {
  updateReadings(x, displacementXReadings, SMALL_FILTER_SIZE);
  updateReadings(y, displacementYReadings, SMALL_FILTER_SIZE);
  // updateReadings(z, displacementZReadings, SMALL_FILTER_SIZE);

  int maxSettleTime = 30000;

  // startup time
  if (millis() < 5000) {
    return;
  }

  float d = trixAxisDisplacement(x, y, z, xMean, yMean, zMean);
  float dThreshold = 1.00;

  if (d > dThreshold && !settleActive) {
    if (dPreviousReading > d) {
      settleTime = calculateSettleTime(dPreviousReading, dThreshold, maxSettleTime);
      settleActive = true;
      settleStart = millis();
      settleEnd = millis() + settleTime;
      Serial.print("settling\t");
      Serial.println(settleTime);
    }
    dPreviousReading = d;
  }

  if (settleActive) {
    settleFrequency(settleStart, settleEnd, settleTime, maxSettleTime);
    if (millis() > settleEnd) {
      settleActive = false;
      dPreviousReading = 0;
      cycle = 0;
      Serial.println("settled");
    }
  }


  /*float freq = (meanX / float(255) * 500 + 1000);

    float ph = 0.0;
    float sinFactor = 0.0;
    float sinPos = 0.0;
    if (sdX > 1.0) {
    sinPos += sdX;
    while (sinPos > 3.14159265359) {
      sinPos -= 3.14159265359;
    }
    sinFactor = sin(sinPos/3.14159265359);
    }?8
    /*Serial.print(sinFactor);
    Serial.print("/t ");
    Serial.print(freq);
    Serial.print("/t ");
    Serial.println(sdX);
    waveform1.frequency(freq); // scale so it can go above or below base frew
    // waveform1.amplitude(0.5);
    noise1.amplitude(0.0);*/
}

void patchTwo() {
  // waveform1.amplitude(0.5);
  // noise1.amplitude(0.0);
}

void patchThree() {

}

void patchFour() {

}

void patchFive() {

}
