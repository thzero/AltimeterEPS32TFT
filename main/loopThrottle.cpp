#include "constants.h"
#include "debug.h"
#include "loopThrottle.h"

loopThrottle::loopThrottle() {
}

int loopThrottle::determine(unsigned long deltaElapsed, int sampleRate) {
  unsigned long delta = deltaElapsed + _delta;
  int samples = 1000000 / sampleRate;
  
// #ifdef DEBUG
//   // if (_countDebugI % 100000 == 0) {
//     debug(" ");
//     debug("deltaElapsed", deltaElapsed);
//     debug("_delta", _delta);
//     debug("delta", delta);
//     debug("sampleRate", sampleRate);
//     debug("samples", ((float)sampleRate / 60));
//     debug("samples", samples);
//     debug("(delta <= samples)", (delta <= samples));
//     // _countDebugI = 0;
//   // }
//   // _countDebugI++;
// #endif

  if (delta <= samples) {
    _countDebug++;
    _delta = delta;
    return 0;
  }

#ifdef DEBUG
  debug("...process");
#endif
  _countDebug = 0;
  _delta = 0;
  return delta;
};

void loopThrottle::reset() {
// #ifdef DEBUG
  // _countDebugI = 0;
// #endif
  _countDebug = 0;
  _delta = 0;
};

bool loopThrottle::signal() {
  return _countDebug % 100 == 0;
};