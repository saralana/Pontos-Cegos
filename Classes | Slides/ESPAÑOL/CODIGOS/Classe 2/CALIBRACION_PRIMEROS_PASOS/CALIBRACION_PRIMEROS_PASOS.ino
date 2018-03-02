
// BOTON QUE VA SER ACIONADO CUANDO HAY UNA CAMERA
int botonCON = 6; 


// LED QUE SE PRENDE CUANDO EL BOTON ES ACCIONADO
int LED = 8;

//PINOS DE LDR
int pin_LDR1 = A0;

//PINOS DE IR
int pin_IR1 = A1;

// VARIAVEIS PARA ARMAZENAMIENTO DE LAS LECTURAS DE IR E LDR
int IR1;
int LDR1;



void setup() {
   
   // ABERTURA DE LA PUERTA DE COMUNICACION SERIAL PARA LECTURA DE LOS DATOS EN EL MONITOR
   Serial.begin(9600);

   // DECLARACION DE ENTRADAS Y SALIDAS
   pinMode(botonCON,INPUT);
   pinMode(LED, OUTPUT);
   pinMode(pin_IR1, INPUT);
   pinMode(pin_LDR1, INPUT);
 
}


void loop() {

    //RETRASO DE 100ms PARA ESTABILIDAD
    delay(100);

    //ARMAZENAMIENTO DE LA LECTURA DE LOS SENSORES EN LAS VARIABLES CREADAS
    LDR1 = 1023 - analogRead(pin_LDR1);
    IR1 = 1023 - analogRead(pin_IR1);

    //COMANDOS PARA ESCRIBIR EN EL MONITOR 
    Serial.print("IR ");
    Serial.print(IR1);
    Serial.print("   LDR ");
    Serial.println(LDR1); //PRINTLN se utiliza para saltar una linea 
                          //y visualizar mejor el archivo en el monitor
            
    // SI EL BOTON ES ACCIONADO, SE PRENDE EL LED
    if (digitalRead(botonCON)==LOW){
          digitalWrite(LED, HIGH);        
    }

    else {
      digitalWrite(LED, HIGH); 
    }


}

