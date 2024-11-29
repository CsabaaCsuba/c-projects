#include <Servo.h>

byte input_x = A0;
byte input_y = A1;
byte button = 2;
int x_val;
int y_val;
byte is_button_pressed;
byte servo_pin = 3;

Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(input_x, INPUT);
  pinMode(input_y, INPUT);
  pinMode(button, INPUT_PULLUP);
  
  servo.attach(8);
  servo.write(0);
  delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
  x_val = analogRead(input_x);
  y_val = analogRead(input_y);
  is_button_pressed = digitalRead(is_button_pressed); //it has to be inverted for true boolean values

  Serial.print("X: ");
  Serial.println(x_val);
  Serial.print("Y: ");
  Serial.println(y_val);
  Serial.print("Button state: ");
  Serial.println(is_button_pressed);

  servo.write(90);
  delay(500);
  servo.write(0);
  delay(500);

}
