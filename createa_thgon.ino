#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11
#include <Robojax_L298N_DC_motor.h>
// motor 1 settings
#define IN1 2
#define IN2 5
#define ENA 3 // this pin must be PWM enabled pin
const int CCW = 2; // do not change
const int CW  = 1; // do not change
const int trigPin = 9;
const int echoPin = 10;
int minn = 10000;
long duration;
int distance;
bool state = true;
#define motor1 1 // do not change
Robojax_L298N_DC_motor motor(IN1, IN2, ENA, true);
int findarr[10][5] = {{1,70,30,40 ,650},{2,98,20,60,600},{3,69,25,20,700},{4,90,30,30,700}};//the first element of the array is plant number
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(13, OUTPUT);
  motor.begin();
  dht.begin(); // initialize the sensor
}
void move_motor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  while(state == true){
    motor.rotate(motor1, 100, CW);
    if(distance < 11){state = false;}}
  motor.rotate(motor1, 100, CCW);
  delay(1000);
  motor.brake(1);
  
  }

void loop() {
  float sm = map(analogRead(A2),530,1023,100,0);
    if(sm < 20.00){
    digitalWrite(13,HIGH);
    }
    else{
      digitalWrite(13,LOW);}
  int aq = analogRead(A1);
  // read humidity
  float humi  = dht.readHumidity();
  // read temperature as Celsius
  float tempC = dht.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht.readTemperature(true);
  int currentarray[] = {humi,tempC,sm,aq};
  for(int i =0;i<8;i++){
    int f = (sq(currentarray[0]-findarr[i][1]) + sq(currentarray[1]-findarr[i][2]) + sq(currentarray[2]-findarr[i][3])+ sq(currentarray[3]-findarr[i][4]));
    if(f<minn){minn = f;if(f == minn){minn = findarr[i][0];} }
    }

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.println("Humidity: "+ (String)humi + "% " + "Temperature: " + (String)tempC + "°C " + "SoilMoisture: " + (String)sm + "% " + "AirQuality: " + (String)aq + "PPM " + "suggestedPlant: " + (String)minn);

  }

}
