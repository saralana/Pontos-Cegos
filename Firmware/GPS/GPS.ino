
// Arduino UNO 
// Comunicação GPS via serial 
// e 
// Registro dados no SD

// Bibliotecas para SD e comunicação serial com o gps
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

// RX da placa se conecta ao TX do arduino
// (RX, TX)
SoftwareSerial mySerial(3,2); 

// Porta de comunicação com o SD
const int chipSelect = 4;


void setup() {

 Serial.begin(38400);
 Serial.println("VAI FUNFAR"); // Debug 
 mySerial.begin(9600); // Reseta o baudrate do SoftwareSerial
 mySerial.flush();
 
 pinMode(10, OUTPUT);
 if (!SD.begin(chipSelect)) {
  return;
 }
}


void loop() {
// delay(5000);

  char index = 0;
  char pos = 0;
  String dataString = "";

  File dataFile = SD.open("gps_log.txt", FILE_WRITE);
  
  if(dataFile) {
    
    while(mySerial.available()) {
      pos = char(mySerial.read());
      dataString += String(pos);
      index++;
      if (index>200)
        break;
    }
    
    dataFile.print(dataString);
    dataFile.close();
  }
  
  else {
    Serial.println("error opening datalog.txt");
  }
}
