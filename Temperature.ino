char degree=176; //ASCII Value of Degree
const int sensor=A1;
void setup()
{
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int tmp = analogRead(sensor);//Reading data from the sensor.This voltage is stored as a 10bit number.
  float voltage = (tmp * 5.0)/1024;//(5*temp)/1024 is to convert the 10 bit number to a voltage reading.
  float milliVolt = voltage * 1000;//This is multiplied by 1000 to convert it to millivolt.
  
  float tmpCel =  (milliVolt-500)/10 ;
  Serial.print("Celsius: ");
  Serial.print(tmpCel);
  Serial.println(degree);
  delay(1000);
}