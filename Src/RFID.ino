#include <SPI.h>
#include <MFRC522.h>
//#include <Servo.h>

#define SDA_Pin 10
#define RST_Pin 9
#define Led_Green_Pin 5
#define Led_Red_Pin   4

// instance variables
MFRC522 rfidCircuitReader(SDA_Pin, RST_Pin); 

static void Init()
{
  Serial.begin(9600);
  SPI.begin();
  rfidCircuitReader.PCD_Init();
}

void setup() 
{
  //myServo.attach(3); //servo pin
  //myServo.write(0); //servo start position
  pinMode(
    Led_Green_Pin, 
    OUTPUT);
  pinMode(
    Led_Red_Pin, 
    OUTPUT);

  //
  // Debugging purposes only
  //
  Serial.println("Put your card to the reader...\n");
}
void loop() 
{

  if ( !rfidCircuitReader.PICC_IsNewCardPresent() || !rfidCircuitReader.PICC_ReadCardSerial()) 
  {
    return;
  }
  else
  {
    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content = "";
    byte letter;
    for (byte i = 0; i < rfidCircuitReader.uid.size; i++) 
    {
       Serial.print(rfidCircuitReader.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(rfidCircuitReader.uid.uidByte[i], HEX);
       content.concat(String(rfidCircuitReader.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(rfidCircuitReader.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "XX XX XX XX") //change here the UID of the card/cards that you want to give access
    {
      Serial.println("Authorized access");
      Serial.println();
      delay(500);
      digitalWrite(LED_G, HIGH);
      tone(BUZZER, 500);
      delay(300);
      noTone(BUZZER);
      myServo.write(180);
      delay(5000);
      myServo.write(0);
      digitalWrite(LED_G, LOW);
    }
    else   
    {
      Serial.println(" Access denied");
      digitalWrite(LED_R, HIGH);
      tone(BUZZER, 300);
      delay(1000);
      digitalWrite(LED_R, LOW);
      noTone(BUZZER);
    }
  }
}
