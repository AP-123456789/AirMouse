
#include <Arduino.h>
#include <Wire.h>
#include <HijelHID_BLEMouse.h>

// ================= PIN CONFIGURATION =================

#define SDA_PIN 6
#define SCL_PIN 7

#define LEFT_CLICK_PIN 0
#define RIGHT_CLICK_PIN 1
#define CURSOR_ENABLE_PIN 2

#define STATUS_LED_PIN 8

// ================= MPU6050 =================

#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B

// ================= BLE MOUSE =================

HijelBLEMouse mouse("AirMouse", "AirMouse Project");

// ================= SETTINGS =================

const float CURSOR_SENSITIVITY = 0.015f;
const int DEAD_ZONE = 1800;
const int MAX_CURSOR_SPEED = 25;

const unsigned long DEBOUNCE_MS = 250;

// ================= VARIABLES =================

bool cursorEnabled = false;

int16_t accelX = 0;
int16_t accelY = 0;
int16_t accelZ = 0;

bool lastEnableButton = HIGH;
bool lastLeftButton = HIGH;
bool lastRightButton = HIGH;

unsigned long lastEnableTime = 0;
unsigned long lastLeftTime = 0;
unsigned long lastRightTime = 0;

// ================= MPU INITIALIZATION =================

bool initializeMPU6050() {
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(400000);

    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_PWR_MGMT_1);
    Wire.write(0x00);

    if (Wire.endTransmission() != 0) {
        Serial.println("MPU6050 not detected!");
        return false;
    }

    delay(100);

    Serial.println("MPU6050 initialized");
    return true;
}

// ================= READ MPU6050 =================

bool readMPU6050() {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_ACCEL_XOUT_H);

    if (Wire.endTransmission(false) != 0) {
        return false;
    }

    if (Wire.requestFrom(MPU6050_ADDR, 6, true) != 6) {
        return false;
    }

    accelX = (Wire.read() << 8) | Wire.read();
    accelY = (Wire.read() << 8) | Wire.read();
    accelZ = (Wire.read() << 8) | Wire.read();

    return true;
}

// ================= BUTTON HANDLING =================

void handleButtons() {
    unsigned long now = millis();

    bool enableState = digitalRead(CURSOR_ENABLE_PIN);

    if (enableState == LOW &&
        lastEnableButton == HIGH &&
        now - lastEnableTime > DEBOUNCE_MS) {

        cursorEnabled = !cursorEnabled;
        lastEnableTime = now;

        digitalWrite(STATUS_LED_PIN, cursorEnabled);

        Serial.println(cursorEnabled ?
                       "Cursor ENABLED" :
                       "Cursor DISABLED");
    }

    lastEnableButton = enableState;

    bool leftState = digitalRead(LEFT_CLICK_PIN);

    if (leftState == LOW &&
        lastLeftButton == HIGH &&
        now - lastLeftTime > DEBOUNCE_MS) {

        if (mouse.isPaired()) {
            mouse.click(MouseButton::Left);
            Serial.println("Left click");
        }

        lastLeftTime = now;
    }

    lastLeftButton = leftState;

    bool rightState = digitalRead(RIGHT_CLICK_PIN);

    if (rightState == LOW &&
        lastRightButton == HIGH &&
        now - lastRightTime > DEBOUNCE_MS) {

        if (mouse.isPaired()) {
            mouse.click(MouseButton::Right);
            Serial.println("Right click");
        }

        lastRightTime = now;
    }

    lastRightButton = rightState;
}

// ================= CURSOR MOVEMENT =================

void handleCursorMovement() {
    if (!cursorEnabled || !mouse.isPaired()) {
        return;
    }

    if (!readMPU6050()) {
        return;
    }

    int x = accelY;
    int y = accelX;

    if (abs(x) < DEAD_ZONE) x = 0;
    if (abs(y) < DEAD_ZONE) y = 0;

    int moveX = (int)(x * CURSOR_SENSITIVITY);
    int moveY = (int)(y * CURSOR_SENSITIVITY);

    moveX = constrain(moveX, -MAX_CURSOR_SPEED, MAX_CURSOR_SPEED);
    moveY = constrain(moveY, -MAX_CURSOR_SPEED, MAX_CURSOR_SPEED);

    if (moveX != 0 || moveY != 0) {
        mouse.move(moveX, moveY);
    }
}

// ================= SETUP =================

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("===== AirMouse Starting =====");

    pinMode(LEFT_CLICK_PIN, INPUT_PULLUP);
    pinMode(RIGHT_CLICK_PIN, INPUT_PULLUP);
    pinMode(CURSOR_ENABLE_PIN, INPUT_PULLUP);

    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);

    initializeMPU6050();

    mouse.begin();

    Serial.println("BLE Mouse advertising started");
    Serial.println("Pair AirMouse with your computer");
}

// ================= MAIN LOOP =================

void loop() {
    handleButtons();
    handleCursorMovement();

    delay(10);
}
