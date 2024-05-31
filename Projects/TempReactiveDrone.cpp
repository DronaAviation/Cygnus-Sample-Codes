#include "PlutoPilot.h"
#include "Sensor.h"
#include "Utils.h"

void plutoInit() {
    // No spare hardware to be initialized
}

void onLoopStart() {
    LED.flightStatus(DEACTIVATE); // Disable default LED behaviour
}

void plutoLoop() {
    // Temperature reactive logic
}

void onLoopFinish() {
    LED.flightStatus(ACTIVATE); // Enable default LED behaviour
}
