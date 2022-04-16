

// include the library code:
#include <LiquidCrystal.h>
#include <iarduino_RTC.h>
//#include <iarduino_RTC.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 14, en = 15, d4 = 7, d5 = 6, d6 = 5, d7 = 4; //rs = 14(A0), en = 15(A1)
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

iarduino_RTC time(RTC_DS1302,A3, A5, A4);//A5 = CLK, A4 = DAT, A3 = RS

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
#define CLK 13       // SPI Clock, shared with SD card
#define MISO 12      // Input data, from VS1053/SD card
#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
/*
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
*/
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     10      // VS1053 chip select pin (output)
#define SHIELD_DCS    9      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 8     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
//mp3 define end

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

//setup mp3
Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  //printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
  Serial.println(F("Playing track 001"));
  musicPlayer.playFullFile("/track001.mp3");
  // Play another file in the background, REQUIRES interrupts!
  Serial.println(F("Playing track 002"));
  musicPlayer.startPlayingFile("/track002.mp3");
}//ent setup mp3


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

  // File is playing in the background
  if (musicPlayer.stopped()) {
    Serial.println("Done playing music");
    while (1) {
      delay(10);  // we're done! do nothing...
    }
  }
  if (Serial.available()) {
    char c = Serial.read();
    
    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }
    
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } else { 
        Serial.println("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }
  }
delay(100);
}

/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
