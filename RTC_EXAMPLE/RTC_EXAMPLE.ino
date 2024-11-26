const int speakerPin = 8;  // 알람을 울릴 스피커 핀

unsigned long countdownTime = 10000;  // 10초 (10000ms)
unsigned long previousMillis = 0;  // 이전 시간 기록
int countdownSeconds = 10;  // 10초 카운트다운
bool alarmTriggered = false;  // 알람 발생 여부

// 노래 음계를 위한 주파수 (Hz)
int melody[] = { 523, 523, 0, 523, 523, 0 };  // C C 0 C C 0 (Beep Beep)  
int noteDurations[] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

void setup() {
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);  // 스피커 핀을 출력으로 설정
  Serial.println("알람시계 시작!");
}

void loop() {
  unsigned long currentMillis = millis();  // 경과 시간 확인

  // 카운트다운을 출력하고 10초가 되면 알람 울리기
  if (currentMillis - previousMillis >= 1000) {  // 1초마다 실행
    previousMillis = currentMillis;
    
    // 카운트다운을 1초씩 줄여가며 출력
    if (countdownSeconds > 0) {
      countdownSeconds--;
      Serial.print("남은 시간: ");
      Serial.println(countdownSeconds);
    } else if (!alarmTriggered) {  // 카운트다운이 0초가 되면 알람 울리기
      triggerAlarm();
      alarmTriggered = true;  // 알람이 울렸음을 기록
    }
  }
}

// 알람을 울리는 함수 (노래)
void triggerAlarm() {
  Serial.println("알람! 시간이 다 되었습니다!");
  
  // 노래를 연주
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];  // 음 길이에 맞는 시간 계산
    tone(speakerPin, melody[thisNote], noteDuration);  // 음 출력
    delay(noteDuration * 1.30);  // 음이 끝나고 잠깐 대기 (음과 음 사이의 간격)
    noTone(speakerPin);  // 음 종료
  }
}
