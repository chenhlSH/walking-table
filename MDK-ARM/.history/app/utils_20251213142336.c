#include "utils.h"
const float k_=0.1;
float fof_update(float input) {

  input_ = input;
  output_ = output_ * (1 - k_) + input_ * k_;
  return output_;
	
}

#define WINDOW_SIZE 5
#define NUM_WHEELS 4

// 为每个轮子的每种状态分别建立数组
float buffer[NUM_WHEELS][WINDOW_SIZE];  // 历史数据缓冲区
int index[NUM_WHEELS] = {0};            // 各轮子的缓冲区索引
float sum[NUM_WHEELS] = {0.0f};        // 各轮子的数据和

// 初始化
void init_filters() {
    for(int w = 0; w < NUM_WHEELS; w++) {
        for(int i = 0; i < WINDOW_SIZE; i++) {
            buffer[w][i] = 0.0f;
        }
        index[w] = 0;
        sum[w] = 0.0f;
    }
}

// 滤波函数（指定轮子序号）
float update_filter(int wheel_id, float input) {
    // 减去旧数据，加上新数据
    sum[wheel_id] -= buffer[wheel_id][index[wheel_id]];
    buffer[wheel_id][index[wheel_id]] = input;
    sum[wheel_id] += input;
    
    // 更新索引
    index[wheel_id]++;
    if(index[wheel_id] >= WINDOW_SIZE) {
        index[wheel_id] = 0;
    }
    
    return sum[wheel_id] / WINDOW_SIZE;
}

// 使用示例
float wheel_speed = get_speed(2);  // 获取第3个轮子速度
float filtered = update_filter(2, wheel_speed);  // 滤波处理

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