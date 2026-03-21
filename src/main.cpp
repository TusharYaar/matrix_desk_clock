#include <WiFi.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Arduino.h>

// #include <bounce_animation.h>
// #include <matrix_animation.h>
#include "manager.h"
#include "matrix_animation.h"
#include "bounce_animation.h"
#include "random_animation.h"
#include "clock.h"
#include "snake.h"

// Define the pins for the LEDs
#define LED_TX_PIN 43
#define LED_RX_PIN 44
#define RGB_LED_PIN 48

#define BUZZER_PIN 4
#define BUTTON_DOWN 5
#define BUTTON_UP 15
#define BUTTON_HOME 40

// #define SCK_PIN 42
// #define WS_PIN 41
// #define SD_PIN 40

// #define DIN_PIN 11
// #define BLCK_PIN 12
// #define LRCLK_PIN 13

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define MATRIX_CLK_PIN 21
#define MATRIX_DATA_PIN 17
#define MATRIX_CS_PIN 18

MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, MATRIX_DATA_PIN, MATRIX_CLK_PIN, MATRIX_CS_PIN, MAX_DEVICES);

// Variables will change:
int lastState = LOW; // the previous state from the input pin
int downButtonState; // the current reading from the input pin
int upButtonState;

const char ssid[] = "prakhar24";
const char pwd[] = "12345678";

const unsigned long MsecPeriod = 1000;
unsigned long msec0;
int buttonState = HIGH;

AppMode currentMode = CLOCK;

void initializePins()
{
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_HOME, INPUT_PULLUP);
}

void setupWifi()
{
  WiFi.begin(ssid, pwd);
  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  // mx.begin();
  // mx.control(MD_MAX72XX::INTENSITY, 1); // Set brightness (0-15)
  // mx.clear();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // matrix.displayZoneText(0, ".", PA_LEFT, matrix.getSpeed(), matrix.getPause(), PA_NO_EFFECT);
    // matrix.displayAnimate();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupDisplay()
{
  matrix.begin();
  matrix.clear();
  matrix.control(MD_MAX72XX::INTENSITY, 6);
}

void setup()
{
  Serial.begin(115200);
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  initializePins();
  setupDisplay();
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  setupWifi();
  initializeClock();
}

void loop()
{
  buttonState = digitalRead(BUTTON_DOWN);
  if (buttonState == LOW)
  {
    if (currentMode > 0)
    {
      currentMode = (AppMode)(currentMode - 1);
    }
  }
  buttonState = digitalRead(BUTTON_UP);
  if (buttonState == LOW)
  {
    if (currentMode < MODE_COUNT - 2)
    {
      currentMode = (AppMode)(currentMode + 1);
    }
  }
  buttonState = digitalRead(BUTTON_HOME);
  if (buttonState == LOW)
  {
    currentMode = CLOCK;
  }

  switch (currentMode)
  {
  case CLOCK:
    handleSnakeGame();
    // showClock();
    break;
  case MATRIX:
    playMatrixAnimation();
    break;

  case BOUNCE:
    playBounceAnimation();
    break;

  case RANDOM:
    playRandomAnimation();
    break;

  default:
    showClock();
    break;
  }
}
