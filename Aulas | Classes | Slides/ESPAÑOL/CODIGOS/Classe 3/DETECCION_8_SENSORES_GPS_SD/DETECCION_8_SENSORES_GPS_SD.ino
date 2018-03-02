/* CODIGO PARA ARDUINO PRO MINI
 *  
 *  CODIGO PARA DETECCAO DE CAMERAS DE SEGURANÇA
   ADAPTADAS PARA VISÃO NOTURNA EQUIPADAS COM 
   EMISSOR DE INFRAVERMELHO  

   DOCUMENTAÇÃO NO SITE
   http://pontoscegos.xyz

   PARA CADA CÂMÊRA DETECTADA (MANUAL||DETECÇÃO IR), AS COORDENADAS SÃO COLETADAS PELO 
   MÓDULO GPS E ARMAZENADAS EM UM ARQUIVO GPS.TXT, GRAVADO NO CARTÃO SD.

   POSTERIORMENTE ESSE ARQUIVO É ENVIADO PARA UM REPOSITÓRIO E AS CÂMERAS SÃO VISTAS EM 
   UM MAPA HOSPEDADO NO UMAP (OPENSTREETMAPS)

   A DETECÇÃO PODE SER FEITA NO MODO AUTOMATICO OU MANUAL

   >> MODO AUTOMATICO:
      - LDRM
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

// PINS DE COMUNICACION COM GPS
SoftwareSerial mySerial(2,3); // RX, TX

// PINO DE COM. COM SD
const int tarjetaSD = 10;

// PINO boton MODO MANUAL
const int boton = 6;

//PINO LED INDICATIVO DE CAMERA DETECTADA
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
   
   pinMode(tarjetaSD, OUTPUT);
   pinMode(led_camera, OUTPUT);
   pinMode(led_manual, OUTPUT);
   pinMode(boton,INPUT);
   pinMode(pin_ir1, INPUT);
   pinMode(pin_ir2, INPUT);
   pinMode(pin_ir3, INPUT);
   pinMode(pin_ir4, INPUT);
   pinMode(pin_ldr1, INPUT);
   pinMode(pin_ldr2, INPUT);
   pinMode(pin_ldr3, INPUT);
   pinMode(pin_ldr4, INPUT);
 
   if (!SD.begin(tarjetaSD)) {
    return;
   }
 
}

void loop() {

  delay(10);
  
  digitalWrite(led_camera, LOW);
  
  //LEITURAS
  //sensores G1
    ldr1 = 1023 - analogRead(pin_ldr1);
    ir1 = 1023 - analogRead(pin_ir1);
  //sensores G2
    ldr2 = 1023 - analogRead(pin_ldr2);
    ir2 = 1023 - analogRead(pin_ir2);
  //sensores G2
    ldr3 = 1023 - analogRead(pin_ldr3);
    ir3 = 1023 - analogRead(pin_ir3);
  //sensores G2
    ldr4 = 1023 - analogRead(pin_ldr4);
    ir4 =  1023 - analogRead(pin_ir4);

  Serial.print("IR1");
  Serial.print(" ");
  Serial.print(ir1);
  Serial.print(" "); 
  Serial.print("LDR1");
  Serial.print(" ");
  Serial.print(ldr1);
  Serial.print("    "); 

  Serial.print("IR2");
  Serial.print(" ");
  Serial.print(ir2);
  Serial.print(" "); 
  Serial.print("LDR2");
  Serial.print(" ");
  Serial.print(ldr2);
  //Serial.println(ldr2);
  Serial.print("      "); 

  Serial.print("IR3");
  Serial.print(" ");
  Serial.print(ir3);
  Serial.print(" "); 
  Serial.print("LD3");
  Serial.print(" ");
  Serial.print(ldr3);
  Serial.print("      "); 
    
  Serial.print("IR4");
  Serial.print(" ");
  Serial.print(ir4);
  Serial.print(" "); 
  Serial.print("LDR4");
  Serial.print(" ");
  Serial.println(ldr4);
  Serial.print(" "); 


//DETECCAO AUTOMATICA
  detect(ir1, ldr1);
  detect(ir2, ldr2);
  detect(ir3, ldr3);
  detect(ir4, ldr4);


//DETECCAO POR boton
  if (digitalRead(boton)==LOW){
      cameraDetectada();
  }

// MODO MANUAL NO CASO DE ALTA LUMINANCIA
  if ((ldr1>600)||(ldr2>600)||(ldr3>600)||(ldr4>600))
        digitalWrite(led_manual, HIGH); 
  else
        digitalWrite(led_manual, LOW);       
}

void detect(int ir, int ldr){ 
  if(ldr<600 && ir>700)
        cameraDetectada();       
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
           // Serial.println("error opening datalog.txt");
          }
}


