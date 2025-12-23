#include "MotorContrl.h"
#include "utils.h"

const float k_=0.1;  // Smoothing factor (0 < k < 1)

// 为4个轮子声明滤波器实例
Chassic_State chassis;

Velocity_Input v_input;// 速度输入结构体实例

/*************************************************************
函数功能：PWM赋值
入口参数：motor_left:左电机PWM值，motor_right:右电机PWM值
返回  值：无
*************************************************************/
void Motor_init(void)
{
    stop_PWM(&htim3);
    stop_PWM(&htim5);
    start_PWM(&htim3);
    start_PWM(&htim5);
    Setup_Filters();
}

void Motor_contrl(JOYSTICK_TypeDef JOYSTICK)
{  
   
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




// 初始化所有滤波器
void Setup_Filters() {
    for(int i = 0; i < 4; i++) {
         static float input_[MOTORNUMBER] = 0.0f;  // Current input value    
  
         static float output_[MOTORNUMBER] = 0.0f; // Current output value
    }
}




Chassic_State fof_update(Chassic_State input) {

  for (i=1; i<MOTORNUMBER; i++) {
    
    input_[i] = chassis.motor[i].targetspeed;  // Current input value    
    output_[i] = chassis.motor[i].lastspeed; // Current output value

    output_[i] = output_[i] * (1 - k_) + input_[i] * k_;
  }
    return output_;
	
}



// int Velocity_A(int TargetVelocity, int CurrentVelocity)
// {  
//      int Bias;  //定义相关变量
// 		static int ControlVelocity, Last_bias; //静态变量，函数调用结束后其值依然存在
//         Bias=TargetVelocity-CurrentVelocity; //求速度偏差
		
// 		ControlVelocity+=Velcity_Kp*(Bias-Last_bias)+Velcity_Ki*Bias;  //增量式PI控制器
//                                                                    //Velcity_Kp*(Bias-Last_bias) 作用为限制加速度
// 	                                                                 //Velcity_Ki*Bias             速度控制值由Bias不断积分得到 偏差越大加速度越大
// 	    if(ControlVelocity>7200)ControlVelocity=7200;
// 		if(ControlVelocity<-7200)ControlVelocity=-7200;
// 		Last_bias=Bias;	
// 		return ControlVelocity; //返回速度控制值
// }




// 全局底盘参数
static Chassis_Params chassis_params;

/**
 * @brief 初始化底盘参数
 * @param length 车长（前后轮距）
 * @param width 车宽（左右轮距）
 * @param wheel_radius 轮子半径
 */
// void kinematics_init(float length, float width, float wheel_radius) {
//     chassis_params.length = length;
//     chassis_params.width = width;
//     chassis_params.wheel_radius = wheel_radius;
// }

/**
 * @brief 全向轮运动学逆解计算
 * @param input 速度输入（vx, vy, omega）
 * @param motor_speeds 输出参数，四电机速度数组
 * 
 * 轮子编号约定：
 * 0: 前左轮  1: 前右轮
 * 2: 后左轮  3: 后右轮
 * 
 * 安装角度：45°（典型全向轮安装方式）
 */
void omni_wheel_inverse_kinematics(Velocity_Input input, Chassic_State chassis) {
    float half_len = chassis_params.length / 2.0f;
    float half_wid = chassis_params.width / 2.0f;
    
    // 计算旋转半径（考虑长宽比影响）
    float R = sqrtf(half_len * half_len + half_wid * half_wid);
    
    // 45°角的正弦余弦值（sqrt(2)/2）
    float sin45 = 0.707106781f;  // sqrt(2)/2
    float cos45 = 0.707106781f;  // sqrt(2)/2
    
    // 全向轮运动学逆解公式[4,6](@ref)
    // 考虑轮子安装角度为45°的情况
    chassis.motors[0].targetspeed = ( input.vx * cos45 - input.vy * cos45 - input.omega * R) / chassis_params.wheel_radius;  // 前左轮
    chassis.motors[1].targetspeed = (-input.vx * cos45 - input.vy * cos45 - input.omega * R) / chassis_params.wheel_radius;  // 前右轮
    chassis.motors[2].targetspeed = ( input.vx * cos45 + input.vy * cos45 + input.omega * R) / chassis_params.wheel_radius;  // 后左轮
    chassis.motors[4].targetspeed = (-input.vx * cos45 + input.vy * cos45 + input.omega * R) / chassis_params.wheel_radius;  // 后右轮
    
    // 注：如果轮子安装角度不同，需要调整上述公式中的角度参数[7](@ref)
}

/**
 * @brief 限制电机速度范围
 * @param motor_speeds 电机速度数组（rad/s）
 * @param max_speed 最大允许速度（rad/s）
 */
// void limit_motor_speeds(float motor_speeds[4], float max_speed) {
//     float max_current = 0.0f;
    
//     // 找到当前最大速度
//     for (int i = 0; i < 4; i++) {
//         if (fabsf(motor_speeds[i]) > max_current) {
//             max_current = fabsf(motor_speeds[i]);
//         }
//     }
    
//     // 如果超过最大速度，按比例缩放
//     if (max_current > max_speed) {
//         float scale = max_speed / max_current;
//         for (int i = 0; i < 4; i++) {
//             motor_speeds[i] *= scale;
//         }
//     }
// }

