/* CODIGO PARA ARDUINO PRO MINI
 *  
 *  CODIGO PARA DETECCAO DE CAMERAS DE SEGURANÇA
   ADAPTADAS PARA VISÃO NOTURNA EQUIPADAS COM 
   EMISSOR DE INFRAVERMELHO  

   DOCUMENTAÇÃO NO SITE
   http://suss-urro.hotglue.me/pontoscegos

   PARA CADA CÂMÊRA DETECTADA (MANUAL||DETECÇÃO IR), AS COORDENADAS SÃO COLETADAS PELO 
   MÓDULO GPS E ARMAZENADAS EM UM ARQUIVO GPS.TXT, GRAVADO NO CARTÃO SD.

   POSTERIORMENTE ESSE ARQUIVO É ENVIADO PARA UM REPOSITÓRIO E AS CÂMERAS SÃO VISTAS EM 
   UM MAPA HOSPEDADO NO OPENSTREETMAPS

   A DETECÇÃO PODE SER FEITA NO MODO AUTOMATICO OU MANUAL

   >> MODO AUTOMATICO:
      - LDR
      - Receptor de infravermelho

           Luzes quentes, emissoras de radiação no espectro visível, 
        também emitem radiação IR, na mesma banda de radiação IR de câmeras 
        de visão noturna.
           Assim um receptor IR detecta não só aparelhos de visão noturna, como 
        também luzes quentes.
           Para contornar esse problema e filtrar a detecção apenas para 
        dispositivos de visão noturna, utiliza-se um LDR que, quando acionado,
        invalida as detecções automáticas e aciona o modo manual de detecção 
        de câmeras.

   >> MODO MANUAL:
     - Botão

          Quando o LDR detecta presença de luz intensa, o modo manual é acionado e 
          as câmeras passam a ser mapeadas manualmente, ao acionar o botão.       
*/

#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

// PINOS DE COM. COM GPS
//***SoftwareSerial mySerial(3,2); // RX, TX
SoftwareSerial mySerial(2,3); // RX, TX

// PINO DE COM. COM SD
// *** const int chipSelect = 4;
const int chipSelect = 10;

// BOTAO MODO MANUAL
//const int botao = 7;
const int botao = 6;

// SENSOR DE LUZ VISIVEL
//***int ldr = 0;
int ldr = A5;

//***int led_luz = 0;
int led_luz = 5;

//***int ledManual = 9;
int ledManual = 4;

//LEITURA IR E MEDIA
//int ir = A5;

//***int led = 8;
int led = 5;
//***int inputPin = A0;
int ir;
int ir1, ir2, ir3, ir4;
int ldr1, ldr2, ldr3, ldr4;
int irPin = A4;

// LDR
int ldrPin = A5;

//REGRESSAO LINEAR | IR | LDR

double reg = 0;


void setup() {

 Serial.begin(9600);
  // Serial.println("VAI FUNFAR"); // testando comunicacao com arduino
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600); // reset SoftwareSerial baudrate
  mySerial.flush();
 
 pinMode(10, OUTPUT);
 pinMode(led, OUTPUT);
 pinMode(ledManual, OUTPUT);
 pinMode(botao,INPUT);
 //pinMode(ir, INPUT);
 
   if (!SD.begin(chipSelect)) {
    return;
   }
 
}

void loop() {
  //delay(500);
//  char index = 0;
//  char pos = 0;
//  String dataString = "";
  digitalWrite(led, LOW);
  ldr = analogRead(ldrPin);
  ldr=1023-ldr;
  ir1 = mediaIR(irPin);

  
  Serial.print("IR");
  Serial.print(ir1);
  Serial.print(" "); 
  Serial.print("LDR");
  Serial.println(ldr);

  // ldr=6*ldr/1000;
  // reg = (163*ln(average)) -804 -ldr;
  // reg = abs(146*exp(ldr) - average);
  
  // REGRESSAO LINEAR PARA ESTABELECER VALORES DE CORRELAÇÃO ENTRE IR E LDR
  // IR>=120+LDR/4
   
  //reg = abs(average-75-(4*ldr));
   //Serial.println(reg);
  

//QUALQUER VALOR FORA DA CURVA DE CORRELAÇÃO ENTRE IR E LDR, ESTABELECIDA NA REG. LINEAR
//EH CONSIDERADO UMA FONTE DE INFRAVERMELHO

  if (ldr<750){
      digitalWrite(ledManual, LOW);  
      if (ir1>=(120+ldr/4) || digitalRead(botao)==LOW){
        digitalWrite(ledManual, LOW);       
        cameraDetectada();    
      }
      else { 
        digitalWrite(ledManual, HIGH);
        if (digitalRead(botao)==LOW){     
          cameraDetectada();
        }
      }
  }
// acionamento por botao no caso de alta luminância
  else { 
      digitalWrite(ledManual, HIGH);      
      if (digitalRead(botao)==LOW){
        cameraDetectada();
      }
  }
  
}


void cameraDetectada(){
          //  Serial.println("CAMERA DETECTADA");
         char index = 0;
         char pos = 0;
         String dataString = "";

          digitalWrite(led, HIGH);
          File dataFile = SD.open("gps_log.txt", FILE_WRITE);
          
          if(dataFile) {
              while(mySerial.available()) {
                // pos = char(mySerial.read());
                pos = mySerial.read();
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


int mediaIR(int ir){
        
          const int numReadings = 10;
          int readings[numReadings];      // the readings from the analog input
          int readIndex = 0;              // the index of the current reading
          int total = 0;                  // the running total
          int average = 0;                // the average
         
          for (int thisReading = 0; thisReading < numReadings; thisReading++) {
            readings[thisReading] = 0;
          }
        // MEDIA DE 10 LEITURAS DO IR
        // ------------------------------------------------------- \\
      
        // subtract the last reading:
        total = total - readings[readIndex];
        // read from the sensor:
        readings[readIndex] = analogRead(ir);
        // add the reading to the total:
        total = total + readings[readIndex];
        // advance to the next position in the array:
        readIndex = readIndex + 1;
        // if we're at the end of the array...
            if (readIndex >= numReadings) {
              // ...wrap around to the beginning:
              readIndex = 0;
            }
        // calculate the average:
        average = total / numReadings;
        // send it to the computer as ASCII digits
        //average = map(average,0,950,0,1023);
        average=1023-average;
        delay(1);        // delay in between reads for stability
        return average;

}

