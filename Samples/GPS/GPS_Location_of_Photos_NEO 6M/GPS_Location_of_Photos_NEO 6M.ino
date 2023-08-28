#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>

// GPS Module Connections
// VCC → 5v (+)
// GND → GND (-)
// RxD → D8 on RedBoard
// TxD → D9 on RedBoard
SoftwareSerial serial_connection(9, 8); // RX=pin 9, TX=pin 8

// Initialize the GPS object
TinyGPSPlus gps;

// Chip select pin for SD card module
const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  serial_connection.begin(9600);

  // Initialize SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("Card initialized.");
}


void loop(){   
// Check if there is new GPS data available
  if (serial_connection.available()) {
    gps.encode(serial_connection.read());
  }

  // Check if GPS data is updated
  if (gps.location.isUpdated()) {   
    for (int i=1; i<=6; i++){
      String GPSData = "GPS Dataset " + String(i);
      GPSData += "\nSatellite Count: ";
      int Satilites = gps.satellites.value();
      GPSData += String(Satilites); 
      String Latitude = String(gps.location.lat(), 6);
      GPSData += "\nLatitude: " + Latitude;
      String Longitude = String(gps.location.lng(), 6);
      GPSData += "\nLongitude: " + Longitude;
      int MPH = gps.speed.mph();
      GPSData += "\nSpeed MPH:" + String(MPH);
      int AltitudeFeet = gps.altitude.feet();
      GPSData += "\nAltitude Feet:" + String(AltitudeFeet);
      GPSData += "\n\n";
      Serial.print(GPSData);
      delay(1000);  
      
      // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File SDFile =  SD.open("GPS.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (SDFile) {
      SDFile.println(GPSData);
      SDFile.close();
      // print to the serial port too:
      Serial.print(GPSData);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
      }
    }
  }   
      }

    
    
  
