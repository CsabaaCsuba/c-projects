#include <Ds1302.h>

#define DS1302_CLK 6
#define DS1302_DAT 7
#define DS1302_RST 8

Ds1302 rtc(DS1302_CLK, DS1302_DAT, DS1302_RST);

void setup() {
  Serial.begin(9600);

  rtc.setTime(2024, 12, 1, 20, 15, 0);

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  Time t = rtc.getTime();

  Serial.print("Time: ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(":");
  Serial.println(t.sec);
  
  delay(1000);

}
