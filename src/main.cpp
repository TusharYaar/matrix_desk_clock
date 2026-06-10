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
#define LED_RX_PIN 15
#define RGB_LED_PIN 8

// #define BUZZER_PIN 4
// #define BUTTON_DOWN 5
// #define BUTTON_UP 15

// // #define SCK_PIN 42
// // #define WS_PIN 41
// // #define SD_PIN 40

#define R_SW_PIN 22
#define R_DT_PIN 3
#define R_CLK_PIN 23

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define MATRIX_CLK_PIN 2
#define MATRIX_DATA_PIN 0
#define MATRIX_CS_PIN 1

#define DIRECTION_CW  0   // clockwise direction
#define DIRECTION_CCW 1   // counter-clockwise direction


volatile int counter = 0;
volatile int direction = DIRECTION_CW;
volatile int CLK_state;
volatile int prev_CLK_state;

MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, MATRIX_DATA_PIN, MATRIX_CLK_PIN, MATRIX_CS_PIN, MAX_DEVICES);

// // Variables will change:
// int lastState = LOW; // the previous state from the input pin
// int downButtonState; // the current reading from the input pin
// int upButtonState;

const char ssid[] = "prakhar24";
const char pwd[] = "12345678";

const unsigned long MsecPeriod = 1000;
unsigned long msec0;
int buttonState = HIGH;

AppMode currentMode = CLOCK;

void initializePins()
{
  // pinMode(BUZZER_PIN, OUTPUT);
  // pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(R_SW_PIN, INPUT_PULLUP);
  pinMode(R_CLK_PIN, INPUT);
  pinMode(R_DT_PIN, INPUT);
  // pinMode(BUTTON_HOME, INPUT_PULLUP);
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

void IRAM_ATTR ISR_encoder() {
  // if ((millis() - last_time) < 50)  // debounce time is 50ms
  //   return;

  if (digitalRead(R_DT_PIN) == HIGH) {
    // the encoder is rotating in counter-clockwise direction => decrease the counter
    counter-=1;
    direction = DIRECTION_CCW;
    Serial.println("Counter-Clockwise");
    currentMode = (AppMode)((currentMode - 1 + MODE_COUNT) % MODE_COUNT);
  } else {
    // the encoder is rotating in clockwise direction => increase the counter
    counter+=1;
    direction = DIRECTION_CW;
    Serial.println("Clockwise");
    currentMode = (AppMode)((currentMode + 1) % MODE_COUNT);
  }

  // last_time = millis();
}

void setup()
{
  Serial.begin(115200);
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  initializePins();
  Serial.print("Initializing display...");
  setupDisplay();
  // digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  // digitalWrite(BUZZER_PIN, LOW);
  setupWifi();
  initializeClock();
  // prev_CLK_state = digitalRead(R_CLK_PIN);
  attachInterrupt(digitalPinToInterrupt(R_CLK_PIN), ISR_encoder, CHANGE);
}

void loop()
{
  // CLK_state = digitalRead(R_CLK_PIN);
// if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
//   // Handle clockwise rotation
//     if (digitalRead(R_DT_PIN) == HIGH) {
//       counter--;
//       currentMode = (AppMode)((currentMode + 1) % MODE_COUNT);
//     } 
//     // Handle counter-clockwise rotation
//     else {
//       counter++;
//       currentMode = (AppMode)((currentMode - 1 + MODE_COUNT) % MODE_COUNT);
//     }
// }
  buttonState = digitalRead(R_SW_PIN);
  // if (buttonState == LOW)
  // {
  //   if (currentMode > 0)
  //   {
  //     currentMode = (AppMode)(currentMode - 1);
  //   }
  // }
  // buttonState = digitalRead(BUTTON_UP);
  // if (buttonState == LOW)
  // {
  //   if (currentMode < MODE_COUNT - 2)
  //   {
  //     currentMode = (AppMode)(currentMode + 1);
  //   }
  // }
  // buttonState = digitalRead(BUTTON_HOME);
  // if (buttonState == LOW)
  // {
  //   currentMode = CLOCK;
  // }

  prev_CLK_state = CLK_state;

  switch (currentMode)
  {
  case CLOCK:
    showClock();
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
