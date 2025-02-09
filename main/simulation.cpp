#include <math.h>
#include "esp_timer.h"

#include "debug.h"
#include "simulation.h"
// #include "tft.h"

struct simulationConfig simulationConfigDefault;

simulation::simulation() {
}

bool simulation::isRunning() {
  return _running;
}

void simulation::evaluateTimestep(double K, double deltaT, double Mr, double Me, double G, double Ft, double *x) {
  // Leap-frog Euler method -- interleave calculation of DeltaX, DeltaV
  // Move the position
  _trace[0] += deltaT * _trace[1];
  // Compute the total force
  double F = -G * Me * Mr / (_trace[0] * _trace[0]);
  double dragSign = (_trace[1] < 0) ? 1.0 : -1.0;
  F += 0.5 * K * (_trace[1] * _trace[1]) * dragSign;
  F += Ft;
  // Apply acceleration
  _trace[1] += deltaT * F / Mr;
}

void simulation::outputPrint(double delta, double thrust, double mass, double height) {
  printf("sim -\t%f\n", _elapsedTime);
  printf("\t%f\n", delta);
  printf("\t%f\n", thrust);
  printf("\t%f\n", mass);
  printf("\t%f\n", _trace[1]);
  printf("\t\t%f\n", _startingAltitude);
  printf("\t\t%f\n", _trace[0]);
  printf("\t%f\n", height);
  printf("\t%s\n", _airborne ? "true" : "false");
}

void simulation::outputPrintHeader() {
  printf("sim -\tTime\tDelta\tThrust\tMass\tVelocity\tStarting Altitude\tPosition\tHeight\tAirborne\n");
}

void simulation::loopStep(double deltaT, bool output) {
  deltaT = deltaT / 1000;

  if (_airborne && _trace[0] <= _startingAltitude) {
    outputPrint(deltaT, 0, 0, 0);
    printf("sim -\tLANDED\n");
    printf("sim -\tMax. Height=%f\n", _maxHeight);
    printf("sim -\tMax. Velocity=%f\n", _maxVelocity);
    printf("\n");
    _running = false;
    return;
  }

  double height = _trace[0] - _startingAltitude;
  double airDensity = AirDensity * pow(0.5, height / AirDensityScale);
  double drag = airDensity * _config.CrossSection * _config.DragCoefficient;

  double mass = _config.RocketMass;
  double thrust = 0;
  if (_elapsedTime < _burnoutTime) {
    mass = _config.RocketMass + _config.MotorFuelMass - _config.MotorFuelBurnRate * _elapsedTime;
    thrust = _motorThrust; // N
  }

  if (output)
    outputPrint(deltaT, thrust, mass, height);

  evaluateTimestep(drag, deltaT, mass, EarthMass, GravConstant, thrust, _trace);
  // TODO: Introduce some  point - recommendation seems to be to use Simplex noise from FastLED

  if (_trace[0] > _startingAltitude)
    _airborne = true;

  if (height > _maxHeight)
    _maxHeight = height;
  if (_trace[1] > _maxVelocity)
    _maxVelocity = _trace[1];

  _elapsedTime += deltaT;
}

void simulation::simulationTaskW(void * parameter) {
    simulation* instance = reinterpret_cast<simulation*>(parameter) ; // obtain the instance pointer
    instance->simulationTask() ; // dispatch to the member function, now that we have an instance pointer
}

void simulation::simulationTask() {
  printf("Simulation task...\n");
  printf("sim -\tSample Rate=%d\n", _config.sampleRate);
  
  // drawTftSplashSim();

  unsigned long start = esp_timer_get_time();
  int count = 0;
  int countHeader = 0;
  int settle = 0;
  while(_running) {
    // debug("Simulation _simulationTimestamp...", _simulationTimestamp);
    unsigned long current = esp_timer_get_time();
    // debug("Simulation current..."), current);

    // unsigned long deltaT = current - _simulationTimestamp;
    // debug("Simulation deltaT..."), deltaT);

    bool output = false;
    bool outputHeader = false;

    int deltaElapsed = current - _simulationTimestamp;
    int delta = _simulationThrottle.determine(deltaElapsed, _config.sampleRate);
    // debug("Simulation delta..."), delta);
    if (delta != 0) {
      // debug("Simulation settle..."), settle);
      // let the loop settle before starting to simulate...
      if (settle < 5) {
        settle++;
        continue;
      }

      if (count % 25 == 0) {
        count = 0;
        output = true;
        if (countHeader % 10 == 0) {
          countHeader = 0;
          countHeader++;
          outputHeader = true;
        }
        countHeader++;
      }
      count++;

      if (outputHeader)
        outputPrintHeader();

      // debug("Simulation, delta"), delta);
      loopStep(delta, output);
    }

    _simulationTimestamp = current;
  }
  
  unsigned long stop = esp_timer_get_time();
  unsigned long runtime = stop - start;
  printf("sim -\tStart=%lu\n", start);
  printf("sim -\tStop=%lu\n", stop);
  printf("sim -\tRuntime (ms)=%lu\n", runtime);
  printf("sim -\tRuntime (s)=%lu\n", runtime / 1000);

  // drawTftSplashSimStop();

  // Delete the task...
  // vTaskDelete(NULL);
}

void simulation::start(simulationConfig startConfig, long initialAltitude) {
  _config = startConfig;
  printf("\n");
  printf("Simulation Started\n");
  if (_running) {
    printf("\tAlready running.\n");
    return;
  }

  _airborne = false;
  _burnoutTime = _config.MotorFuelMass / _config.MotorFuelBurnRate; // s
  _elapsedTime = 0.0;
  _maxHeight = 0;
  _maxVelocity = 0;
  _motorThrust = _config.MotorExhaustVelocity * _config.MotorFuelBurnRate; // N
  _startingAltitude = EarthRadius + initialAltitude;
  _trace[0] = _startingAltitude; // Initial position
  _trace[1] = 0.0; // Initial velocity

  // printf("sim -\tTime\tDelta\tThrust\tMass\tVelocity\tStarting Altitude\tPosition\tHeight\tAirborne\n");

  _running = true;
  // printf("Simulation\tCreating task...\n");
  // BaseType_t  xReturned = xTaskCreatePinnedToCore(
  //   &simulation::simulationTaskW, /* Function to implement the task */
  //   "simulationTask", /* Name of the task */
  //   4000,  /* Stack size in words */
  //   this,  /* Task input parameter */
  //   0,  /* Priority of the task */
  //   &simulationTaskHandle,  /* Task handle. */
  //   0 /* Core where the task should run */
  // );
  // // if (xReturned == pdPASS) {
  // //     /* The task was created. Use the task's handle to delete the task. */
  // //     vTaskDelete(simulationTaskHandle);
  // // }
}

void simulation::stop() {
  if (_running) {
    printf("Simulation\tStopped.\n");
    printf("\n");
    _running = false;
    return;
  }
  printf("Simulation\tNo simulation was running.\n");
  printf("\n");
}

double simulation::valueAltitude() {
  if (!_running)
    return 0;
  double altitude = _trace[0] - _startingAltitude;
  debug("simulation.altitude", altitude);
  return altitude;
}

simulation _simulation;