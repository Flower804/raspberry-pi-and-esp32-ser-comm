#include <iostream>
#include <cstring>
#include <string>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//-----------COMMENTS--------------
// This version is still a beta and probably I'll try out more stuff and so maybe there will be future verions
// There are still comments with TODO stuff and some !comments! are signaling stuff that are just for testing that stuff is working as intended 
//and will be removed/changed later so that the Serial pannel and log file won't be such a mess
// ass: Your dearest Flower :P

//TODO: think of how to implement sensor reading

//Task handles
TaskHandle_t esp_turn_handle = NULL; //_2_
TaskHandle_t HUB_handle = NULL;      //_1_

//Hardware Stuff
int analogPin= 4;
int outputPin = 2;
int inputValue= 0;

//--------Handlers-------
boolean Busy = false;
boolean Turn = false;

//--------Serial Stuff---
String inputString = "";
String State;
boolean stringComplete = false;
boolean progresse;
int wait_time = 500;

//--------turns----------
String esp_Jessie;

//--------sensor-data----
struct dados_sense
{
  //14336, 1.44, 2.40, -12.49, 12.80, 26.80, 99260.00, 36.63, 0.00, 1.41, 0, 40.122281, -8.237581, 176.70, 0.18, 335.26
  double dado1 = 14336;
  double dado2 = 1.44;
  double dado3 = 2.40;
  double dado4 = -12.49;
  double dado5 = 12.80;
  double dado6 = 26.80;
  double dado7 = 99260.00;
  double dado8 = 36.63;
  double dado9 = 0.00;
  double dado10 = 1.41;
  double dado11 = 0.00;
  double dado12 = 1.41;
  double dado13 = 0;
  double dado14 = 40.122281;
  double dado15 = -8.237581;
  double dado16 = 176.70;
  double dado17 = 0.18;
  double dado18 = 335.26;  
} dados_sense;

//--------Threads--------  !maybe change the name of the Threads now that the communication doesn't have to be 2 ways!
void HUB(void *parameter) {
  while(1){
    Serial.println(inputString);
    digitalWrite(outputPin, LOW); //!Thread control with break and busy! if this runs the break command ran well
    if(Busy == true){ //TODO: try to change the busy mecanic for vTaskSuspend to controll tasks and waste less process power
      Serial.println("!Busy = True!");
      vTaskResume(esp_turn_handle);
    } else {
      while(1){
        digitalWrite(outputPin, LOW);
        Serial.println("ready"); //signal to rasp that esp is ready to receive commands
        serialEvent(); //analise Ser input
        if(stringComplete) {
          if(inputString.indexOf("on") > -1) { //equivelent of esp_turn
            Serial.println("on"); //!repeat command ONLY FOR DEBUGG!
            Serial.println("received"); //reception token
            Busy = true;
            vTaskResume(esp_turn_handle);//!Thread control with break and busy! 
            vTaskSuspend(HUB_handle);
          } else {
            Serial.println("."); //TODO: create other case scenarios for quality of life reasons
            inputString = "";
            stringComplete = false;
          }
        }
      }
    }
  }
}


void esp_turn(void *parameter) { //TODO: simplify this for the dear love of god
  while(1){
    if(Busy == true || inputString.indexOf("on") > -1){ //maybe try to turn this into somekind of list so like Serial.println(dados[i]) or smth
      Serial.println(dados_sense.dado1);
      delay(wait_time);
      Serial.println(dados_sense.dado2);
      delay(wait_time);
      Serial.println(dados_sense.dado3);
      delay(wait_time);
      Serial.println(dados_sense.dado4);
      delay(wait_time);
      Serial.println(dados_sense.dado5);
      delay(wait_time);
      Serial.println(dados_sense.dado6);
      delay(wait_time);
      Serial.println(dados_sense.dado7);
      delay(wait_time);
      Serial.println(dados_sense.dado8);
      delay(wait_time);
      Serial.println(dados_sense.dado9);
      delay(wait_time);
      Serial.println(dados_sense.dado10);
      delay(wait_time);
      Serial.println(dados_sense.dado11);
      delay(wait_time);
      Serial.println(dados_sense.dado12);
      delay(wait_time);
      Serial.println(dados_sense.dado13);
      delay(wait_time);
      Serial.println(dados_sense.dado14);
      delay(wait_time);
      Serial.println(dados_sense.dado15);
      delay(wait_time);
      Serial.println(dados_sense.dado16);
      delay(wait_time);
      Serial.println(dados_sense.dado17);
      delay(wait_time);
      Serial.println(dados_sense.dado18);      
      while(1){
        Serial.println("sent_complete");
        serialEvent();
        if(inputString.indexOf("done") > -1){
          inputString = "";
          stringComplete = false;
          Busy = false; //!Thread controll with break and busy!
          vTaskResume(HUB_handle);
          break;
        }
      }
    }
  }
}

//norm voids
void sendStatus() { //like I think this is unnecessary but the last time I took this out it broke the hole code so...
  char buffer[50];
  inputValue = analogRead(analogPin);
  sprintf (buffer, "Analog in %d id %d", analogPin);
  Serial.println(buffer);
}

void serialEvent() { //Serial analiser !ONLY TAKES 8 BIT STUFF!
  while(Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if(inChar == '\n') { //just want to say THAT THIS GAVE ME SO MANY HEADHACKES BRO 
      stringComplete = true;
    }
  }
}

void setup() {
  pinMode(outputPin, OUTPUT);
  for(int i = 0; i<5; i++){
    digitalWrite(outputPin, HIGH);
    delay(0.5);
    digitalWrite(outputPin, LOW);
    delay(2);
  }
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  xTaskCreate(
    HUB,
    "HUB",
    1024,
    NULL,
    1,
    &HUB_handle);

  xTaskCreate(
    esp_turn,
    "esp_turn",
    1024,
    NULL,
    0,
    &esp_turn_handle);
}

void loop() {
  // put your main code here, to run repeatedly:

}
