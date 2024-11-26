#include <Servo.h>

Servo myServo;

#define trigPin 11
#define echoPin 10
 
// 거리 임시 저장소
int temp_distance = 0;

void setup() {
  // 초기 설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  myServo.attach(9);
  myServo.write(0);
  delay(1000);
}

void loop() {
  // 거리 구하기
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration * 0.034) / 2;

  // 거리 시리얼 모니터에 띄워주기
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 거리가 가깝다면 (5cm 이하) 모터 돌리기 (뚜껑 열기)
  if (distance < 5) {
    myServo.write(90);
  } else {
    myServo.write(0);
  }
  delay(1000);
}
