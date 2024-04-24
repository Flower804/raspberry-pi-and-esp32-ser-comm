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

//--------turns----------
String esp_Jessie;

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
            break;
          }
        }
      }
    }
  }
}

void esp_turn(void *parameter) { //!basicaly all of this thread is for testing purposes do to it being completely relying on sensores!
  while(1){
    Serial.println("here");
    if(Busy == true || inputString.indexOf("on") > -1){
      vTaskDelete(HUB_handle); //delete isen't the answer
      Serial.println("esp_turn");
      digitalWrite(outputPin, HIGH);
      inputString = "";
      stringComplete = false;
      Busy = false; //!Thread controll with break and busy!
      vTaskResume(HUB_handle);
    }
    vTaskResume(HUB_handle);
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
