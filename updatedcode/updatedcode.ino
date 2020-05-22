
#include <DHT.h>;

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT11// DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
#include <DHT.h>;
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)

#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750   // Define min value we consider soil 'dry'
// Sensor pins
#define sensorPower 7
#define sensorPin A0
#define WATERPIN 2 //+++++++++++++++++++++++++++++++++pump
//#define WATERPOSTDELAY 1000 //++++++++++++++++++++++++++++pump
const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
int sensorValue = 0;        // value read from the pot
int outputValue = 0;  
// value output to the PWM (analog out)
//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(WATERPIN, OUTPUT);
 // pinMode(WATERPOSTDELAY, OUTPUT);
  pinMode(sensorPower, OUTPUT);
  //pinMode(sensorPin,OUTPUT);
  pinMode(analogInPin,INPUT);
  pinMode(analogOutPin,OUTPUT);
  pinMode(sensorValue,INPUT);
  pinMode(outputValue,OUTPUT);
  // Initially keep the sensor OFF
  digitalWrite(sensorPower, LOW);
  // initialize serial communications at 9600 bps:

  Serial.begin(9600);
  dht.begin();
}

void loop() { 
  
       delay(2000);
    //Read data and store it to variables hum and temp
    digitalWrite(led, HIGH);
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    delay(10000); //Delay 2 sec.
  
  int waterMoisture = readSensorWater();
  Serial.print("Water Output: ");
  Serial.println(waterMoisture);

  int moisture = readSensorSoil();
  Serial.print("Soil Output: ");
  Serial.println(moisture);
  
  // Determine status of our soil
  if ((moisture < soilWet)||(waterMoisture<100)) {
    Serial.println("Water does not flow");
    digitalWrite(WATERPIN, LOW);
    
  } else if ((moisture >= soilWet && moisture < soilDry) ||(waterMoisture<100)){
    Serial.println("Water does not flow");
    digitalWrite(WATERPIN, LOW);
  } else{
    Serial.println("Status: Soil is too dry - time to water!");
    digitalWrite(WATERPIN, HIGH);          //??????????????????????????????????????????? 
    //delay(WATERPOSTDELAY);//?????????????????????????????????????????????????????????//

    //delay(WATERPOSTDELAY);
    
    }
  
  delay(1000);  // Take a reading every second for testing
    
  Serial.println();
}

int readSensorWater() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);
  delay(2);
  return outputValue;
}

int readSensorSoil() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = analogRead(sensorPin);  // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}
