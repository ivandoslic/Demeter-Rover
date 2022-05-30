#include <WiFi.h>
#include <ArduinoWebsockets.h>

// ↓ Adjust the pins ↓
#define MOTOR_RIGHT_IN1 19
#define MOTOR_RIGHT_IN2 16
#define MOTOR_RIGHT_EN 5

#define MOTOR_LEFT_IN1 17
#define MOTOR_LEFT_IN2 15
#define MOTOR_LEFT_EN 4

#define STEP_PIN 3
#define DIR_PIN 52
#define BREAK_SWITCH_LEFT 35
#define BREAK_SWITCH_RIGHT 6
#define STEPPER_NOT_EN_PIN 8

#define BUTTON_A 1000
#define BUTTON_B 1001
#define BUTTON_C 1002
#define BUTTON_D 1003
#define BUTTON_E 1004
#define BUTTON_F 1005

int movementMode = 0; // ► Variable that determines what type of movement the rover will do at the point in time

// ►  0 - IDLE
// ►  1 - FORWARD
// ►  2 - BACK
// ►  3 - LEFT
// ►  4 - RIGHT
// ►  5 - TURRET TO LEFT
// ►  6 - TURRET TO RIGHT

// PWM properties & definitions:
const int frequency = 30000;
const int pwmChannelRight = 0;
const int pwmChannelLeft = 1;
const int resolution = 8;
int dutyCycle = 200;

// ↓ WiFi configuration (Strings that are used to connect to LAN which hosts the server)
const char* ssid = "RoverAP";
const char* password = "demeterjesranje";

// ↓ Variable for stepper direction
bool stepperDir = 0;

// ↓ Variables for button logic
int activeButton = BUTTON_A;  // Determines which button is currently pressed
bool isButtonPressed = false;  // Determines if button is pressed

using namespace websockets;

WebsocketsClient client;

TaskHandle_t Task1; // Task that will be done on first core of ESP32
TaskHandle_t Task2; // Task that will be done on second core of ESP32

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  pinMode(MOTOR_LEFT_EN, OUTPUT);

  pinMode(MOTOR_RIGHT_IN1, OUTPUT);
  pinMode(MOTOR_RIGHT_IN2, OUTPUT);
  pinMode(MOTOR_RIGHT_EN, OUTPUT);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(BREAK_SWITCH_LEFT, INPUT_PULLUP);
  pinMode(BREAK_SWITCH_RIGHT, INPUT_PULLUP);

  pinMode(BUTTON_A, OUTPUT);
  pinMode(BUTTON_B, OUTPUT);
  pinMode(BUTTON_C, OUTPUT);
  pinMode(BUTTON_D, OUTPUT);
  pinMode(BUTTON_E, OUTPUT);
  pinMode(BUTTON_F, OUTPUT);

  // Setting up the PWM channels on ESP and "connecting" them to dedicated pins
  ledcSetup(pwmChannelRight, frequency, resolution);
  ledcSetup(pwmChannelLeft, frequency, resolution);
  ledcAttachPin(MOTOR_RIGHT_EN, pwmChannelRight);
  ledcAttachPin(MOTOR_LEFT_EN, pwmChannelLeft);

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){    // Establishing connection to WiFi which makes wireless control possible
      Serial.print(".");
      delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connecting to server");

  while(!client.connect("ws://192.168.1.18:5000/device")){    // Establishing connection to the local server which makes wireless control possible
    Serial.print(".");
    delay(100);
  }
  
  Serial.println("Connected to server");
  client.send("init-esp");    // Sends the message with which the server can acknowledge that this device is ESP32 (Rover), for deeper understanding look at server code
  
  client.onMessage([&](WebsocketsMessage message){  // A simple callback function which will get data (body) of the message we got from the server and pass it to executeInstruction
      executeInstruction(message.data());           // function which will change movementMode accordingly
  });
  
  xTaskCreatePinnedToCore(
                    Task1code,   // Function in which all code for first thread if contained
                    "Task1",     // We need to give it some name
                    10000,       
                    NULL,        
                    1,           
                    &Task1,      
                    0);         // Last parameter is the ID of a core we want our task to run on
  delay(500);

  xTaskCreatePinnedToCore(
                    Task2code,   // Function in which all code for second thread if contained
                    "Task2",     
                    10000,       
                    NULL,        
                    1,           
                    &Task2,      
                    1);         // Last parameter is the ID of a core we want our task to run on
  delay(500);
}

void goForward() {
  ledcWrite(pwmChannelRight, 255);
  ledcWrite(pwmChannelLeft, 255);
  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
}

void goBackwards() {
  ledcWrite(pwmChannelRight, 255);
  ledcWrite(pwmChannelLeft, 255);
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, HIGH);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
}

void turnRight() {
  ledcWrite(pwmChannelRight, 100);
  ledcWrite(pwmChannelLeft, 255);
  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
}

void turnLeft() {
  ledcWrite(pwmChannelRight, 255);
  ledcWrite(pwmChannelLeft, 100);
  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
}

void turretLeft() {
  stepperDir = 0;
  digitalWrite(DIR_PIN, stepperDir);
  digitalWrite(STEPPER_NOT_EN_PIN, LOW);
}

void turretRight() {
  stepperDir = 1;
  digitalWrite(DIR_PIN, stepperDir);
  digitalWrite(STEPPER_NOT_EN_PIN, LOW);
}

void stopMoving() {
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  digitalWrite(STEPPER_NOT_EN_PIN, HIGH);
}

void executeInstruction(String instruction) {
  if(instruction == "f") {          // f stands for "forward"
      movementMode = 1;
  } else if(instruction == "b") {   // b stands for "back"
      movementMode = 2;
  } else if(instruction == "l") {   // l stands for "left"
      movementMode = 3;
  } else if(instruction == "r") {   // r stands for "right"
      movementMode = 4;
  } else if(instruction == "s") {   // s stands for "stop"
      movementMode = 0;
  } else if(instruction == "tl") {  // t stands for turret, l stands for "left" (same is for right)
      turretLeft();
  } else if(instruction == "tr") {
      turretRight();
  } else if(instruction == "ba") {  // b stands for button, a for the button A (same pattern goes for other buttons)
      activeButton = BUTTON_A;
      isButtonPressed = true;
  } else if(instruction == "bb") {
      activeButton = BUTTON_B;
      isButtonPressed = true;
  } else if(instruction == "bc") {
      activeButton = BUTTON_C;
      isButtonPressed = true;
  } else if(instruction == "bd") {
      activeButton = BUTTON_D;
      isButtonPressed = true;
  } else if(instruction == "be") {
      activeButton = BUTTON_E;
      isButtonPressed = true;
  } else if(instruction == "bf") {
      activeButton = BUTTON_F;
      isButtonPressed = true;
  } else if(instruction == "br") {  // b stands for button, r stands for "released"
      isButtonPressed = false;
      digitalWrite(activeButton, LOW);
  } else {  
    Serial.println(instruction); 
  }
}

void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  
  for(;;){
    if(client.available()) {
      client.poll();
    }
    switch(movementMode) {
      case 0:
        stopMoving();
        break;
      case 1:
        goForward();
        break;
      case 2:
        goBackwards();
        break;
      case 3:
        turnLeft();
        break;
      case 4:
        turnRight();
        break;
      default:
        stopMoving();
    }

    if(isButtonPressed) {
      digitalWrite(activeButton, HIGH);
    } else {
      digitalWrite(activeButton, LOW);
    }
  } 
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(STEP_PIN, HIGH);
    delay(5);
    digitalWrite(STEP_PIN, LOW);
    delay(5);
  }
}

void loop() {

}


// Additional methods that can be implemented for "better" or "safer" controlling:

// Soft starting the movement of DC motors so current spikes are not surpassing the current limit on driver
void softStart(){
  for(int i = 0; i < 255; i++) {
    if(movementMode == 1 || movementMode == 2) {
      ledcWrite(pwmChannelRight, i);
      ledcWrite(pwmChannelLeft, i);
    } else if(movementMode == 3) {
      if(i <= 100)
        ledcWrite(pwmChannelRight, i);
      ledcWrite(pwmChannelLeft, i);
    } else if(movementMode == 4) {
      if(i <= 100)
        ledcWrite(pwmChannelLeft, i);
      ledcWrite(pwmChannelRight, i);
    }
    delay(20);
  }
}

// When stepper "limiters" are added
void stepperWithLimiters(){
  for(;;){
    if((!digitalRead(BREAK_SWITCH_LEFT) && stepperDir) || (!digitalRead(BREAK_SWITCH_RIGHT) && !stepperDir) || (digitalRead(BREAK_SWITCH_RIGHT) && digitalRead(BREAK_SWITCH_LEFT))) {
      digitalWrite(STEP_PIN, HIGH);
      delay(5);
      digitalWrite(STEP_PIN, LOW);
      delay(5);
    }
  }
}
