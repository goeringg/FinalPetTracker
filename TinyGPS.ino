#include <TinyGPS++.h> // Include the TinyGPS++ library
#include <string.h>
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object

#define GPS_BAUD 9600 // GPS module baud rate. GP3906 defaults to 9600.

// If you're using an Arduino Uno, RedBoard, or any board that uses the
// 0/1 UART for programming/Serial monitor-ing, use SoftwareSerial:
#include <SoftwareSerial.h>
#define ARDUINO_GPS_RX 9 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 8 // GPS RX, Arduino TX pin
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial

// Set gpsPort to either ssGPS if using SoftwareSerial or Serial1 if using an
// Arduino with a dedicated hardware serial port
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo

// Define the serial monitor port. On the Uno, and Leonardo this is 'Serial'
//  on other boards this may be 'SerialUSB'
#define SerialMonitor Serial

void setup()
{
  //SerialMonitor.begin(9600);
  Serial.begin(9600);
  gpsPort.begin(GPS_BAUD);
}

void loop()
{
  // print position, altitude, speed, time/date, and satellites:
  printGPSInfo();

  // "Smart delay" looks for GPS data while the Arduino's not doing anything else
  smartDelay(1000); 
}

void printGPSInfo()
{
  // Print latitude, longitude, altitude in feet, course, speed, date, time,
  // and the number of visible satellites.
  //SerialMonitor.print("Lat: "); SerialMonitor.println(tinyGPS.location.lat(), 6);
  //SerialMonitor.println();
  //SerialMonitor.print("Long: "); SerialMonitor.println(tinyGPS.location.lng(), 6);
  double latitude = tinyGPS.location.lat();
  double longitude = tinyGPS.location.lng();
  double latInfo;
  double longInfo;
  double latFrac = modf(latitude, &latInfo);
  double longFrac = modf(longitude, &longInfo);
  String s = String(latitude, 6);
  String s1 = String(longitude, 6);
  String finalLat = s;
  String finalLong = s1;
  if(0 <= latInfo && latInfo < 10){
    //SerialMonitor.print("lat ran here");
    //SerialMonitor.println();
    String catString = "000";
    finalLat = catString + s;
  }
  else if(10 <= latInfo && latInfo < 100){
    String catString = "00";
    finalLat = catString + s;
  }
  else if(latInfo >= 100){
    String catString = "0";
    finalLat = catString + s;
  }
  if(0 <= longInfo && longInfo < 10){
    //SerialMonitor.print("long ran here 1");
    //SerialMonitor.println();
    String catString = "000";
    finalLong = catString + s1;
  }
  else if(10 <= longInfo && longInfo < 100){
    //SerialMonitor.print("long ran here");
    //SerialMonitor.println();
    //SerialMonitor.println();
    String catString = "00";
    finalLong = catString + s1;
  }
  else if(longInfo >= 100){
    String catString = "0";
    finalLong = catString + s1;
  }
  if(latInfo < 0 && latInfo > -10){
    latitude = latitude * -1;
    s = String(latitude, 6);
    String catString = "00";
    finalLat = "-" + catString + s;
  }
  else if(latInfo <= -10 && latInfo > -100){
    latitude = latitude * -1;
    s = String(latitude, 6);
    String catString = "0";
    finalLat = "-" + catString + s;
  }
  if(latInfo < 0 && latInfo > -10){
    longitude = longitude * -1;
    s1 = String(longitude, 6);
    String catString = "00";
    finalLong = "-" + catString + s1;
  }
  else if(longInfo <= -10 && longInfo > -100){
    longitude = longitude * -1;
    s1 = String(longitude, 6);
    String catString = "0";
    finalLong = "-" + catString + s1;
  }

  String comma = "0123.456789/-123.456789";
  String printString = " ";
  printString = finalLat + "/" + finalLong;
  


  //SerialMonitor.print(printString);
  Serial.print(printString);
  //Serial.print(finalLong);
  //Serial.print(",");
  //Serial.print(finalLong);
  //Serial.println();
  
  
}

// This custom version of delay() ensures that the tinyGPS object
// is being "fed". From the TinyGPS++ examples.
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    // If data has come in from the GPS module
    while (gpsPort.available())
      tinyGPS.encode(gpsPort.read()); // Send it to the encode function
    // tinyGPS.encode(char) continues to "load" the tinGPS object with new
    // data coming in from the GPS module. As full NMEA strings begin to come in
    // the tinyGPS library will be able to start parsing them for pertinent info
  } while (millis() - start < ms);
}
