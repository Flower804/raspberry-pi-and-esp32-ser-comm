#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//task handles---------------order of appearence
TaskHandle_t Busy_handle = NULL;     //[BACKUP]
TaskHandle_t wait = NULL;     //_1_ (ref_name: bluetick)
TaskHandle_t esp_turn_handle = NULL; //_3_
TaskHandle_t rasp_turn_handle = NULL;//_4_
TaskHandle_t HUB_handle = NULL;      //_2_

//Hardware Stuff
int analogPin = 4;
int outputPin = 2; //led pin
int inputValue = 0;

//Serial Stuff
String inputString = "";
String State;
boolean stringComplete = false;
boolean progresse;
boolean first_run_state = true;
String esp_Jessie; //signal to state - esp32 has priority
String rasp_Jessie;//signal to state - raspberry PI has priority
boolean life_line = false;
boolean reply;

//Tasks
//void first_run(void *parameter) { //first run setup stuff and garanty conection is made
//  vTaskSuspend(wait);
//  vTaskSuspend(esp_turn_handle);
//  vTaskSuspend(rasp_turn_handle);
//  vTaskSuspend(HUB_handle);
//  while(life_line == false) {
//    Serial.println("hello_world");
//    if(inputString.indexOf("hello_back") > -1){
//      life_line == true;
//      vTaskResume(HUB_handle);
//      vTaskDelete(NULL);
//    }
//  }
//  if(life_line == true) {
//    vTaskResume(HUB_handle);
//    vTaskDelete(NULL);
//  }
//}

void bluetick(void *parameter) { //place to go after tasks
  while(!reply) {
    Serial.println(State);
    serialEvent();
    if(inputString.indexOf("received") > -1){
      Serial.println("ok");
      reply = false;
      vTaskResume(HUB_handle);
      vTaskSuspend(NULL);
      }
  }
}

void HUB(void *parameter) { //await for task direction
      while(life_line==false){  
        Serial.println("hello_world");
        serialEvent();
        if(stringComplete){
          if(inputString.indexOf("hello_back") > -1){
            life_line == true;
            vTaskResume(HUB_handle);
            vTaskDelete(NULL);
          } 
        }
      }
        serialEvent();
        if(stringComplete) {
          if(inputString.indexOf("on") > -1){
            State = esp_Jessie;
            vTaskResume(esp_turn_handle);
            vTaskSuspend(NULL); //suspend self
          }
          else if(inputString.indexOf("off") > -1){
            State = rasp_Jessie;
            vTaskResume(rasp_turn_handle);
            vTaskSuspend(NULL); //suspend self
          }
          //stringComplete= false;
        }
      
    delay(10);

    if(Serial.available()>0) serialEvent();
} 

void esp_turn(void *parameter) { //send info (for testing purpesses we consider this LED on)
  digitalWrite(outputPin, HIGH);
  Serial.println("rasp_turn");
  vTaskResume(wait);
}

void rasp_turn(void *parameter) {
  digitalWrite(outputPin, LOW);
  Serial.println("esp_turn");
  vTaskResume(wait);
}

//norm_voids
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
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, HIGH);
  Serial.begin(9600);
  while(!Serial) {
    ;
  }

  //Task def
  xTaskCreate (
    bluetick,  //function to be called
    "bluetick",//name of the task
    1024,      //stack size
    NULL,      //parameter to pass to the task
    1,         //task priority
    &wait);    //task handle 
  
  xTaskCreate (
    HUB,
    "HUB",
    1024,
    NULL,
    2,
    &HUB_handle);
  
  xTaskCreate (
    esp_turn,
    "esp_turn",
    1024,
    NULL,
    0,
    &esp_turn_handle);
  
  xTaskCreate (
    rasp_turn,
    "rasp_turn",
    1024,
    NULL,
    0,
    &rasp_turn_handle);
  
  //xTaskCreate (
  //  first_run,
  //  "first_run",
  //  1024,
  //  NULL,
  //  4, 
  //  &Busy_handle);
}

void loop() {
  // put your main code here, to run repeatedly:

}
