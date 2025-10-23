#ifndef UTILS_H
#define UTILS_H
float k_ = 0.1f;      // Smoothing factor (0 < k < 1)
float input_ = 0.0f;  // Current input value    
float output_ = 0.0f; // Current output value
float update(float input);

#endif