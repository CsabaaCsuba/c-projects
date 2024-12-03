#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

byte readCard[4];
String MasterTag = "137485FD";
String tagID = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{

  Serial.begin(9600);
  SPI.begin();

  mfrc522.PCD_Init();
  delay(4);

  mfrc522.PCD_DumpVersionToSerial();

  Serial.println("--------------------------");
  Serial.println("Scan Your Card>>");

}

void loop() 
{

  while (getID()) {

    if (tagID == MasterTag){
      Serial.println(" Access Granted!");
      Serial.println("--------------------------");

    }

    else{
      Serial.println(" Access Denied!");
      Serial.println("--------------------------");
    }

    delay(2000);
    Serial.println("--------------------------");
    Serial.println("Scan Your Card>>");
  }

}

boolean getID() 
{

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
  return false;
  }

  tagID = "";

  for ( uint8_t i = 0; i < 4; i++) {

  tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); 
  return true;

}