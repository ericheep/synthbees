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

      Serial.print("UART received: ");
      Serial.print(bytes[0], DEC);
      Serial.print(", ");
      Serial.print(bytes[1], DEC);
      Serial.print(", ");
      Serial.print(bytes[2], DEC);
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
