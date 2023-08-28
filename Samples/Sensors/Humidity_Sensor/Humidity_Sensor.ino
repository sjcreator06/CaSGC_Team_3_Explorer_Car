
#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to A0
#include <SPI.h>
#include <SD.h>

dht DHT;

const int chipSelect = 10;

void setup(){ 
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor

// SD Card Code 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  

}

 
void loop(){
  //Start of Program 
 
    DHT.read11(dht_apin);
    
    
    while(1){
      int i = 1;
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
     i +=1;
     String HumidityOrdered = String(i) + ". " + CurrentHumidity;
     Serial.println(HumidityOrdered);
     delay(5000);//Wait 5 seconds before accessing sensor again.        //Fastest should be once every two seconds.

     // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(HumidityOrdered);
      dataFile.close();
      // print to the serial port too:
      Serial.println(HumidityOrdered);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
    }
  
}// end loop() 
