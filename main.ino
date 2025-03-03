#include "thingProperties.h"
#include <DHT.h>

#define RelayPin1 D1  
#define RelayPin2 D2  
#define RelayPin3 D3  
#define RelayPin4 D4  
#define IR_sensor D5
#define temp_sensor D6

#define DHT_PIN D6        // The DHT11 sensor pin
#define DHT_TYPE DHT11    // Define DHT11 type sensor
DHT dht(DHT_PIN, DHT_TYPE); // Initialize DHT sensor

// Global variables for IR sensor and temperature
float temp = 0;
int lightflag1 = 0;
int lightflag2 = 0;
int IR_value = 0;
bool networkConnected = true;  // Assume network is connected initially
bool cloudControlActive = false;  // Flag to check if cloud control is active
int fan1Flag = 0;  // Separate flag for Fan 1
int fan2Flag = 0;  // Separate flag for Fan 2

void setup() {
  // Initialize serial and wait for port to open:
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);

  Serial.begin(9600);
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Set debug message level for connection info
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  ArduinoCloud.update();
  
  // Read temperature from DHT sensor
  temp = dht.readTemperature();
  
  // Check if network is connected
  /*if (ArduinoCloud.connected()) {
    networkConnected = true;
  } else {
    networkConnected = false;
  }*/

  // PIR sensor logic to control lights if no network or network is available
  IR_value = digitalRead(IR_sensor);  // Read PIR sensor value

  if ((IR_value == 0) ||(lightflag1==1)){
    
    digitalWrite(RelayPin1, LOW);  // Turn on light 1
  } else {

    digitalWrite(RelayPin1, HIGH);  // Turn off light 1
   
  }

    if ((IR_value == 0) ||(lightflag2==1)){
 
    digitalWrite(RelayPin2, LOW);  // Turn on light 2
   
  } else {
    digitalWrite(RelayPin2, HIGH);  // Turn off light 2
   
  }


  // Cloud control: Only act on cloud control if PIR has no effect
  /*if (networkConnected) {
    // Cloud control for light 1
    if (light1 == 1) {
      digitalWrite(RelayPin1, LOW);  // Turn on light 1 (cloud control)
      flag = 1;  // Cloud control flag for light 1
    } else {
      digitalWrite(RelayPin1, HIGH);  // Turn off light 1 (cloud control)
      flag = 0;  // Reset cloud control flag for light 1
    }

    // Cloud control for light 2
    if (light2 == 1) {
      digitalWrite(RelayPin2, LOW);  // Turn on light 2 (cloud control)
      flag = 1;  // Cloud control flag for light 2
    } else {
      digitalWrite(RelayPin2, HIGH);  // Turn off light 2 (cloud control)
      flag = 0;  // Reset cloud control flag for light 2
    }
  }
*/
  // Fan control based on temperature or Alexa app (IoT Cloud)
  if ((temp > 32) || (fan1Flag == 1)) {
    digitalWrite(RelayPin3, LOW);  // Turn off fan 1
  } else {
    digitalWrite(RelayPin3, HIGH);  // Turn on fan 1
  }

  if ((temp > 32) || (fan2Flag == 1)) {
    digitalWrite(RelayPin4, LOW);  // Turn off fan 2
  } else {
    digitalWrite(RelayPin4, HIGH);  // Turn on fan 2
  }
}

void onLight1Change() {
  // Cloud control for Light 1
  if (light1 == 1) {
    digitalWrite(RelayPin1, LOW);
   lightflag1 = 1;
  } else {
    digitalWrite(RelayPin1, HIGH);
    lightflag1 = 0;
  }
}

void onLight2Change() {
  // Cloud control for Light 2
  if (light2 == 1) {
    digitalWrite(RelayPin2, LOW);
   lightflag2 = 1;
  } else {
    digitalWrite(RelayPin2, HIGH);
   lightflag2 = 0;
  }
}

/*
  Since Fan1 is READ_WRITE variable, onFan1Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onFan1Change() {
  // Fan control from cloud (IoT Cloud)
  if (fan1 == 1) {
    digitalWrite(RelayPin3, LOW);
    fan1Flag = 1;  // Fan 1 turned off
  } else {
    digitalWrite(RelayPin3, HIGH);
    fan1Flag = 0;  // Fan 1 turned on
  }
}

/*
  Since Fan2 is READ_WRITE variable, onFan2Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onFan2Change() {
  // Fan control from cloud (IoT Cloud)
  if (fan2 == 1) {
    digitalWrite(RelayPin4, LOW);
    fan2Flag = 1;  // Fan 2 turned off
  } else {
    digitalWrite(RelayPin4, HIGH);
    fan2Flag = 0;  // Fan 2 turned on
  }
}




