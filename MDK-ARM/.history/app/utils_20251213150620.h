#ifndef UTILS_H
#define UTILS_H
#include "MotorContrl.h"
#define MOTORNUMBER 4
extern const float k_;      // Smoothing factor (0 < k < 1)

float fof_update(Single_Motor input);

#endif
