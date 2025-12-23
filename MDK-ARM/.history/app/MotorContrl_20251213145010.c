#include "MotorContrl.h"
#include "utils.h"

/*************************************************************
函数功能：PWM赋值
入口参数：motor_left:左电机PWM值，motor_right:右电机PWM值
返回  值：无
*************************************************************/
void Motor_contrl(JOYSTICK_TypeDef JOYSTICK)
{  
   Start_PWM(&htim3);
   Start_PWM(&htim5);
   Set_PWM(&htim3,0,0);
   Set_PWM(&htim5,fof_update((JOYSTICK.RJoy_LR-0x80)*50),fof_update(-(JOYSTICK.RJoy_UD-0x7f)*50));
}

void Set_PWM(TIM_HandleTypeDef *htim,int motor_left,int motor_right)
{	motor_left = (motor_left > 7200) ? 7200 : motor_left;
   motor_left = (motor_left < -7200) ? -7200 : motor_left;
   motor_right = (motor_right > 7200) ? 7200 : motor_right;
   motor_right = (motor_right < -7200) ? -7200 : motor_right;

   if(motor_left>0)   
   {  
	   __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,7200);
	   __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,7200-motor_left);
   }
   else
   {
	   __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,7200+motor_left);
	   __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,7200);
   }
   if(motor_right>0)
   {
	   __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3,7200);
	   __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4,7200-motor_right);
   }
   else
   {
	   __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3,7200+motor_right);
	   __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4,7200);
   }
	
	
}
void Stop_PWM(TIM_HandleTypeDef *htim)
{
   HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
   HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_2);
   HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_3);
   HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_4);
}
void Start_PWM(TIM_HandleTypeDef *htim)
{
   HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
   HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
   HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3);
   HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4);
} 

// 滤波器结构体定义
typedef struct {
    float buffer[WINDOW_SIZE]; // 滑动窗口缓冲区
    int index;                 // 当前缓冲区索引
    float sum;                 // 窗口内数据的和
} MovingAverageFilter;

// 初始化滤波器
void Filter_Init(MovingAverageFilter* filter) {
    for(int i = 0; i < WINDOW_SIZE; i++) {
        filter->buffer[i] = 0.0f;
    }
    filter->index = 0;
    filter->sum = 0.0f;
}

// 通用滤波函数（移动平均法示例）[1](@ref)
float Filter_Update(MovingAverageFilter* filter, float input) {
    // 减去即将被覆盖的旧数据
    filter->sum -= filter->buffer[filter->index];
    // 存入新数据
    filter->buffer[filter->index] = input;
    // 加上新数据
    filter->sum += input;
    
    // 更新索引（循环缓冲区）
    filter->index++;
    if(filter->index >= WINDOW_SIZE) {
        filter->index = 0;
    }
    
    // 返回平均值
    return filter->sum / WINDOW_SIZE;
}

// 为4个轮子声明滤波器实例
MovingAverageFilter wheel_filters[4];

// 初始化所有滤波器
void Setup_Filters() {
    for(int i = 0; i < 4; i++) {
        Filter_Init(&wheel_filters[i]);
    }
}

// 使用示例：处理第2个轮子（索引1）的新数据
float new_speed = get_wheel_speed(1);  // 获取第2个轮子的新数据
float filtered_speed = Filter_Update(&wheel_filters[1], new_speed);