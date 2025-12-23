#ifndef UTILS_H
#define UTILS_H
#include MotorControl.h
extern const float k_;      // Smoothing factor (0 < k < 1)
static float input_ = 0.0f;  // Current input value    
static float output_ = 0.0f; // Current output value
float fof_update(float input);

#endif
