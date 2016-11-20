// This #include statement was automatically added by the Particle IDE.
#include "InternetButton/InternetButton.h"


InternetButton b = InternetButton();
int successLED = 0;
int INTERVAL = 25; // 255 / 10 intervals is approximately 25
int NUM_LIGHTS = 11;
int HALF_FLASH_PERIOD = 500;
int buttonProcessingDelay = 3000;
int successButton = 2;
int resetButton = 3;

void setup() {
    b.begin();
    // For response from server for button press
    Particle.subscribe("buttonPressAck", buttonResponseHandler, MY_DEVICES);

}

void buttonResponseHandler(const char *event, const char *data) {
  // Handle the integration response
  if (strcmp(data, "success") == 0) {
      //digitalWrite(successLED, HIGH); Board LED
      b.ledOn(successLED, 0, successLED * INTERVAL, (NUM_LIGHTS -  (++successLED)) * INTERVAL); // success only registered if response received
  }
}

void turnOffAllLights() {
    for (int led = 0; led < NUM_LIGHTS; led ++) {
        b.ledOff(led);
    }
}

void flashGreen() {
    for (int led = 0; led < NUM_LIGHTS; led ++) {
        b.ledOn(led, 0, 255, 0);
    }
    delay(HALF_FLASH_PERIOD);
    turnOffAllLights();
    delay(HALF_FLASH_PERIOD);
}

void resetContract() {
    successLED = 0;
}

void loop() {
    if (successLED == NUM_LIGHTS) { // all lights turn green when successful
        flashGreen();
        if (b.buttonOn(resetButton)) {
            resetContract();
        }
    }
// Get some data
    else { // new success button press only registered if user resets
      String data = String(successLED + 1);
      // Trigger the integration
      if (b.buttonOn(successButton)) {
        Particle.publish("buttonPress", data, PRIVATE);
        delay(buttonProcessingDelay);  // Wait 3 seconds
      }
    }
}