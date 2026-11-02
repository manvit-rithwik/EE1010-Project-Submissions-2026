#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#define INCLUDE_SENSOR_MODULE
#include <DabbleESP32.h>

// --- L293D Pin Assignments (Matched to Hardware Orientation) ---
const int LEFT_MOTOR_IN1  = 21; 
const int LEFT_MOTOR_IN2  = 22; 
const int RIGHT_MOTOR_IN3 = 18; 
const int RIGHT_MOTOR_IN4 = 19; 

// --- PWM Configuration (LEDC ESP32 Core 3.x) ---
const int PWM_FREQ       = 1000;
const int PWM_RESOLUTION = 8;    // 8-bit duty resolution (0 - 255)

// Base driving speed (80 to 255)
int baseSpeed = 220;

// Motor Drive Subroutine
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

  // Initialize Dabble BLE stack
  Dabble.begin("UGV-ESP32");

  // Core 3.x unified LEDC setup
  ledcAttach(LEFT_MOTOR_IN1,  PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(LEFT_MOTOR_IN2,  PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(RIGHT_MOTOR_IN3, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(RIGHT_MOTOR_IN4, PWM_FREQ, PWM_RESOLUTION);

  setMotorPWM(0, 0);
  Serial.println("UGV BLE Ready with Joystick & Accelerometer Support.");
}

void loop() {
  // Poll BLE packets from Dabble app
  Dabble.processInput();

  // Read Joystick / Accelerometer inputs from GamePad module
  // (In Dabble, selecting 'Accel Mode' maps tilt directly to X and Y axis channels)
  float x_axis = GamePad.getXaxisData();
  float y_axis = GamePad.getYaxisData();

  // 1. Proportional Drive (Analog Joystick & Accelerometer Tilt)
  // Threshold of 1.5 avoids drift when the phone is held loosely flat
  if (abs(x_axis) > 1.5 || abs(y_axis) > 1.5) {
    // Normalizes input range (-7.0 to +7.0) to current base speed
    float throttle = (y_axis / 7.0) * baseSpeed;
    float steering = (x_axis / 7.0) * (baseSpeed * 0.75);

    // Differential steering mixing
    int leftDrive  = (int)(throttle + steering);
    int rightDrive = (int)(throttle - steering);

    setMotorPWM(leftDrive, rightDrive);
  }
  // 2. Fallback: Digital D-Pad Buttons
  else if (GamePad.isUpPressed()) {
    setMotorPWM(baseSpeed, baseSpeed);                // Forward
  } 
  else if (GamePad.isDownPressed()) {
    setMotorPWM(-baseSpeed, -baseSpeed);              // Reverse
  } 
  else if (GamePad.isLeftPressed()) {
    setMotorPWM(-baseSpeed * 0.75, baseSpeed * 0.75); // Turn Left
  } 
  else if (GamePad.isRightPressed()) {
    setMotorPWM(baseSpeed * 0.75, -baseSpeed * 0.75); // Turn Right
  } 
  else {
    setMotorPWM(0, 0);                                // Stop / Idle
  }

  // 3. Dynamic Speed Trim Buttons
  if (GamePad.isTrianglePressed()) {
    baseSpeed = min(255, baseSpeed + 20); // Increase max speed
    delay(150);
  }
  if (GamePad.isCrossPressed()) {
    baseSpeed = max(100, baseSpeed - 20); // Decrease max speed
    delay(150);
  }
}