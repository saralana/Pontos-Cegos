
 
int photocellPin = A0;    // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider

 
void setup(void) {
  Serial.begin(9600);   
}
 
void loop(void) {
  photocellReading = analogRead(photocellPin);  
 
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);     // the raw analog reading
 
  // LED gets brighter the darker it is at the sensor
  // that means we have to -invert- the reading from 0-1023 back to 1023-0
  
  photocellReading = 1023 - photocellReading;
  
  delay(100);
}
