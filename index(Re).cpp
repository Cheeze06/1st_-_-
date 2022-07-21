#include <MFRC522.h> // NFC
#include <LiquidCrystal_I2C.h> // LCD, liquidcrystal_i2c 라이브러리 설치하기
#include <Wire.h> // LCD
#include <Servo.h> // 서보 모터

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);  // rfid 객체 생성

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 서보 모터
int servoPin = 3;
Servo servo; 
int angle = 0; // servo position in degrees 

// 아이스아메리카노, 라떼 함수
int iceame = 0;
int latte = 0;

void setup()
{ // RFID 설정
  Serial.begin(9600);   // 시리얼 통신 시작
  SPI.begin();             // SPI 통신 시작
  rfid.PCD_Init();         // rfid(MFRC522) 초기화
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  // LCD 설정
  lcd.init(); //LCD
  lcd.backlight(); //LCD 백라이트 켜짐

  // 서보 모터 설정
  servo.attach(servoPin);

  //버튼 설정
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(6) == LOW) {
  Serial.println("ame");
  iceame += 1;
  Serial.println(iceame);
  lcd.setCursor(0, 0);
  lcd.print("Ice americano");
  lcd.setCursor(0, 1);
  lcd.print(iceame);
  delay(1000);
  lcd.clear();
 }
 if (digitalRead(7) == LOW) {
  Serial.println("latte");
  latte += 1;
  Serial.println(latte);
  lcd.setCursor(0, 0);
  lcd.print("Cafe Latte");
  lcd.setCursor(0, 1);
  lcd.print(latte);
  delay(1000);
  lcd.clear();
 }

  // 새 카드 접촉이 없으면 돌아감
  if ( ! rfid.PICC_IsNewCardPresent()) {
    delay(500);  // 0.5초 대기
    return;
  }
  // 카드 태그를 읽지 못했으면 돌아감
  if ( ! rfid.PICC_ReadCardSerial()) {
    delay(500);    // 읽지 못했으면 0.5초 대기
    lcd.setCursor(0, 0);
    lcd.print("Access Denied.  ");
    lcd.setCursor(0, 0);
    lcd.print("please wait     ");
    delay(1000);  // 카드 인증 실패 시 1초 대기
    return;
  }
  // UID 값을 16진 값으로 읽어서 시리얼 모니터에 표시함
  Serial.print("UID tag :");
  String content= "";
  byte letter;
 
  for (byte i = 0; i < rfid.uid.size; i++)
  {
     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(rfid.uid.uidByte[i], HEX);
     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));
  }
 
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
 
  if (content.substring(1)="A0 CB 62 80" || content.substring(1)=="D6 20 C3 C7" )// 승인할 UID들 비교
  {
   // 인증 성공
    Serial.println("Authorized access");
    Serial.println();

    //카드값 - (아아 + 라떼) 값 계산, 보여주기

  }
  else { // 인증 실패하면...  
    Serial.println(" Access denied"); // 그외 UID는 승인 거부 표시
    lcd.setCursor(0, 0);
    lcd.print("Access Denied.");
    lcd.setCursor(0, 1);
    lcd.print("please wait");
    delay(1000);  // 카드 인증 실패 시 1초 대기
  }
}   
