#include <Servo.h>

//sg90 can rotate 180 degrees 

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  // put your setup code here, to run once:

}

void loop() {
  int val;

  while (Serial.available() > 0){
    val = Serial.parseInt();
    if (val != 0){
      Serial.print("parsed value: ");
      Serial.println(val);
      myServo.write(val);
    }
    delay(5);
  }

}
