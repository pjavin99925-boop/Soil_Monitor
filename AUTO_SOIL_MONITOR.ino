#include <SoftwareSerial.h>
const int RX = 11; //arduino pins
const int TX = 10;// arduino pins
const int Soil_Port = A0;
const int DRY_val = 1023; // dry = high resistence = high number
const int WET_val = 400; // sensor measure voltage resistance, wet = low resistance = low number 


struct LEDpins {
  int green = 6;
  int yellow = 7; 
  int red = 8; 
};

const LEDpins led;

SoftwareSerial hm10(11, 10);
//                   ^    ^
//                   RX   TX on arduino uno
void OffLED3(bool r, bool y, bool g) {
  digitalWrite(led.green,  g ? HIGH : LOW);
  digitalWrite(led.yellow, y ? HIGH : LOW);
  digitalWrite(led.red,    r ? HIGH : LOW);
} //true false function, can select which leds activate without constantly calling digitalWrite()

void Message(String msg, int maped) {
      Serial.print("Moisture % = ");
      Serial.print(maped);
      Serial.println(msg);

      hm10.print("Moist%= ");
      hm10.print(maped);
      hm10.println(msg);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  hm10.begin(9600);
  Serial.println("Ready — waiting for Bluetooth connection");
  pinMode(led.green, OUTPUT);
  pinMode(led.yellow, OUTPUT);
  pinMode(led.red, OUTPUT); 
  hm10.print("AT+POWE3");//amplify connection

}

void loop() {
  // put your main code here, to run repeatedly:

    int raw = analogRead(Soil_Port); 
    int maped = constrain( ((raw - DRY_val) * 100L / (WET_val - DRY_val)), 0, 100);



    if (maped <= 20 && maped >= 0 ) {

      Message(" BAD DRY ", maped);
      OffLED3(true, false, false);
     
    } else if (maped <= 39 && maped >= 21) {
  
      Message(" dry ", maped);
      OffLED3(false, true, false);

    } else if (maped <= 65 && maped >= 40) {
    
      Message(" Safe ", maped);
      OffLED3(false, false, true);

    } else if (maped <= 80 && maped >= 66) {
    
      Message(" Let rest", maped);
      OffLED3(false, true, false);

    } else if (maped <= 100 && maped >= 81) {
   
      Message(" DRAIN", maped);
      OffLED3(true, false, false);

    } else {
    
      Serial.print("-----  UNKNOWN ");
      OffLED3(false, false, false);

    }


    
    delay(200);
}
