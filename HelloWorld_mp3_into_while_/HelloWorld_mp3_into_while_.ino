

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

int volume = 20;
const int arraySize = 5;
char songs[arraySize][15] =
{
  {"track001.mp3"},
  {"track002.mp3"},
  {"track003.mp3"},
  {"track004.mp3"},
  {"track005.mp3"}
};

int songNumber = 0;

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
  //time.settime(0,20,13,22,07,20,3);  // 0  сек, 43 мин, 13 час, 15, июля, 2020 года, ср
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Track: -");
  lcd.setCursor(0,1);
  lcd.print("V 00");
  lcd.setCursor(3,1);
  lcd.print(volume);
  
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

  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(volume, volume);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  
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
  

  
/*
  Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
  delay(100); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
*/
  
  
  
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
  if((time.minutes == 19) && (time.Hours == 00))
  {
    digitalWrite(rel, LOW);
    musicPlayer.startPlayingFile(songs[0]);
  }
  else
  {
    digitalWrite(rel, HIGH);
  }

  //my_control_system

  //alarm
  if(musicPlayer.GPIO_digitalRead(7) == HIGH)
  {
    musicPlayer.startPlayingFile("alarm.mp3");
    lcd.setCursor(6,0);
    lcd.print("alarm");
  }
  
  // Start/stop Music Player
  if (musicPlayer.GPIO_digitalRead(2) == HIGH)
  {
    if (! musicPlayer.playingMusic)
    {
      musicPlayer.startPlayingFile(songs[songNumber]);
      sec = 0;
      lcd.setCursor(6,0);
      lcd.print(songs[songNumber]);
    }
  }

  // Pause / Resume
  if (musicPlayer.GPIO_digitalRead(1) == HIGH)
  {
    if (! musicPlayer.paused())
    {
      musicPlayer.pausePlaying(true);
    }
    else
    {
      musicPlayer.pausePlaying(false);
    }
    delay(500);
  }

  // Prev Song
  if (musicPlayer.GPIO_digitalRead(3) == HIGH)
  {
    if (songNumber - 1 >= 0)
    {
      songNumber = songNumber - 1;
    }
    else
    {
      songNumber = arraySize - 1;
    }
    Serial.println(songNumber);
    musicPlayer.startPlayingFile(songs[songNumber]);
    lcd.setCursor(6,0);
    lcd.print(songs[songNumber]);
    sec = 0;
    delay(500);
  }

  // Next Song
  if (musicPlayer.GPIO_digitalRead(4) == HIGH)
  {
    if (songNumber + 1 < arraySize)
    {
      songNumber = songNumber + 1;
    }
    else
    {
      songNumber = 0;
    }
    Serial.println(songNumber);
    musicPlayer.startPlayingFile(songs[songNumber]);
    lcd.setCursor(6,0);
    lcd.print(songs[songNumber]);
    sec = 0;
    delay(500);
  }

  // Lower Volume
  if (musicPlayer.GPIO_digitalRead(5) == HIGH)
  {
    if (volume < 70)
    {
      volume = volume + 1;
      musicPlayer.setVolume(volume, volume);
      lcd.setCursor(3,1);
      lcd.print(volume);
      delay(50);
    }
  }

  // Increase Volume
  if (musicPlayer.GPIO_digitalRead(6) == HIGH)
  {
    if (volume > 0)
    {
      volume = volume - 1;
      musicPlayer.setVolume(volume, volume);
      lcd.setCursor(3,1);
      lcd.print(volume);
      delay(50);
    }
  }

  /*previous version
  // Alternately, we can just play an entire file at once
  // This doesn't happen in the background, instead, the entire
  // file is played and the program will continue when it's done!
  musicPlayer.playFullFile("track001.mp3");

  // Start playing a file, then we can do stuff while waiting for it to finish
  if (! musicPlayer.startPlayingFile("/track001.mp3")) {
    Serial.println("Could not open file track001.mp3");
    while (1);
  }
  Serial.println(F("Started playing"));
*/
  while (musicPlayer.playingMusic) {
    // file is now playing in the 'background' so now's a good time
    // to do something else like handling LEDs or buttons :)

  //alarm
  if(musicPlayer.GPIO_digitalRead(7) == HIGH)
  {
    musicPlayer.startPlayingFile("alarm.mp3");
    lcd.setCursor(6,0);
    lcd.print("alarm");
  }

  // Start/stop Music Player
  if (musicPlayer.GPIO_digitalRead(2) == HIGH)
  {
      musicPlayer.stopPlaying();
      lcd.setCursor(6,0);
      lcd.print("stop");
      break;
  }

    // Pause / Resume
  if (musicPlayer.GPIO_digitalRead(1) == HIGH)
  {
    if (! musicPlayer.paused())
    {
      musicPlayer.pausePlaying(true);
    }
    else
    {
      musicPlayer.pausePlaying(false);
    }
    delay(500);
  }

  // Prev Song
  if (musicPlayer.GPIO_digitalRead(3) == HIGH)
  {
    if (songNumber - 1 >= 0)
    {
      songNumber = songNumber - 1;
    }
    else
    {
      songNumber = arraySize - 1;
    }
    Serial.println(songNumber);
    musicPlayer.startPlayingFile(songs[songNumber]);
    lcd.setCursor(6,0);
    lcd.print(songs[songNumber]);
    sec = 0;
    delay(500);
  }

  // Next Song
  if (musicPlayer.GPIO_digitalRead(4) == HIGH)
  {
    if (songNumber + 1 < arraySize)
    {
      songNumber = songNumber + 1;
    }
    else
    {
      songNumber = 0;
    }
    Serial.println(songNumber);
    musicPlayer.startPlayingFile(songs[songNumber]);
    lcd.setCursor(6,0);
    lcd.print(songs[songNumber]);
    sec = 0;
    delay(500);
  }

  // Lower Volume
  if (musicPlayer.GPIO_digitalRead(5) == HIGH)
  {
    if (volume < 70)
    {
      volume = volume + 1;
      musicPlayer.setVolume(volume, volume);
      lcd.setCursor(3,1);
      lcd.print(volume);
      delay(50);
    }
  }

  // Increase Volume
  if (musicPlayer.GPIO_digitalRead(6) == HIGH)
  {
    if (volume > 0)
    {
      volume = volume - 1;
      musicPlayer.setVolume(volume, volume);
      lcd.setCursor(3,1);
      lcd.print(volume);
      delay(50);
    }
  }
    
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
    //Serial.print(".");
    //delay(1000);
  lcd.setCursor(11, 2);
  lcd.print(time.gettime("H:i"));
}

  lcd.setCursor(11, 2);
  lcd.print(time.gettime("H:i"));
  
}
/*/// File listing helper
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
}*/
