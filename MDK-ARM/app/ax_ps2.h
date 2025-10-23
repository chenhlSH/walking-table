#ifndef __AX_PS2_H
#define __AX_PS2_H

#include "main.h"
#include "cmsis_os.h" // 提供FreeRTOS类型和API
#include "usart.h"
#include "MotorContrl.h"
#include <string.h>  // 为 strncpy, strstr, strlen
#include <stdlib.h>  // 为 strtol
extern  JOYSTICK_TypeDef table_state;
uint8_t parse_joystick_data(const char* data, JOYSTICK_TypeDef* joy);


#endif
