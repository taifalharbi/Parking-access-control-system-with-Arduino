#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo myservo ;
LiquidCrystal_I2C lcd (0x27,2,1,0,4,5,6,7,3,POSITIVE);

#define vermelho 4
#define verde 5
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522 (SS_PIN, RST_PIN);

void setup() {
 
  Wire.begin();
  lcd.begin(16,2);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Approach your   ");
  lcd.setCursor(0,1);
  lcd.print("reader tag ");
  
  pinMode(verde,OUTPUT);
  pinMode(vermelho,OUTPUT);
  
  Serial.begin(9600);
  SPI.begin();
  Serial.println("Bring the tag closer to the reader");
  
  mfrc522.PCD_Init(); 
  
  digitalWrite(verde,0);
  digitalWrite(vermelho,0);
  
  myservo.attach(6);
  
}

void loop() 
{
  
  if(!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  if(!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print(" Tag UID : ");
  
  String contents = "";
  
  byte letra;
  
  for (byte i = 0; i< mfrc522.uid.size; i++)
  {
   Serial.print(mfrc522.uid.uidByte[i] <0x10 ? "0":" ");
   Serial.print(mfrc522.uid.uidByte[i], HEX);
   
   contents.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
   contents.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  Serial.print("Message : ");
  
  contents.toUpperCase();
  
  if(contents.substring(1) == "14 45 29 57")
  {
    lcd.clear();
    Serial.println("access released ");
  
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print("Ola !");
    lcd.setCursor(0,1);
    lcd.print("access released");
    
    digitalWrite(verde,1);
    digitalWrite(vermelho,0);
    
    myservo.write(95);
    delay(800);
    myservo.write(10);
    digitalWrite(verde,0);
    digitalWrite(vermelho,1);
  }
  
 if(contents.substring(1) == "F1 B1 03 1F")
  {
    
    lcd.clear();
    Serial.println("Access denied ");
    digitalWrite(verde,0);
    digitalWrite(vermelho,1);
    
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print("Mistake tag number");
    lcd.setCursor(0,1);
    lcd.print("authorized ");
    
  }
   
   delay(1000);
   lcd.clear();
    
   lcd.setBacklight(HIGH);
   lcd.setCursor(0,0);
   lcd.print("Approach your  ");
   lcd.setCursor(0,1);
   lcd.print("reader tag ");
}
