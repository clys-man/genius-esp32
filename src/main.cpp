#include <Arduino.h>
#include <ezButton.h>
#include <song.h>

#define RED_BTN_PIN  22
#define BLUE_BTN_PIN 23
#define YELLOW_BTN_PIN 21
#define GREEN_BTN_PIN 19
#define RED_LED_PIN 5
#define BLUE_LED_PIN 15
#define YELLOW_LED_PIN 4
#define GREEN_LED_PIN 18
#define DEBOUNCE_TIME  50
#define ROUNDS 4

ezButton redButton(RED_BTN_PIN);
ezButton blueButton(BLUE_BTN_PIN);
ezButton yellowButton(YELLOW_BTN_PIN);
ezButton greenButton(GREEN_BTN_PIN);

u_int8_t GAME_LEDS[ROUNDS] = {BLUE_LED_PIN, YELLOW_LED_PIN, RED_LED_PIN, GREEN_LED_PIN};
u_int8_t GAME_BUTTONS[ROUNDS] = {BLUE_BTN_PIN, YELLOW_BTN_PIN, RED_BTN_PIN, GREEN_BTN_PIN};

int current_round = 1;
int clicks = 0;
bool game_started = false;
bool show_options = true;
bool win = false;

int lastSteadyState = LOW;  
int lastFlickerableState = LOW;
int currentState;
unsigned long lastDebounceTime = 0;

template < typename T>
int sizeOfArray(T array)
{
  return (sizeof(array)/sizeof(*array));
}

void setMultiplesPinModes(u_int8_t pins[], u_int8_t mode) {
  int size = sizeOfArray(pins);

  for (int i = 0; i < size; i++) {
    pinMode(pins[i], mode);
  }
}

void setup() {
  Serial.begin(9600);

  setMultiplesPinModes(GAME_LEDS, OUTPUT);

  redButton.setDebounceTime(DEBOUNCE_TIME);
  blueButton.setDebounceTime(DEBOUNCE_TIME);
  yellowButton.setDebounceTime(DEBOUNCE_TIME);
  greenButton.setDebounceTime(DEBOUNCE_TIME);
}

void shuffleLedsAndButtonsInSamePosition() {
  for (int i = 0; i < ROUNDS ; i++)
  {
      size_t j = random(0, ROUNDS);

      int t = GAME_LEDS[i];
      GAME_LEDS[i] = GAME_LEDS[j];
      GAME_LEDS[j] = t;

      t = GAME_BUTTONS[i];
      GAME_BUTTONS[i] = GAME_BUTTONS[j];
      GAME_BUTTONS[j] = t;
  }
}

void startGame() {
  if (game_started) {
    return;
  }

  shuffleLedsAndButtonsInSamePosition();

  game_started = true;
}

void showOptions() {
  if (!show_options) {
    return;
  }

  int decresing_delay = - (current_round*100);

  for (int i = 0; i < current_round; i++) {
    digitalWrite(GAME_LEDS[i], LOW);
    delay(100);
    digitalWrite(GAME_LEDS[i], HIGH);
    delay(500);
    digitalWrite(GAME_LEDS[i], LOW);
    delay(500);
  }

  show_options = false;
}

void checkIfKeyPressedIsCorrect(u_int8_t key_pressed) {
  digitalWrite(GAME_LEDS[clicks], HIGH);

  if (key_pressed == GAME_BUTTONS[clicks]) {
    Serial.println("Correct!");
    clicks++;
  } else {
    Serial.println("You lose!");

    while (true)
    {
      for (int i = 0; i < ROUNDS; i++) {
        digitalWrite(GAME_LEDS[i], HIGH);
      }
      delay(1000);
      for (int i = 0; i < ROUNDS; i++) {
        digitalWrite(GAME_LEDS[i], LOW);
      }
      delay(1000);
    }
  }
}

void checkPlayerOption () {
  if (redButton.isPressed()) {
    checkIfKeyPressedIsCorrect(RED_BTN_PIN);
  } else if (blueButton.isPressed()) {
    checkIfKeyPressedIsCorrect(BLUE_BTN_PIN);
  } else if (yellowButton.isPressed()) {
    checkIfKeyPressedIsCorrect(YELLOW_BTN_PIN);
  } else if (greenButton.isPressed()) {
    checkIfKeyPressedIsCorrect(GREEN_BTN_PIN);
  } 

  if (clicks == current_round) {
    current_round++;
    clicks = 0;
    show_options = true;
  }

  if (current_round > ROUNDS) {
    show_options = false;
    win = true;
    Serial.println("You win!");
    while (true)
    {
      for (int i = 0; i < ROUNDS; i++) {
        digitalWrite(GAME_LEDS[i], HIGH);
      }
      delay(100);
      for (int i = 0; i < ROUNDS; i++) {
        digitalWrite(GAME_LEDS[i], LOW);
      }
      delay(100);
    }

    return;
  }
}

void loop() {
  redButton.loop();
  blueButton.loop();
  yellowButton.loop();
  greenButton.loop();

  startGame();
  showOptions();
  checkPlayerOption();
}

