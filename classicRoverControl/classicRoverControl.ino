#include <WiFi.h>
#include <ArduinoWebsockets.h>

// DC Motors:
#define MOTOR_RIGHT_IN1 4
#define MOTOR_RIGHT_IN2 16
#define MOTOR_RIGHT_EN 17

#define MOTOR_LEFT_IN1 5
#define MOTOR_LEFT_IN2 18
#define MOTOR_LEFT_EN 19

#define BUTTON_A 14
#define BUTTON_B 27
#define BUTTON_C 26
#define BUTTON_D 33 
#define BUTTON_E 25 
#define BUTTON_F 32 

#define STEPPER_DISABLE_PIN 23
#define STEPPER_DIRECTION_PIN 22

int movementMode = 0;

// ↓ PWM properties ↓
const int frequency = 30000;
const int pwmChannelRight = 0;
const int pwmChannelLeft = 1;
const int resolution = 8;
int dutyCycle = 200;

// ↓ WiFi ↓
const char* ssid = "RoverAP";
const char* password = "APDEMETER";

int buttonDuration = 150;
int pwrButtonDuration = 2500;

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

  pinMode(STEPPER_DISABLE_PIN, OUTPUT);
  pinMode(STEPPER_DIRECTION_PIN, OUTPUT);

  digitalWrite(STEPPER_DISABLE_PIN, HIGH);

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

void turnRight() {
  ledcWrite(pwmChannelRight, 100);
  ledcWrite(pwmChannelLeft, 100);
  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
}

void turnLeft() {
  ledcWrite(pwmChannelRight, 100);
  ledcWrite(pwmChannelLeft, 100);
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, HIGH);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
}

void goForward() {
  ledcWrite(pwmChannelRight, 100);
  ledcWrite(pwmChannelLeft, 100);
  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
}

void goBackwards() {
  ledcWrite(pwmChannelRight, 100);
  ledcWrite(pwmChannelLeft, 100);
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, HIGH);
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
}

void stopMoving() {
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
}

void turretLeft() {
  digitalWrite(STEPPER_DIRECTION_PIN, LOW);
  digitalWrite(STEPPER_DISABLE_PIN, LOW);
}

void turretRight() {
  digitalWrite(STEPPER_DIRECTION_PIN, HIGH);
  digitalWrite(STEPPER_DISABLE_PIN, LOW);
}

void turretStop() {
  digitalWrite(STEPPER_DISABLE_PIN, HIGH);
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
      turretLeft();
  } else if(instruction == "tr") {
      turretRight();
  } else if(instruction == "ts") {
      turretStop();
      Serial.println("Stopping turret");
  } else if(instruction == "ba") {
      digitalWrite(BUTTON_A, HIGH);
      delay(buttonDuration);
      digitalWrite(BUTTON_A, LOW);
  } else if(instruction == "bb") {
      digitalWrite(BUTTON_B, HIGH);
      delay(buttonDuration);
      digitalWrite(BUTTON_B, LOW);
  } else if(instruction == "bc") {
      digitalWrite(BUTTON_C, HIGH);
      delay(buttonDuration);
      digitalWrite(BUTTON_C, LOW);
  } else if(instruction == "bd") {
      digitalWrite(BUTTON_D, HIGH);
      delay(buttonDuration);
      digitalWrite(BUTTON_D, LOW);
  } else if(instruction == "be") {
      digitalWrite(BUTTON_E, HIGH);
      delay(buttonDuration);
      digitalWrite(BUTTON_E, LOW);
  } else if(instruction == "bf") {
      digitalWrite(BUTTON_F, HIGH);
      delay(buttonDuration);
      digitalWrite(BUTTON_F, LOW);
  } else if(instruction == "pwr") {
      digitalWrite(BUTTON_A, HIGH);
      delay(pwrButtonDuration);
      digitalWrite(BUTTON_A, LOW);
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
  
  delay(200);
}
