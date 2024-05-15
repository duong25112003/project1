#include<Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RST_PIN         9
#define SS_PIN          10
int ls1=7;// limited switch 1 
int ls2=6; //limited switch 2 
int i1=2;
int i2=3;
int led = 8;
int S1=5; //Sensor 
int S2=4;

int UID[4], i;

int ID1[4] = {48, 104, 204, 20}; //Thẻ mở đèn
int ID2[4] = {206, 214, 185, 29}; //Thẻ tắt đèn

MFRC522 mfrc522(SS_PIN, RST_PIN);

void forward(){
 digitalWrite(i1,HIGH);
 digitalWrite(i2,LOW);

}
void goback(){
  digitalWrite(i1,LOW);
 digitalWrite(i2,HIGH);

}
void stop(){
  digitalWrite(i1,LOW);
 digitalWrite(i2,LOW);
}

void setup() 
{

    Serial.begin(9600);   
    pinMode(ls1,INPUT_PULLUP);
    pinMode(ls2,INPUT_PULLUP);
    pinMode(i1,OUTPUT);
    pinMode(i2,OUTPUT);
    pinMode(S1,INPUT);
    pinMode(S2,INPUT);

    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    lcd.init();
    lcd.backlight();
    
    SPI.begin();    
    mfrc522.PCD_Init();

}

void loop() 
{
  int rS1 = digitalRead(S1);  // Đọc trạng thái của cảm biến S1
  int rS2 = digitalRead(S2);  // Đọc trạng thái của cảm biến S2
  int rls1 = digitalRead(ls1); // Đọc trạng thái của công tắc giới hạn ls1
  int rls2 = digitalRead(ls2); // Đọc trạng thái của công tắc giới hạn ls2

  if (rls1 == 0 || rls2 == 0)
  {
    stop(); // Dừng hệ thống nếu có công tắc giới hạn được nhấn
  }

  if (!mfrc522.PICC_IsNewCardPresent()) 
  { 
    return;  // Nếu không có thẻ mới, thoát khỏi hàm loop()
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) 
  {  
    return;  // Nếu không đọc được thẻ, thoát khỏi hàm loop()
  }
  
  Serial.print("UID của thẻ: ");    
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  { 
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");   
    UID[i] = mfrc522.uid.uidByte[i];
    Serial.print(UID[i]);
  }

  Serial.println("   ");

  // So sánh UID đọc được với các ID đã định nghĩa
  if (UID[i] == ID1[i])
  {
    digitalWrite(led, HIGH);  // Bật đèn LED
    forward();  // Điều khiển điều hướng tiến
    Serial.println("Thẻ mở đèn - ĐÈN ON");
    lcd.setCursor(1,0);
    lcd.print("Moi vao");  // Hiển thị trên LCD khi thẻ mở đèn được đọc
  }
  else if (UID[i] == ID2[i] && rS1 == 0 && rS2 == 0)
  {
    digitalWrite(led, LOW);  // Tắt đèn LED
    goback();  // Điều khiển điều hướng lùi
    Serial.println("Thẻ tắt đèn - ĐÈN OFF");
    lcd.setCursor(1,0);
    lcd.print("Dong cua");  // Hiển thị trên LCD khi thẻ tắt đèn được đọc
  }
  else
  {
    Serial.println("Sai thẻ");
    lcd.setCursor(1,0);
    lcd.print("Sai the");  // Hiển thị thông báo sai thẻ trên LCD
  }
  
  mfrc522.PICC_HaltA();  
  mfrc522.PCD_StopCrypto1();

  Serial.println(rls2);    
  Serial.println(rls1); 

  lcd.setCursor(0, 1);  // Đặt con trỏ LCD ở hàng 2
  
  String message = "TRAN HUY DUONG - NGUYEN DANG THANH - VU THE SON  ";
  
  for (int pos = 0; pos <= message.length() - 16; pos++) {
    lcd.clear();  // Xóa màn hình LCD
    lcd.setCursor(0, 1);  // Đặt con trỏ LCD ở hàng 2
    lcd.print(message.substring(pos, pos + 16));  // In ra màn hình 16 ký tự của chuỗi
    delay(500);  // Chờ 0.5 giây trước khi dịch chuyển vị trí in
  }
}
