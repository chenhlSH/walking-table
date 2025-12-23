#include "utils.h"

const float k_=0.1;
float fof_update(Single_Motor input) {
  static float input_[MOTORNUMBER] = 0.0f;  // Current input value    
  static float output_[MOTORNUMBER] = 0.0f; // Current output value
  for (i=1; i<MOTORNUMBER; i++) {
    
    input_[i] = Single_Motor;  // Current input value    
    output_[i] = Single_Motor; // Current output value
    input_ = input;
    output_ = output_ * (1 - k_) + input_ * k_;
    
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