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
   UM MAPA HOSPEDADO NO UMAP (OPENSTREETMAPS)

   A DETECÇÃO PODE SER FEITA NO MODO AUTOMATICO OU MANUAL

   >> MODO AUTOMATICO:
      - LDR
      - Receptor de infravermelho

           Luzes quentes, emissoras de radiação no espectro visível, 
        também emitem radiação IR, na mesma banda de radiação IR de câmeras 
        de visão noturna. (880 a 940 nm)
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
SoftwareSerial mySerial(2,3); // RX, TX

// PINO DE COM. COM SD
const int chipSelect = 10;

// PINO BOTAO MODO MANUAL
const int botao = 6;

//PINO LED INDICATIVO DE CAMERA
int led_camera = 5;

//PINO LED INDICATIVO MODO MANUAL (VERMELHO)
int led_manual = 4;

// VARIAVEIS PARA ARMAZENAMENTO DE IR E LDR
int ir1, ir2, ir3, ir4;
int ldr1, ldr2, ldr3, ldr4;

//PINOS DE IR
int pin_ir1 = A3;
int pin_ir2 = A1;
int pin_ir3 = A7;
int pin_ir4 = A4;

//PINOS DE LDR
int pin_ldr1 = A2;
int pin_ldr2 = A0;
int pin_ldr3 = A6;
int pin_ldr4 = A5;

//REGRESSAO LINEAR | IR | LDR
double reg = 0;


void setup() {

  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600); // reset SoftwareSerial baudrate
  mySerial.flush();
   
   pinMode(chipSelect, OUTPUT);
   pinMode(led_camera, OUTPUT);
   pinMode(led_manual, OUTPUT);
   pinMode(botao,INPUT);
   pinMode(pin_ir1, INPUT);
   pinMode(pin_ir2, INPUT);
   pinMode(pin_ir3, INPUT);
   pinMode(pin_ir4, INPUT);
   pinMode(pin_ldr1, INPUT);
   pinMode(pin_ldr2, INPUT);
   pinMode(pin_ldr3, INPUT);
   pinMode(pin_ldr4, INPUT);
 
   if (!SD.begin(chipSelect)) {
    return;
   }
 
}

void loop() {

  digitalWrite(led_camera, LOW);

  //sensores G1
    ldr1 = 1023 - analogRead(pin_ldr1);
    ir1 = mediaIR(pin_ir1);
  //sensores G2
    ldr2 = 1023 - analogRead(pin_ldr2);
    ir2 = mediaIR(pin_ir2);
  //sensores G2
    ldr3 = 1023 - analogRead(pin_ldr3);
    ir3 = mediaIR(pin_ir3);
  //sensores G2
    ldr4 = 1023 - analogRead(pin_ldr4);
    ir4 = mediaIR(pin_ir4);
  
  Serial.print("IR");
  Serial.print(ir1);
  Serial.print(" "); 
  Serial.print("LDR");
  Serial.println(ldr1);

  detect(ir1, ldr1);
  detect(ir2, ldr2);
  detect(ir3, ldr3);
  detect(ir4, ldr4);
  
}


void detect(int ir, int ldr){
  
    if (ldr<750){
      digitalWrite(led_manual, LOW);  
      if (ir>=(120+ldr/4) || digitalRead(botao)==LOW){
        digitalWrite(led_manual, LOW);       
        cameraDetectada();    
      }
      else { 
        digitalWrite(led_manual, HIGH);
        if (digitalRead(botao)==LOW){     
          cameraDetectada();
        }
      }
  }
// acionamento por botao no caso de alta luminância
  else { 
      digitalWrite(led_manual, HIGH);      
      if (digitalRead(botao)==LOW){
        cameraDetectada();
      }
  }

  return;
}

void cameraDetectada(){
          //  Serial.println("CAMERA DETECTADA");
         char index = 0;
         char pos = 0;
         String dataString = "";

          digitalWrite(led_camera, HIGH);
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

