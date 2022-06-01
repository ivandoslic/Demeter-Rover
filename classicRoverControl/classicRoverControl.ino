#include <WiFi.h>
#include <ArduinoWebsockets.h>

// DC Motors:
#define MOTOR_RIGHT_IN1 4
#define MOTOR_RIGHT_IN2 16
#define MOTOR_RIGHT_EN 17

#define MOTOR_LEFT_IN1 5
#define MOTOR_LEFT_IN2 18
#define MOTOR_LEFT_EN 19

#define BUTTON_A 34
#define BUTTON_B 35
#define BUTTON_C 32
#define BUTTON_D 33
#define BUTTON_E 25
#define BUTTON_F 26

int movementMode = 0;

// ↓ PWM properties ↓
const int frequency = 30000;
const int pwmChannelRight = 0;
const int pwmChannelLeft = 1;
const int resolution = 8;
int dutyCycle = 200;

// ↓ WiFi ↓
const char* ssid = "DOSLIC";
const char* password = "31012104";

int activeButton = BUTTON_A;  // Determines which button is currently pressed
bool isButtonPressed = false;

using namespace websockets;

WebsocketsClient client;

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  pinMode(MOTOR_LEFT_EN, OUTPUT);

  pinMode(MOTOR_RIGHT_IN1, OUTPUT);
  pinMode(MOTOR_RIGHT_IN2, OUTPUT);
  pinMode(MOTOR_RIGHT_EN, OUTPUT);

  pinMode(BUTTON_A, OUTPUT);
  pinMode(BUTTON_B, OUTPUT);
  pinMode(BUTTON_C, OUTPUT);
  pinMode(BUTTON_D, OUTPUT);
  pinMode(BUTTON_E, OUTPUT);
  pinMode(BUTTON_F, OUTPUT);

  ledcSetup(pwmChannelRight, frequency, resolution);
  ledcSetup(pwmChannelLeft, frequency, resolution);
  ledcAttachPin(MOTOR_RIGHT_EN, pwmChannelRight);
  ledcAttachPin(MOTOR_LEFT_EN, pwmChannelLeft);

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connecting to server");

  while(!client.connect("ws://192.168.1.18:5000/device")){
    Serial.print(".");
    delay(100);
  }
  
  Serial.println("Connected to server");
  client.send("init-esp");
  
  client.onMessage([&](WebsocketsMessage message){
      executeInstruction(message.data());
  });
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

void stopMoving() {
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
}

void executeInstruction(String instruction) {
  if(instruction == "f") {
      movementMode = 1;
  } else if(instruction == "b") {
      movementMode = 2;
  } else if(instruction == "l") {
      movementMode = 3;
  } else if(instruction == "r") {
      movementMode = 4;
  } else if(instruction == "s") {
      movementMode = 0;
  } else if(instruction == "tl") {
      Serial.println("Truning turret to the left");
  } else if(instruction == "tr") {
      Serial.println("Truning turret to the right");
  } else if(instruction == "ba") {
      activeButton = BUTTON_A;
      isButtonPressed = true;
      Serial.println("Button A pressed");
  } else if(instruction == "bb") {
      activeButton = BUTTON_B;
      isButtonPressed = true;
      Serial.println("Button B pressed");
  } else if(instruction == "bc") {
      activeButton = BUTTON_C;
      isButtonPressed = true;
      Serial.println("Button C pressed");
  } else if(instruction == "bd") {
      activeButton = BUTTON_D;
      isButtonPressed = true;
      Serial.println("Button D pressed");
  } else if(instruction == "be") {
      activeButton = BUTTON_E;
      isButtonPressed = true;
      Serial.println("Button E pressed");
  } else if(instruction == "bf") {
      activeButton = BUTTON_F;
      isButtonPressed = true;
      Serial.println("Button F pressed");
  } else if(instruction == "br") {
      isButtonPressed = false;
      digitalWrite(activeButton, LOW);
      Serial.println("Button released");
  } else {  
    Serial.println(instruction); 
  }
}

void loop(){
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
  
  delay(200);
}
