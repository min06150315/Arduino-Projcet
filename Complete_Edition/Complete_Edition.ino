#include <LiquidCrystal_I2C.h> // LCD 모듈을 위한 라이브러리
#include <Wire.h>              // LCD 모듈을 위한 라이브러리
#include <SoftwareSerial.h>    // Bluetooth 모듈을 위한 라이브러리
#include <Servo.h>             // Servo 모터를 위한 라이브러리
#include <Stepper.h>           // Stepper 모터를 위한 라이브러리

Servo myServo;

SoftwareSerial MyBluetooth(6, 7); // RX, TX

String receivedMessage = "";

LiquidCrystal_I2C lcd(0x3F, 16, 2);

const int stepsPerRevolution = 1024;
Stepper myStepper(stepsPerRevolution,0,1,2,12);

#define RED 3
#define GREEN 4
#define BLUE 5
#define trigPin 11
#define echoPin 10

const int speakerPin = 13;  // 알람을 울릴 스피커 핀

void setup() {
  setupByRGBLED();       // RGB LED 모듈 연결
  setupByLCDModule();       // LCD 디스플레이 모듈 연결
  setupByBluetoothModule(); // 블루투스 모듈 연결
  setupByTrash();
  Serial.begin(9600); // 시리얼 모니터 연결
}

void loop() {
  // 메인 기능 수행
  mainController();
}

// LCD 모듈 관련 초기 설정
void setupByLCDModule() {
  lcd.init();          // LCD 초기화
  lcd.backlight();     // 백라이트 켜기
  Serial.println("LCD Module Started"); // 시리얼 모니터에 확인용 메시지 출력
  // 시작 멘트 (LCD 디스플레이에 출력)
  printByLCD(0, "Hello, Students!");
  printByLCD(1, "Project Started!");
}

// LED 모듈 관련 초기 설정
void setupByRGBLED() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

void setupByBluetoothModule() {
  MyBluetooth.begin(9600);
  Serial.println("My Bluetooth Module Started"); // 시리얼 모니터에 확인용 메시지 출력
} 

void setupByTrash() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(9);
  myServo.write(0);
}

void setBySmartDoor() {
  myStepper.setSpeed(30);
  Serial.println("Stepper Motor Started");
}

void mainController() {
  while (MyBluetooth.available()) { // 블루투스 모듈이 사용 가능할 때 작동
    char incomingChar = MyBluetooth.read(); // 블루투스 모듈에서 글 읽어오기
    
    if (incomingChar == '\n' || incomingChar == '\r') {
      receivedMessage.trim();
      Serial.println(receivedMessage);
      
      if (receivedMessage == "LED ON" || receivedMessage == "LED OFF") {
        remoteLEDController(receivedMessage);  // LED 제어
        printByLCD(0, "LED Controller     ");
      } else if (receivedMessage.startsWith("Alarm")) {
        printByLCD(0, "Alarm Clock      ");
        // "Alarm" 뒤에 숫자 (초)를 추출하여 알람 시간을 설정
        int alarmTime = receivedMessage.substring(6).toInt();  // "Alarm " 이후 숫자 추출
        alarmClockController(alarmTime);
      } else if (receivedMessage == "Smart Door Open" || receivedMessage == "Smart Door Close") {
        smartDoorLockController(receivedMessage);  // 스마트 도어락 제어
        printByLCD(0, "Door Lock");
      } else if (receivedMessage.startsWith("Trash")) {
        remoteTrashController(receivedMessage);  // 스마트 쓰레기통 제어
        printByLCD(0, "Trash Controller");
      }

      delay(100);  // 딜레이 추가
      receivedMessage = "";
    } else {
      receivedMessage += incomingChar;
    }
  }
}

void printByLCD(int row, String str) {
  lcd.setCursor(0, row);
  lcd.print(str);
}

void printByLED(int R, int G, int B) {
  analogWrite(RED, R);
  analogWrite(GREEN, G);
  analogWrite(BLUE, B);
}

void remoteLEDController(String receivedMessage) {
  if (receivedMessage == "LED ON") {
    printByLED(255, 255, 255);
    printByLCD(1, "LED ON              ");
  } else if (receivedMessage == "LED OFF") {
    printByLED(0, 0, 0);
    printByLCD(1, "LED OFF              ");
  }
}

void alarmClockController(int time) {
  for (int i = time - 1; i > 0; i--) {
    printByLCD(1, String(i) + "s remaining       ");
    Serial.print(i);
    Serial.println("s remaining");
    delay(1000);
  }
  
  printByLCD(1, "ALARM TRIGGERED!        ");
  triggerAlarm();
}

void smartDoorLockController(String receivedMessage) {
  if (receivedMessage == "Smart Door Open") {
    myStepper.step(stepsPerRevolution);
    printByLCD(1, "Smart Door Open     ");
  } else if (receivedMessage == "Smart Door Close") {
    myStepper.step(-stepsPerRevolution);
    printByLCD(1, "Smart Door Close    ");
  }
}

void remoteTrashController(String receivedMessage) {
    if (receivedMessage == "Trash Open") {
      myServo.write(90);
      printByLCD(1, "Trash Open           ");
    } else if (receivedMessage == "Trash Close") {
      myServo.write(0);
      printByLCD(1, "Trash Close           ");
    }
    delay(1000);
}

// 초음파 센서를 사용해서 가리 구하기

// 알람 소리 실행
void triggerAlarm() {
  int melody[] = { 523, 523, 0, 523, 523, 0 };  // C C 0 C C 0 (Beep Beep)  
  int noteDurations[] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

  Serial.println("알람! 시간이 다 되었습니다!");
  
  // 노래를 연주
  for (int thisNote = 0; thisNote < 6; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];  // 음 길이에 맞는 시간 계산
    tone(speakerPin, melody[thisNote], noteDuration);  // 음 출력
    delay(noteDuration * 1.30);  // 음이 끝나고 잠깐 대기 (음과 음 사이의 간격)
    noTone(speakerPin);  // 음 종료
  }
}
