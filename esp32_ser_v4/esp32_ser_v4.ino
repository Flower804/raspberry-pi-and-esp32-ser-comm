#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//task handles
TaskHandle_t esp_turn_handle = NULL; //_2_
TaskHandle_t rasp_turn_handle = NULL;//_3_
TaskHandle_t HUB_handle = NULL;      //_1_

//Hardware Stuff
int analogPin = 4;
int outputPin = 2;
int inputValue = 0;

//------Handlers----------
boolean Busy = false;
boolean Turn = false;  //for reference- Turn= false: esp32 turn
                       //                   = true : rasp  turn


//-------Serial Stuff------
String inputString = "";
String State;
boolean stringComplete = false;
boolean progresse;
//---turns--------
String esp_Jessie; //signal to state - esp32 has priority
String rasp_Jessie;//signal to state - raspberry PI has priority

//Threads
void HUB(void *parameter) {
  while(1){
    if(Busy == true){
      vTaskDelay(10);
    } else {
      Serial.println("ready");
      serialEvent();
      if(stringComplete) {
        if(inputString.indexOf("value") > -1){
          if(Turn){
            Serial.println("True");
          } else {
            Serial.println("False");
          }
        } else if(inputString.indexOf("on") > -1){
          State = esp_Jessie;
          Turn = false;
          Serial.println("on");
          Serial.println("received");
          Busy = true;
          vTaskResume(esp_turn_handle);
        } else if(inputString.indexOf("off") > -1){
          State = rasp_Jessie;
          Turn = true;
          Serial.println("off");
          Serial.println("received");
          Busy = true;
          vTaskResume(rasp_turn_handle);
        } else {
          Serial.println(".");
        }
      }
    }
  }
}

void esp_turn(void *parameter) {
  while(1){
    if(State == esp_Jessie && Busy == true){
      digitalWrite(outputPin, HIGH);
      Serial.println("esp_turn");
      Busy = false;
      vTaskSuspend(esp_turn_handle);
    } else {
      Serial.println("!not my turn E!");
      vTaskDelay(10);
    }
  }
}

void rasp_turn(void *parameter) {
  while(1){
    if(State == rasp_Jessie && Busy == true){
      digitalWrite(outputPin, LOW);
      Serial.println("rasp_turn");
      Busy = false;
      vTaskSuspend(rasp_turn_handle);
    } else {
      Serial.println("!not my turn R!");
      vTaskDelay(10);
    }
  }
}

//norm Voids
void sendStatus() {
  char buffer[50];
  inputValue = analogRead(analogPin);
  sprintf (buffer, "Analog in %d id %d", analogPin);
  Serial.println(buffer);
}

void serialEvent() {
  while(Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if(inChar == '\n') {
      stringComplete = true;
    }
  }
}



void setup() {
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, HIGH);
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  //task def
  xTaskCreate(
    HUB,
    "HUB",
    1024,
    NULL,
    2,
    &HUB_handle);

  xTaskCreate(
    esp_turn,
    "esp_turn",
    1024,
    NULL,
    1,
    &esp_turn_handle);

  xTaskCreate(
    rasp_turn,
    "rasp_turn",
    1024,
    NULL,
    1,
    &rasp_turn_handle);
}

void loop() {

}
