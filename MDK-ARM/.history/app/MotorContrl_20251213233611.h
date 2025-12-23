#ifndef __PWM_H
#define	__PWM_H
#include "stm32f4xx_hal.h"       // ��HAL��ͷ�ļ�
#include "stm32f4xx_hal_tim.h"   // ��ʱ��HAL��ͷ�ļ�
#include "main.h"
#include "cmsis_os.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern const float k_;
typedef struct			 				
{
  uint8_t mode;		    /* 手柄的工作模式 */

  uint8_t btn1;         /* B0:SLCT B1:JR  B0:JL B3:STRT B4:UP B5:R B6:DOWN  B7:L   */

  uint8_t btn2;         /* B0:L2   B1:R2  B2:L1 B3:R1   B4:Y  B5:B B6:A     B7:X */

  uint8_t RJoy_LR;      /* 右边摇杆  0x00 = 左    0xff = 右   */

  uint8_t RJoy_UD;      /* 右边摇杆  0x00 = 上    0xff = 下   */

  uint8_t LJoy_LR;      /* 左边摇杆  0x00 = 左    0xff = 右   */

  uint8_t LJoy_UD;      /* 左边摇杆  0x00 = 上    0xff = 下   */
	
}JOYSTICK_TypeDef;

// 单个电机结构体定义
typedef struct {
    float buffer[WINDOW_SIZE]; // 滑动窗口缓冲区
    int index;                 // 当前缓冲区索引
    float sum;                 // 窗口内数据的和
} Single_Motor;

typedef struct {
    Single_Motor motors[MOTORNUMBER]; // 多个电机的滤波器
} Chassic_State;
    
#define MOTORNUMBER 4
extern const float k_;      // Smoothing factor (0 < k < 1)

float fof_update(Single_Motor input);
static JOYSTICK_TypeDef table_state;
void Set_PWM(TIM_HandleTypeDef *htim,int motor_left,int motor_right);
void Motor_contrl(JOYSTICK_TypeDef JOYSTICK);
void Stop_PWM(TIM_HandleTypeDef *htim);
void Start_PWM(TIM_HandleTypeDef *htim);

#endif

