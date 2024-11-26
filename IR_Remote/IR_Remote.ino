#include <Arduino.h>
#include <IRremote.hpp> // 최신 IRremote 라이브러리 포함

int RECV_PIN = A0;      // IR 리모컨 수신 핀 설정
int led1 = 13;          // 첫 번째 LED 핀 번호
int led2 = 12;          // 두 번째 LED 핀 번호
int led3 = 11;          // 세 번째 LED 핀 번호
int led4 = 10;          // 네 번째 LED 핀 번호

void setup() {
    Serial.begin(9600);          // 시리얼 모니터 초기화
    while (!Serial);               // 시리얼 통신이 준비될 때까지 대기
    Serial.println("IR Receiver initialized");

    IrReceiver.begin(RECV_PIN); // 수신기 초기화 (ENABLE_LED_FEEDBACK 제거)

    // LED 핀을 출력 모드로 설정
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);

    // 모든 LED를 처음에 꺼짐 상태로 설정
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
}

void loop() {
    // IR 신호가 수신되었는지 확인하고, 있다면 디코딩 시도
    if (IrReceiver.decode()) {
        uint32_t receivedCode = IrReceiver.decodedIRData.command; // 수신된 코드 저장

        // 수신된 코드 출력
        Serial.print("Received IR code: ");
        Serial.println(receivedCode, DEC);

        // 수신된 코드에 따라 LED 상태 변경
        switch (receivedCode) {
            case 0xFF9867: digitalWrite(led1, HIGH); break;  // LED1 켜기
            case 0xFFA25D: digitalWrite(led1, LOW); break;   // LED1 끄기
            case 0xFF629D: digitalWrite(led2, HIGH); break;  // LED2 켜기
            case 0xFFE21D: digitalWrite(led2, LOW); break;   // LED2 끄기
            case 0xFF22DD: digitalWrite(led3, HIGH); break;  // LED3 켜기
            case 0xFF02FD: digitalWrite(led3, LOW); break;   // LED3 끄기
            case 0xFFC23D: digitalWrite(led4, HIGH); break;  // LED4 켜기
            case 0xFFE01F: digitalWrite(led4, LOW); break;   // LED4 끄기
        }
        
        // 다음 신호 수신을 위해 IR 리모컨 리셋
        IrReceiver.resume();
    }
}
