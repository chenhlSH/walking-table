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



// 初始化滤波器部分


// 通用滤波函数（移动平均法示例）[1](@ref)

// 为4个轮子声明滤波器实例
Chassic_State chassis;

// 初始化所有滤波器
void Setup_Filters() {
    for(int i = 0; i < 4; i++) {
        Filter_Init(&wheel_filters[i]);
    }
}



const float k_=0.1;
float fof_update(Single_Motor input) {
  static float input_[MOTORNUMBER] = 0.0f;  // Current input value    
  static float output_[MOTORNUMBER] = 0.0f; // Current output value
  for (i=1; i<MOTORNUMBER; i++) {
    
    input_[i] = chassis.motor[i];  // Current input value    
    output_[i] = chassis.motor[i].lastspeed; // Current output value
    input_ = input;
    output_ = output_ * (1 - k_) + input_ * k_;
  }
    return output_;
	
}



int Velocity_A(int TargetVelocity, int CurrentVelocity)
{  
     int Bias;  //定义相关变量
		static int ControlVelocity, Last_bias; //静态变量，函数调用结束后其值依然存在
        Bias=TargetVelocity-CurrentVelocity; //求速度偏差
		
		ControlVelocity+=Velcity_Kp*(Bias-Last_bias)+Velcity_Ki*Bias;  //增量式PI控制器
                                                                   //Velcity_Kp*(Bias-Last_bias) 作用为限制加速度
	                                                                 //Velcity_Ki*Bias             速度控制值由Bias不断积分得到 偏差越大加速度越大
	    if(ControlVelocity>7200)ControlVelocity=7200;
		if(ControlVelocity<-7200)ControlVelocity=-7200;
		Last_bias=Bias;	
		return ControlVelocity; //返回速度控制值
}