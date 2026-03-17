#ifndef __ENCODE_H
#define __ENCODE_H

#include "stm32f4xx_hal.h"

// 编码器句柄（在main.c中声明）
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;

// 函数声明
void Encoder_StartAll(void);
int16_t Read_Encoder(uint8_t encoder_num);
void Clear_Encoder(uint8_t encoder_num);
int32_t Get_Encoder_Count(uint8_t encoder_num);
void Encoder_Overflow_Handler(TIM_HandleTypeDef *htim);

// 编码器编号定义
#define ENCODER1 1
#define ENCODER2 2
#define ENCODER3 3
#define ENCODER4 4

#endif
