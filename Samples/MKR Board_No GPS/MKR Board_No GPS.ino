// General Libraries
#include <SPI.h>
#include <SD.h>

// Camera Module
#include <ArduCAM.h>
#include <Wire.h>
#include "memorysaver.h"

// GPS Module
#include <Arduino_MKRGPS.h>

// Humidity and Temperature Sensor 
#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to A0
dht DHT;

//____________________________________Camera to SD Card ____________________________________//

//This demo can only work on OV2640_MINI_2MP or OV5642_MINI_5MP or OV5642_MINI_5MP_BIT_ROTATION_FIXED platform.
#if !(defined OV5642_MINI_5MP || defined OV5642_MINI_5MP_BIT_ROTATION_FIXED || defined OV2640_MINI_2MP || defined OV3640_MINI_3MP)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif

#define SD_CS SDCARD_SS_PIN
const int SPI_CS = 7;

#if defined (OV2640_MINI_2MP)
  ArduCAM myCAM( OV2640, SPI_CS );
#elif defined (OV3640_MINI_3MP)
  ArduCAM myCAM( OV3640, SPI_CS );
#else
  ArduCAM myCAM( OV5642, SPI_CS );
#endif

void myCAMSaveToSDFile(){
char str[8];
byte buf[256];
static int i = 0;
static int k = 0;
uint8_t temp = 0,temp_last=0;
uint32_t length = 0;
bool is_header = false;
File outFile;
//Flush the FIFO
myCAM.flush_fifo();
//Clear the capture done flag
myCAM.clear_fifo_flag();
//Start capture
myCAM.start_capture();
Serial.println(F("start Capture"));
while(!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK));
Serial.println(F("Capture Done."));  
length = myCAM.read_fifo_length();
Serial.print(F("The fifo length is :"));
Serial.println(length, DEC);
if (length >= MAX_FIFO_SIZE) //384K
{
  Serial.println(F("Over size."));
  return ;
}
if (length == 0 ) //0 kb
{
  Serial.println(F("Size is 0."));
  return ;
}
//Construct a file name
k = k + 1;
itoa(k, str, 10);
strcat(str, ".jpg");
//Open the new file
outFile = SD.open(str, O_WRITE | O_CREAT | O_TRUNC);
if(!outFile){
  Serial.println(F("File open faild"));
  return;
}
myCAM.CS_LOW();
myCAM.set_fifo_burst();
while ( length-- )
{
  temp_last = temp;
  temp =  SPI.transfer(0x00);
  //Read JPEG data from FIFO
  if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
  {
    buf[i++] = temp;  //save the last  0XD9     
    //Write the remain bytes in the buffer
    myCAM.CS_HIGH();
    outFile.write(buf, i);    
    //Close the file
    outFile.close();
    Serial.println(F("Image save OK."));
    is_header = false;
    i = 0;
  }  
  if (is_header == true)
  { 
    //Write image data to buffer if not full
    if (i < 256)
    buf[i++] = temp;
    else
    {
      //Write 256 bytes image data to file
      myCAM.CS_HIGH();
      outFile.write(buf, 256);
      i = 0;
      buf[i++] = temp;
      myCAM.CS_LOW();
      myCAM.set_fifo_burst();
    }        
  }
  else if ((temp == 0xD8) & (temp_last == 0xFF))
  {
    is_header = true;
    buf[i++] = temp_last;
    buf[i++] = temp;   
  } 
} 
}


void setup(){
//_________________Camera Module_________________//
  
uint8_t vid, pid;
uint8_t temp;
Wire.begin();
Serial.begin(115200);
while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
if (!GPS.begin()) {
    Serial.println("Failed to initialize GPS!");
    while (1);
  }
Serial.println(F("ArduCAM Start!"));
//set the CS as an output:
pinMode(SPI_CS,OUTPUT);
digitalWrite(SPI_CS, HIGH);
// initialize SPI:
SPI.begin();
  
//Reset the CPLD
myCAM.write_reg(0x07, 0x80);
delay(100);
myCAM.write_reg(0x07, 0x00);
delay(100);
  
while(1){
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  
  if (temp != 0x55){
    Serial.println(F("SPI interface Error!"));
    delay(1000);continue;
  }else{
    Serial.println(F("SPI interface OK."));break;
  }
}
//Initialize SD Card
while(!SD.begin(SD_CS)){
  Serial.println(F("SD Card Error!"));delay(1000);
}
Serial.println(F("SD Card detected."));


#if defined (OV2640_MINI_2MP)
  while(1){
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
      Serial.println(F("Can't find OV2640 module!"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("OV2640 detected."));break;
    } 
  }
#elif defined (OV3640_MINI_3MP)
  while(1){
    //Check if the camera module type is OV3640
    myCAM.rdSensorReg16_8(OV3640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV3640_CHIPID_LOW, &pid);
    if ((vid != 0x36) || (pid != 0x4C)){
      Serial.println(F("Can't find OV3640 module!"));
      delay(1000);continue; 
    }else{
      Serial.println(F("OV3640 detected."));break;    
    }
 } 
#else
  while(1){
    //Check if the camera module type is OV5642
    myCAM.wrSensorReg16_8(0xff, 0x01);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
    if((vid != 0x56) || (pid != 0x42)){
      Serial.println(F("Can't find OV5642 module!"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("OV5642 detected."));break;
    } 
  }
#endif
myCAM.set_format(JPEG);
myCAM.InitCAM();
#if defined (OV2640_MINI_2MP)
  myCAM.OV2640_set_JPEG_size(OV2640_1024x768);
#elif defined (OV3640_MINI_3MP)
  myCAM.OV3640_set_JPEG_size(OV3640_320x240);
#else
  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_320x240);
#endif
delay(1000);


//_________________Humidity and Temperature Sensor_________________//
Serial.println("DHT11 Humidity & temperature Sensor\n\n");

}


int i = 1;
void loop(){
//_________________Camera Module_________________//
myCAMSaveToSDFile();
Serial.println(" ");


String Image = "_______________Image ";
Image += i;
Image += "_______________";
Serial.println(Image);
Serial.println("");
i += 1;


//_________________GPS Module_________________//
// check if there is new GPS data available
  if (GPS.available()) {
    // read GPS values
    float latitude   = GPS.latitude();
    float longitude  = GPS.longitude();
    float altitude   = GPS.altitude();
    float speed      = GPS.speed();
    int   satellites = GPS.satellites();

    // print GPS values
    Serial.print("Location: ");
    Serial.print(latitude, 7);
    Serial.print(", ");
    Serial.println(longitude, 7);

    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println("m");

    Serial.print("Ground speed: ");
    Serial.print(speed);
    Serial.println(" km/h");

    Serial.print("Number of satellites: ");
    Serial.println(satellites);

    Serial.println();
    
  }

  
  

//_________________Humidity and Temperature Sensor _________________//

DHT.read11(dht_apin);
      String CurrentHumidity = "Current Humidity = ";
      int Humidity = DHT.humidity;
      CurrentHumidity += Humidity;
      String Percent = "%    ";
      CurrentHumidity += Percent;
      String Temperature = "Temperature = ";
      CurrentHumidity += Temperature;
      int TemperatureAmount = DHT.temperature;
      CurrentHumidity += TemperatureAmount;
      String TemperatureUnit = "C   ";
      CurrentHumidity += TemperatureUnit;
      String HumidityOrdered = CurrentHumidity;
     

     // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("DATALOG.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(Image);
      dataFile.println(" ");
      dataFile.println("Location: ");
      dataFile.print(GPS.latitude());
      dataFile.print(", ");
      dataFile.println(GPS.longitude());
      dataFile.print("Altitude: ");
      dataFile.print(GPS.altitude());
      dataFile.println("m");
      dataFile.print("Ground speed: ");
      dataFile.print(GPS.speed());
      dataFile.println(" km/h");
      dataFile.print("Number of satellites: ");
      dataFile.print(GPS.satellites());
      dataFile.println(" ");
      dataFile.println(HumidityOrdered);
      dataFile.println(" ");
      dataFile.close();
      Serial.println(" ");
      Serial.println(HumidityOrdered);
      Serial.println(" ");
    }

    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening DATALOG.txt");
    }

  delay(10000);     // Logs Data Every 10 Seconds 

}
