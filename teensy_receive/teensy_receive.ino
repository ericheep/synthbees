#define HWSERIAL Serial1

char bytes[7];

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(115200);
  Serial.print("hello");
}

void loop() {
  if (HWSERIAL.available()) {
    if (HWSERIAL.read() == 0xff) {
      HWSERIAL.readBytes(bytes, 7);

      byte lightReading = byte(bytes[0]);
      byte leftButtonPressed = byte(bytes[1]);
      byte rightButtonPressed = byte(bytes[2]);

      Serial.print("UART received: ");
      Serial.print(lightReading);
      Serial.print(", ");
      Serial.print(leftButtonPressed);
      Serial.print(", ");
      Serial.print(rightButtonPressed);
      Serial.print(", ");
      Serial.print(bytes[3], DEC);
      Serial.print(", ");
      Serial.print(bytes[4], DEC);
      Serial.print(", ");
      Serial.print(bytes[5], DEC);
      Serial.print(", ");
      Serial.println(bytes[6], DEC);

    }
  }
}
