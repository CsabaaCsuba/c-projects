/*How to use a seven-segment LED display.
  created by the SriTu Tech team.
  Read the code below and use it for any of your creations.
  Home Page
*/
/*
pin order:  Digit1    | UpperMid | UpperLeft  | Digit2      | Digit3 | UpperRight
            LowerLeft | LowerMid | Dot        | LowerRight  | Middle | Digit4


*/

byte digit1 = 2;
byte digit2 = 3;
byte digit3 = 4;
byte digit4 = 5;

byte uppermid = 6;
byte upperleft = 7;
byte upperright = 8;
byte lowerleft = 9;

byte lowermid = 10;
byte dot = 11;
byte lowerright = 12;
byte middle = 13;

byte pin[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};//arduino pin array

byte numbers[][8] = {
  {0, 0, 0, 0, 0, 1, 0, 1}, //0
  {1, 1, 0, 1, 1, 1, 0, 1}, //1
  {0, 1, 0, 0, 0, 1, 1, 0}, //2
  {0, 1, 0, 1, 0, 1, 0, 0}, //3
  {1, 0, 0, 1, 1, 1, 0, 0}, //4
  {0, 0, 1, 1, 0, 1, 0, 0}, //5
  {0, 0, 1, 0, 0, 1, 0, 0}, //6
  {0, 1, 0, 1, 1, 1, 0, 1}, //7
  {0, 0, 0, 0, 0, 1, 0, 0}, //8
  {0, 0, 0, 1, 0, 1, 0, 0} //9
};

void setup() {
  Serial.begin(9600);
  for (byte a = 0; a < sizeof(pin); a++) {
    pinMode(pin[a], OUTPUT);//define output pins
  }
  digitalWrite(digit1, LOW);
  digitalWrite(digit2, LOW);
  digitalWrite(digit3, LOW);
  digitalWrite(digit4, LOW);
  digitalWrite(uppermid, HIGH);
  digitalWrite(upperleft, HIGH);
  digitalWrite(upperright, HIGH);
  digitalWrite(lowerleft, HIGH);
  digitalWrite(lowermid, HIGH);
  digitalWrite(dot, HIGH);
  digitalWrite(lowerright, HIGH);
  digitalWrite(middle, HIGH);
}

void writedigit(byte place, byte digit){
  Serial.print("digit found:");
  Serial.println(digit);
  for (int i=0; i<5; i++){
    if (i+2 == place){
      digitalWrite(i+2, HIGH);
    }
    else {
      digitalWrite(i+2, LOW); //turn it off if it's not the digit we want to write to
    }
  }
  if (digit > 9){
    return;
  }
  digitalWrite(place, HIGH);
  for (int i=0; i<8; i++){
    //Serial.print("current output written:");
    //Serial.println(i+6);
    //(numbers[digit][i] == 0) ? Serial.println("LOW (0) value is written to output") : Serial.println("HIGH (1) value is written to output");
    digitalWrite(i+6, numbers[digit][i]);
    delay(1);
  }
}

void test_digits(){
  for (int i=0; i<10; i++){
    delay(1000);
    writedigit(digit4, i);
    if (i == 9){
      i = -1; //so it can be 0 again
    }
  }
}

void loop() {
  //writedigit(digit1, 0);
  //writedigit(digit2, 1);
  //writedigit(digit3, 2);
  test_digits();
  
}
