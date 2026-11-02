#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// --- L293D Pin Assignments (Left & Right Swapped) ---
const int LEFT_MOTOR_IN1  = 21; // Assigned to physical Left Wheel
const int LEFT_MOTOR_IN2  = 22; 
const int RIGHT_MOTOR_IN3 = 18; // Assigned to physical Right Wheel
const int RIGHT_MOTOR_IN4 = 19; 

// --- PWM Configuration (LEDC) ---
const int PWM_FREQ       = 1000;
const int PWM_RESOLUTION = 8;

// Driving Speed (80 to 255)
int baseSpeed = 220;

void setMotorPWM(int leftSpeed, int rightSpeed) {
  // Left Motor Control
  if (leftSpeed > 0) {
    ledcWrite(LEFT_MOTOR_IN1, constrain(leftSpeed, 0, 255));
    ledcWrite(LEFT_MOTOR_IN2, 0);
  } else if (leftSpeed < 0) {
    ledcWrite(LEFT_MOTOR_IN1, 0);
    ledcWrite(LEFT_MOTOR_IN2, constrain(abs(leftSpeed), 0, 255));
  } else {
    ledcWrite(LEFT_MOTOR_IN1, 0);
    ledcWrite(LEFT_MOTOR_IN2, 0);
  }

  // Right Motor Control
  if (rightSpeed > 0) {
    ledcWrite(RIGHT_MOTOR_IN3, constrain(rightSpeed, 0, 255));
    ledcWrite(RIGHT_MOTOR_IN4, 0);
  } else if (rightSpeed < 0) {
    ledcWrite(RIGHT_MOTOR_IN3, 0);
    ledcWrite(RIGHT_MOTOR_IN4, constrain(abs(rightSpeed), 0, 255));
  } else {
    ledcWrite(RIGHT_MOTOR_IN3, 0);
    ledcWrite(RIGHT_MOTOR_IN4, 0);
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize Dabble BLE
  Dabble.begin("UGV-ESP32");

  // Core 3.x unified LEDC attachment
  ledcAttach(LEFT_MOTOR_IN1,  PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(LEFT_MOTOR_IN2,  PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(RIGHT_MOTOR_IN3, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(RIGHT_MOTOR_IN4, PWM_FREQ, PWM_RESOLUTION);

  setMotorPWM(0, 0);
  Serial.println("UGV BLE Ready.");
}

void loop() {
  Dabble.processInput();

  if (GamePad.isUpPressed()) {
    setMotorPWM(baseSpeed, baseSpeed);                // Both forward
  } 
  else if (GamePad.isDownPressed()) {
    setMotorPWM(-baseSpeed, -baseSpeed);              // Both reverse
  } 
  else if (GamePad.isLeftPressed()) {
    setMotorPWM(-baseSpeed * 0.75, baseSpeed * 0.75); // Left reverse, Right forward -> Turns Left
  } 
  else if (GamePad.isRightPressed()) {
    setMotorPWM(baseSpeed * 0.75, -baseSpeed * 0.75); // Left forward, Right reverse -> Turns Right
  } 
  else {
    setMotorPWM(0, 0);                                // Stop
  }

  // Speed Adjustment
  if (GamePad.isTrianglePressed()) {
    baseSpeed = min(255, baseSpeed + 20);
    delay(150);
  }
  if (GamePad.isCrossPressed()) {
    baseSpeed = max(100, baseSpeed - 20);
    delay(150);
  }
}