#include <SoftwareSerial.h> // BlueTooth 모듈을 위한 라이브러리

SoftwareSerial MyBluetooth(6, 7); // RX, TX

String receivedMessage = "";

#define RedPin 13
#define YellowPin 12
#define GreenPin 11
#define BluePin 10

void setup() {
  pinMode(RedPin, OUTPUT); // RedPin
  pinMode(YellowPin, OUTPUT); // YellowPin
  pinMode(GreenPin, OUTPUT); // GreenPin
  pinMode(BluePin, OUTPUT); // BluePin

  Serial.begin(9600);
  MyBluetooth.begin(9600);
  Serial.println("My Bluetooth Module Started");
}

void loop() {
  while (MyBluetooth.available()) {
    char incomingChar = MyBluetooth.read();
    
    if (incomingChar == '\n' || incomingChar == '\r') {  // \r 추가하여 처리
      receivedMessage.trim();
      Serial.print("Received from MyBluetooth: ");
      Serial.println(receivedMessage);
      if (receivedMessage == "LED ON") {  // .trim() 추가하여 공백 제거
        digitalWrite(RedPin, HIGH);
        digitalWrite(YellowPin, HIGH);
        digitalWrite(GreenPin, HIGH);
        digitalWrite(BluePin, HIGH);
      } else if (receivedMessage == "LED OFF") {
        digitalWrite(RedPin, LOW);
        digitalWrite(YellowPin, LOW);
        digitalWrite(GreenPin, LOW);
        digitalWrite(BluePin, LOW);
      }
      receivedMessage = "";
    } else {
      receivedMessage += incomingChar;
    }
  }
}
