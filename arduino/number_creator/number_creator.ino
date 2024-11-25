/*How to use a seven-segment LED display.
  created by the SriTu Tech team.
  Read the code below and use it for any of your creations.
  Home Page
*/
byte pin[] = {2, 3, 4, 5, 6, 7, 8, 9};//arduino pin array

byte numbers[10][8] = {
  //lower left, lower middle, lower right, dot, upper right, upper middle, upper left, middle
  {1, 1, 1, 0, 1, 1, 1, 0}, //0
  {1, 0, 0, 0, 0, 0, 1, 0}, //1
  {1, 1, 0, 0, 1, 1, 0, 1}, //2
  {0, 1, 1, 0, 1, 1, 0, 1}, //3
  {0, 0, 1, 0, 1, 0, 1, 1}, //4
  {0, 1, 1, 0, 0, 1, 1, 1}, //5
  {1, 1, 1, 0, 0, 1, 1, 1}, //6
  {0, 0, 1, 0, 1, 1, 0, 0}, //7
  {1, 1, 1, 0, 1, 1, 1, 1}, //8
  {0, 1, 1, 0, 1, 1, 1, 1}, //9
};

void write_num(byte num){
  if (num > 9) {
    return;
  }
  else {
    for (byte i=0; i<9; i++) {
      digitalWrite(i+2, numbers[num][i]); //+2 because numbers start at pin 2
    }
  }
}

void setup() {
  for (byte a = 0; a < 8; a++) {
    pinMode(pin[a], OUTPUT);//define output pins
  }
  Serial.begin(9600);
}

void loop() {
  //write_num(9);
  int sensor_value = analogRead(A0);
  byte num_value = (byte)(sensor_value * (9.0 / 1023.0));
  write_num(num_value);
  Serial.println(num_value);
  delay(300);
  
}
