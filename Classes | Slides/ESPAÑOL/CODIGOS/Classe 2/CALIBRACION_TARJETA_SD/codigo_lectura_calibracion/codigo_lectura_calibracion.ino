// programa para calibracion


// BIBLIOTECAS QUE VAN A SER UTILIZADAS PARA LA GRAVACION EN LA TARJETA
#include <SPI.h>
#include <SD.h>

// PINO DE COM. COM SD
int tarjetaSD = 10;

// PIN DEL BOTON QUE VA SER ACIONADO CUANDO HAY UNA CAMERA
int botonCON = 6;

// PIN DEL BOTON QUE VA SER ACIONADO CUANDO NO HAY CAMERA
int botonSIN = 7;

// LED QUE SE PRENDE CUANDO LOS BOTONES SON ACCIONADOS
int LED = 8;

// VARIABLES PARA ARMAZENAMEINTO DE IR E LDR
int IR1;
int LDR1;

//PIN DEL LDR, SENSOR DE LUZ VISIBLE
int pin_LDR1 = A0;

//PIN DE IR, SENSOR DE INFRARROJO
int pin_IR1 = A1;


void setup() {
   Serial.begin(9600);
   pinMode(tarjetaSD, OUTPUT);
   pinMode(botonCON,INPUT);
   pinMode(botonSIN,INPUT);
   pinMode(pin_IR1, INPUT);
   pinMode(pin_LDR1, INPUT);
   pinMode(LED, OUTPUT);

   if (!SD.begin(tarjetaSD)) {
    return;
   }
 
}

void loop() {

    delay(100);
    LDR1 = 1023 - analogRead(pin_LDR1);
    IR1 = 1023 - analogRead(pin_IR1);

            Serial.print("IR ");
            Serial.print(IR1);
            Serial.print("   LDR ");
            Serial.println(LDR1); //PRINTLN se utiliza para saltar una linea 
                                  //y visualizar mejor el archivo en el monitor
            
  
    if (digitalRead(botonCON)==LOW){
           
           // Se prende el LED
           digitalWrite(LED, HIGH);  
           // configuraciones importantes para crear un archivo de texto
           char index = 0;
           char pos = 0;
           String dataString = "";

            // enviando comandos para registro
            File dataFile = SD.open("concam.txt", FILE_WRITE); //SD.open para crear o abrir el archivo           
            dataFile.print("IR ");
            dataFile.print(IR1);
            dataFile.print("   LDR ");
            dataFile.println(LDR1);
            dataFile.close(); //close para cerrar el archivo   
    }

       
     else if (digitalRead(botonSIN)==LOW){
            
           // Se prende el LED
           digitalWrite(LED, HIGH); 
           
           // configuraciones importantes para crear un archivo de texto
           char index = 0;
           char pos = 0;
           String dataString = "";
           
            // enviando comandos para registro
            File dataFile = SD.open("sinmcam.txt", FILE_WRITE);  
            dataFile.print("IR ");
            dataFile.print(IR1);
            dataFile.print("    LDR ");
            dataFile.println(LDR1);
            dataFile.close();
    }

}

