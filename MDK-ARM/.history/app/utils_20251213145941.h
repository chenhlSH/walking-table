#ifndef UTILS_H
#define UTILS_H
#include "MotorContrl.h"
#define motornumber 4
extern const float k_;      // Smoothing factor (0 < k < 1)

float fof_update(float input);

#endif
