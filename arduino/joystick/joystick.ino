byte input_x = A0;
byte input_y = A1;
byte button = 2;
int x_val;
int y_val;
byte is_button_pressed;
void setup() {
  Serial.begin(9600);
  pinMode(input_x, INPUT);
  pinMode(input_y, INPUT);
  pinMode(button, INPUT_PULLUP);

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

}
