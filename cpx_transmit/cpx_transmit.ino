#include <Adafruit_CircuitPlayground.h>

void setup() {
	CircuitPlayground.begin();
	Serial1.begin(115200);
}

bool leftButtonPressed;
bool rightButtonPressed;
byte bytes[9];

int loopDelay = 50;

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
	byte lightValue = CircuitPlayground.lightSensor();
	leftButtonPressed = CircuitPlayground.leftButton();
	rightButtonPressed = CircuitPlayground.rightButton();
	byte xReading = scaleReading(CircuitPlayground.motionX(), 10, 20);
	byte yReading = scaleReading(CircuitPlayground.motionY(), 10, 20);
	byte zReading = scaleReading(CircuitPlayground.motionZ(), 10, 20);
	byte soundReading = scaleReading(CircuitPlayground.mic.soundPressureLevel(10), -50, 50);

	bytes[0] = 0xff;
	bytes[1] = lightValue;
	bytes[2] = leftButtonPressed;
	bytes[3] = rightButtonPressed;
	bytes[4] = xReading;
	bytes[5] = yReading;
	bytes[6] = zReading;
	bytes[7] = soundReading;

	Serial1.write(bytes, 9);
	delay(loopDelay);
}
