    /////////////////////////////////////////////////////////////////
   //            Arduino Simple Data Logger             v1.03     //
  //         Get the latest version of the code here:            //
 //         http://educ8s.tv/arduino-data-logger-project        //
/////////////////////////////////////////////////////////////////

#include <Wire.h>
#include "ds3231.h"
#include <SD.h>
#include <SPI.h>

#define BUFF_MAX 128

int CS_PIN = 10;

File file;

String temperature;
String timeString;
String dateString;

int entryId = 0;

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 1000;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    
    initializeSD();
    
    DS3231_init(DS3231_INTCN);
    memset(recv, 0, BUFF_MAX);
    
    //parse_cmd("T302410426032015",16); //Set time
}

void loop()
{
    String logEntry;
    char in;
    char buff[BUFF_MAX];
    unsigned long now = millis();

    // Log data once in a while
    if ((now - prev > interval) && (Serial.available() <= 0)) {
      getTime();
      getTemperature(); 
      logEntry = createLogEntry();
      writeEntryToFile(logEntry);
      prev = now;
    }

    if (Serial.available() > 0) {
        in = Serial.read();

        if ((in == 10 || in == 13) && (recv_size > 0)) {
            parse_cmd(recv, recv_size);
            recv_size = 0;
            recv[0] = 0;
        } else if (in < 48 || in > 122) {;       // ignore ~[0-9A-Za-z]
        } else if (recv_size > BUFF_MAX - 2) {   // drop lines that are too long
            // drop
            recv_size = 0;
            recv[0] = 0;
        } else if (recv_size < BUFF_MAX - 2) {
            recv[recv_size] = in;
            recv[recv_size + 1] = 0;
            recv_size += 1;
        }

    }
}

void parse_cmd(char *cmd, int cmdsize)
{
    uint8_t i;
    uint8_t reg_val;
    char buff[BUFF_MAX];
    struct ts t;

    // TssmmhhWDDMMYYYY aka set time
    if (cmd[0] == 84 && cmdsize == 16) {
        //T355720619112011
        t.sec = inp2toi(cmd, 1);
        t.min = inp2toi(cmd, 3);
        t.hour = inp2toi(cmd, 5);
        t.wday = inp2toi(cmd, 7);
        t.mday = inp2toi(cmd, 8);
        t.mon = inp2toi(cmd, 10);
        t.year = inp2toi(cmd, 12) * 100 + inp2toi(cmd, 14);
        DS3231_set(t);
        //Serial.println("OK");
    } else if (cmd[0] == 49 && cmdsize == 1) {  // "1" get alarm 1
        DS3231_get_a1(&buff[0], 59);
        //Serial.println(buff);
    } else if (cmd[0] == 50 && cmdsize == 1) {  // "2" get alarm 1
        DS3231_get_a2(&buff[0], 59);
       // Serial.println(buff);
    } else if (cmd[0] == 51 && cmdsize == 1) {  // "3" get aging register
        //Serial.print("aging reg is ");
        //Serial.println(DS3231_get_aging(), DEC);
    } else if (cmd[0] == 65 && cmdsize == 9) {  // "A" set alarm 1
        DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
        //ASSMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // ss, mm, hh, dd
        }
        byte flags[5] = { 0, 0, 0, 0, 0 };
        DS3231_set_a1(time[0], time[1], time[2], time[3], flags);
        DS3231_get_a1(&buff[0], 59);
       // Serial.println(buff);
    } else if (cmd[0] == 66 && cmdsize == 7) {  // "B" Set Alarm 2
        DS3231_set_creg(DS3231_INTCN | DS3231_A2IE);
        //BMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // mm, hh, dd
        }
        byte flags[5] = { 0, 0, 0, 0 };
        DS3231_set_a2(time[0], time[1], time[2], flags);
        DS3231_get_a2(&buff[0], 59);
        //Serial.println(buff);
    } else if (cmd[0] == 67 && cmdsize == 1) {  // "C" - get temperature register
        //Serial.print("temperature reg is ");
        //Serial.println(DS3231_get_treg(), DEC);
    } else if (cmd[0] == 68 && cmdsize == 1) {  // "D" - reset status register alarm flags
        reg_val = DS3231_get_sreg();
        reg_val &= B11111100;
        DS3231_set_sreg(reg_val);
    } else if (cmd[0] == 70 && cmdsize == 1) {  // "F" - custom fct
        reg_val = DS3231_get_addr(0x5);
       // Serial.print("orig ");
        //Serial.print(reg_val,DEC);
        //Serial.print("month is ");
        //Serial.println(bcdtodec(reg_val & 0x1F),DEC);
    } else if (cmd[0] == 71 && cmdsize == 1) {  // "G" - set aging status register
        DS3231_set_aging(0);
    } 
}

void getTemperature()
{
  parse_cmd("C",1);
  temperature = String(DS3231_get_treg());
}

void getTime()
{
  String minute;
  String hour;
  struct ts t;
  
  DS3231_get(&t);
  
  if(t.min<10)
  {
    minute = "0"+String(t.min);
  }else
  {
    minute = String(t.min);
  }
  
  timeString = String(t.hour)+":"+minute;
  dateString = String(t.mon)+"/"+t.mday;
}

String createLogEntry()
{
  String logEntry;
  entryId ++; 
  logEntry = String(entryId)+","+dateString+","+timeString+","+temperature;
  return logEntry;
}

void writeEntryToFile(String entry)
{
  openFileToWrite("log.txt");
  Serial.println(entry);
  writeToFile(entry);
  closeFile();
}

void initializeSD()
{
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
  } else
  {
    return;
  }
}

int openFileToWrite(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
    return 1;
  } else
  {
    return 0;
  }
}

int writeToFile(String text)
{
  if (file)
  {
    file.println(text);
    return 1;
  } else
  {
    return 0;
  }
}

void closeFile()
{
  if (file)
  {
    file.close();
  }
}
