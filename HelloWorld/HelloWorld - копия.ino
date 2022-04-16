
/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include <iarduino_RTC.h>
//#include <iarduino_RTC.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 14, en = 15, d4 = 7, d5 = 6, d6 = 5, d7 = 4; //rs = 14(A0), en = 15(A1)
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

iarduino_RTC time(RTC_DS1302,A3, A5, A4);//A5 = CLK, A4 = DAT, A3 = RS

int b1 = 8;
int b2 = 9;
int minut = 0;
int sec = 0;
unsigned long cur_time = 0;
const int rel = 2;


void setup() 
{

  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(rel, OUTPUT);
    delay(300);
    
  Serial.begin(9600);
  

  time.begin();
  //time.settime(0,10,13,15,07,20,3);  // 0  сек, 43 мин, 13 час, 15, июля, 2020 года, ср
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Track:**********");
  lcd.setCursor(0,1);
  lcd.print("V 00");
  
  lcd.setCursor(7,2);
  lcd.print(":");
  
  lcd.setCursor(11, 2);
  lcd.print("00:00");
Serial.print("Start was succesfully");
}

void loop() {


  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(6, 2);
  // print the number of seconds since reset:
  //lcd.print("After start:");
  //minut = millis() / 1000 / 60;
  //sec = millis() / 1000 % 60;

  //cur_time = millis();
  if((millis() - cur_time) > 1000){
    sec += 1;
    cur_time = millis();
  
  if(sec > 59)
  {
    minut += 1;
    sec = 0;
  }
  
  if(minut >= 10)
  {
    lcd.setCursor(5, 2);
    lcd.print(minut);
    lcd.setCursor(7, 2);
    lcd.print(":");
  }
  else
  {
    lcd.setCursor(6, 2);
    lcd.print(minut);
  }
  
  if((sec > 0) && (sec < 10))
  {
    lcd.setCursor(8, 2);
    lcd.print("00");
    lcd.setCursor(9, 2);
    lcd.print(sec);
  }
  else
  {
    lcd.setCursor(8, 2);
    lcd.print(sec);
  }
  }

  if(time.minutes == 39)
  {
    digitalWrite(rel, LOW);
  }
  else
  {
    digitalWrite(rel, HIGH);
  }
/*
  Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
  delay(100); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
*/
  
  lcd.setCursor(11, 2);
  lcd.print(time.gettime("H:i"));
  
  /*
  if(millis() % 1000 == 0){ // если прошла 1 секунда
      Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
      delay(100); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
    }
    */
/*  
  if(digitalRead(b1) == LOW)
  {
    lcd.scrollDisplayLeft();
    Serial.println("b1_high");
  }

  if(digitalRead(b2) == LOW)
  {
    lcd.scrollDisplayRight();
    Serial.println("b2_high");
    
  }
*/
}
