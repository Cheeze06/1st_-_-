#include <MFRC522.h> // NFC
#include <LiquidCrystal_I2C.h> // LCD, liquidcrystal_i2c 라이브러리 설치하기
#include <Wire.h> // LCD
#include <SoftwareSerial.h> // 블루투스

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);  // rfid 객체 생성

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// BlueTooth
SoftwareSerial BT(2, 3);

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
  
  // BlueTooth 설정
  BT.begin(9600);
}

void loop()
{
  int ice_ame = 0;
  int latte = 0;

  // 새 카드 접촉이 없으면 돌아감
  if ( ! rfid.PICC_IsNewCardPresent()) {
    delay(500);  // 0.5초 대기
    return;
  }
  // 카드 태그를 읽지 못했으면 돌아감
  if ( ! rfid.PICC_ReadCardSerial()) {
    delay(500);    // 읽지 못했으면 0.5초 대기
    lcd.setCursor(0, 0);
    lcd.print('Access Denied.  ');
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print('please wait     ');
    lcd.setCursor(0, 1);
    lcd.print('person in charge');
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
    // 아이스 아메리카노 구매 갯수 구함
    lcd.setCursor(0, 0);
    lcd.print("Ice Americano   ");
    int ice_ame = BT.read();
    if (BT.available()) {
        Serial.write(BT.read());
    }
    lcd.setCursor(0, 1);
    lcd.print('x' + ice_ame);
    delay(2000);
    // 카페라떼 구매 갯수 구함
    lcd.setCursor(0, 0);
    lcd.print("Cafe Latte      ");
    int latte = BT.read();
    if (BT.available()) {
        Serial.write(BT.read());
    }
    lcd.setCursor(0, 1);
    lcd.print("x" + latte);
    delay(2000);
     // 계산
    int ice_ame_prise = ice_ame * 3000;
    int latte_prise = latte * 4000;
    lcd.setCursor(0, 0);
    lcd.print('ice ame:' + ice_ame + '=' + ice_ame_prise);
    lcd.setCursor(0, 1);
    lcd.print('latte:' + latte + '=' + latte_prise);
    delay(2000);
    lcd.clear();
  }
  else { // 인증 실패하면...  
    Serial.println(" Access denied"); // 그외 UID는 승인 거부 표시
    lcd.setCursor(0, 0);
    lcd.print('Access Denied.  ');
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print('please wait     ');
    lcd.setCursor(0, 1);
    lcd.print('person in charge');
    delay(1000);  // 카드 인증 실패 시 1초 대기
  }
}
