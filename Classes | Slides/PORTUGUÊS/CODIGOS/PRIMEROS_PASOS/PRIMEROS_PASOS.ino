
// PINOS
int botonCON = 6; 
int LED1 = 5;
int LED2 = 4;
int pin_LDR1 = A3;
int pin_IR1 = A2;

// VARIAVEIS PARA ARMAZENAMIENTO DE LAS LECTURAS DE IR E LDR
int IR1;
int LDR1;



void setup() {
   
   // ABERTURA DE LA PUERTA DE COMUNICACION SERIAL PARA LECTURA DE LOS DATOS EN EL MONITOR
   Serial.begin(9600);

   // DECLARACION DE ENTRADAS Y SALIDAS
   pinMode(botonCON,INPUT);
   pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);
   pinMode(pin_IR1, INPUT);
   pinMode(pin_LDR1, INPUT);
 
}


void loop() {

    //RETRASO DE 100ms PARA ESTABILIDAD
    delay(127);

    //ARMAZENAMIENTO DE LA LECTURA DE LOS SENSORES EN LAS VARIABLES CREADAS
    LDR1 = 1023 - analogRead(pin_LDR1);
    IR1 = 1023 - analogRead(pin_IR1);

    //COMANDOS PARA ESCRIBIR EN EL MONITOR 
    Serial.print("IR ");
    Serial.print(IR1);
    Serial.print("   LDR ");
    Serial.println(LDR1); //PRINTLN se utiliza para saltar una linea 
                          //y visualizar mejor el archivo en el monitor
            
    // SE O BOTAO FOR ACCIONADO, ACENDER LED1 AMARELO, INDICANDO CAMERA
    if (digitalRead(botonCON)==LOW){
          digitalWrite(LED1, HIGH);        
    }
    
    // SE OS SENSORES DETECTAREM, ACENDER LED1 AMARELO, INDICANDO CAMERA
    else {
        
          if (LDR1<600 && IR1>700){
               digitalWrite(LED1, HIGH);        
          }

          else{
               digitalWrite(LED1, LOW); 
          }
    }
    
// SE TEM MUITA LUZ ACENDER LED2, VERMELHO, INDICANDO MODO MANUAL

    if (LDR1>700){
         digitalWrite(LED2, HIGH);        
    }

    else {
         digitalWrite(LED1, LOW); 
    }
}

