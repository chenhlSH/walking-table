#ifndef UTILS_H
#define UTILS_H
const float k_ = 0.1f;      // Smoothing factor (0 < k < 1)
static float input_ = 0.0f;  // Current input value    
static float output_ = 0.0f; // Current output value
float fof_update(float input);

#endif
