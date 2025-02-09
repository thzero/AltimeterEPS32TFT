#include <stdio.h>
#include <inttypes.h>
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "constants.h"
#include "loopThrottle.h"
#include "utilities.h"

// Assumed environmental values
float altitudeBarometer = 1650.3;  // meters ... map readings + barometer position
int altitudeOffsetLiftoff = 20;
int altitudeOffsetGround = altitudeOffsetLiftoff / 2;
int timeoutRecording = 300 * 1000;
int timeOutTimeToApogee = 2000;

//////////////////////////////////////////////////////////////////////
// Global variables
//////////////////////////////////////////////////////////////////////
enum loopStates {
  AIRBORNE_ASCENT,
  AIRBORNE_DESCENT,
  GROUND
};

struct loopStateMachine {
  enum loopStates current = GROUND;
} _loopState;

unsigned long _timestamp;

void setup() {
}

loopThrottle _throttleGround;
void loopStateGROUND(unsigned long timestamp, unsigned long deltaElapsed) {
  // Query the button handler to check for button press activity.
//   handleButtonLoop();

  // check wifi...
//   _wifi.loop();

  // Capture the command buffer.
//   if (readSerial(timestamp, deltaElapsed))
//     interpretCommandBuffer();  // TODO: It'd be nice to kick this to the other processor...

    // Determine the ground loop time delay based on sampling rate.
    int delta = _throttleGround.determine(deltaElapsed, (int)SAMPLE_RATE_GROUND);
    // printf("_throttleGround.delta...%d\n", delta);
    if (delta == 0)
        return;

    printf("hello...\n");

  // // Functionality that happen on the tick goes here:

  // // Get the current altitude and determine the delta from initial.
  // float altitude = readSensorAltitude();
  // float altitudeDelta = altitude - _flightLogger.data.altitudeInitial;
  // _flightLogger.data.altitudeCurrent = altitude;

  // if (_throttleGround.signal()) {
  //   debug("stateGROUND...processing, delta", delta);
  //   debug("stateGROUND...altitude", altitude);
  //   debug("stateGROUND...altitudeInitial", _flightLogger.data.altitudeInitial);
  //   debug("stateGROUND...altitudeDelta", altitudeDelta);
  //   debug("stateGROUND...altitudeCurrent", _flightLogger.data.altitudeCurrent);
  // }

  // // Check for whether we've left the ground
  // // If the delta altitude is less than the specified liftoff altitude, then its on the ground.
  // // Lift altitude is a measurement of the difference between the initial altitude and current altitude.
  // debug("stateGROUND...altitudeOffsetLiftoff", altitudeOffsetLiftoff);
  // if (altitudeDelta > altitudeOffsetLiftoff) {
  //   // left the ground...
  //   debug("AIRBORNE!!!!");
  //   // Transition to the AIRBORNE_ASCENT ascent stage.
  //   loopStateGROUNDToAIRBORNE_ASCENT(timestamp);
  //   return;
  // }
}

void loop(void) {
    unsigned long current = esp_timer_get_time();
    unsigned long delta = current - _timestamp;

    // printf("\t...loop...\n");

    // Simple state machine for the flight...

    // printf("state...\n");
    // printf("%d\n", _loopState.current);
    // Ground state
    if (_loopState.current == GROUND) {
        // printf("state...GROUND");
        // Only blink while on the ground!
        // _neoPixelBlinker.blink(current, 500);
        // Run the ground state algorithms
        // printf("loopStateGROUND...\n");
        loopStateGROUND(current, delta);
    }
    if (_loopState.current == AIRBORNE_ASCENT) {
        // printf("state...AIRBORNE_ASCENT");
        // Only blink while on the ground!
        // _neoPixelBlinker.off();
        // Run the airborne ascent state algorithms
        // loopStateAIRBORNE_ASCENT(current, delta);
    }
    if (_loopState.current == AIRBORNE_DESCENT) {
        // printf("state...AIRBORNE_DESCENT");
        // Only blink while on the ground!
        // _neoPixelBlinker.off();
        // Run the airborne descent state algorithms
        // loopStateAIRBORNE_DESCENT(current, delta);
    }

    _timestamp = current;
}

extern "C" void app_main(void) {
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    deviceInfo();

    printf("Setup...\n");
    setup();
    printf("...setup\n");

    printf("Loop...\n");
    for (;;) {
        loop();
        vTaskDelay(1);
    }
    printf("...loop\n");

    fflush(stdout);

    // for (int i = 10; i >= 0; i--) {
    //     printf("Restarting in %d seconds...\n", i);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    // printf("Restarting now.\n");
    // fflush(stdout);
    // esp_restart();
}
