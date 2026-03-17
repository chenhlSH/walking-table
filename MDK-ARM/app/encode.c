#include "encode.h"
#include "main.h"

// 全局变量存储编码器总计数（处理溢出）
static int32_t encoder1_total_count = 0;
static int32_t encoder2_total_count = 0;
static int32_t encoder3_total_count = 0;
static int32_t encoder4_total_count = 0;
static int16_t encoder1_last_cnt = 0;
static int16_t encoder2_last_cnt = 0;
static int16_t encoder3_last_cnt = 0;
static int16_t encoder4_last_cnt = 0;

// 开始所有编码器
void Encoder_StartAll(void)
{
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
    
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim8);
}

// 读取编码器增量值（单位时间的计数变化）
int16_t Read_Encoder(uint8_t encoder_num)
{
    int16_t encoder_cnt = 0;
    
    switch(encoder_num)
    {
        case ENCODER1:
            encoder_cnt = (int16_t)__HAL_TIM_GET_COUNTER(&htim1);
            __HAL_TIM_SET_COUNTER(&htim1, 0);
            break;
            
        case ENCODER2:
            encoder_cnt = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
            __HAL_TIM_SET_COUNTER(&htim2, 0);
            break;
            
        case ENCODER3:
            encoder_cnt = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
            __HAL_TIM_SET_COUNTER(&htim4, 0);
            break;
            
        case ENCODER4:
            encoder_cnt = (int16_t)__HAL_TIM_GET_COUNTER(&htim8);
            __HAL_TIM_SET_COUNTER(&htim8, 0);
            break;
            
        default:
            encoder_cnt = 0;
            break;
    }
    
    return encoder_cnt;
}

// 获取编码器总计数（带溢出处理）
int32_t Get_Encoder_Count(uint8_t encoder_num)
{
    int32_t count = 0;
    
    switch(encoder_num)
    {
        case ENCODER1:
            count = encoder1_total_count;
            break;
            
        case ENCODER2:
            count = encoder2_total_count;
            break;
            
        case ENCODER3:
            count = encoder3_total_count;
            break;
            
        case ENCODER4:
            count = encoder4_total_count;
            break;
            
        default:
            count = 0;
            break;
    }
    
    return count;
}

// 清除编码器计数
void Clear_Encoder(uint8_t encoder_num)
{
    switch(encoder_num)
    {
        case ENCODER1:
            encoder1_total_count = 0;
            __HAL_TIM_SET_COUNTER(&htim1, 0);
            break;
            
        case ENCODER2:
            encoder2_total_count = 0;
            __HAL_TIM_SET_COUNTER(&htim2, 0);
            break;
            
        case ENCODER3:
            encoder3_total_count = 0;
            __HAL_TIM_SET_COUNTER(&htim4, 0);
            break;
            
        case ENCODER4:
            encoder4_total_count = 0;
            __HAL_TIM_SET_COUNTER(&htim8, 0);
            break;
    }
}

// 编码器中断处理函数（处理溢出）
void Encoder_Overflow_Handler(TIM_HandleTypeDef *htim)
{
    int16_t current_cnt = 0;
    
    if(htim->Instance == TIM1)
    {
        current_cnt = __HAL_TIM_GET_COUNTER(&htim1);
        if((encoder1_last_cnt > 30000) && (current_cnt < -30000))
            encoder1_total_count += 65536;
        else if((encoder1_last_cnt < -30000) && (current_cnt > 30000))
            encoder1_total_count -= 65536;
        encoder1_last_cnt = current_cnt;
    }
    else if(htim->Instance == TIM2)
    {
        current_cnt = __HAL_TIM_GET_COUNTER(&htim2);
        if((encoder2_last_cnt > 30000) && (current_cnt < -30000))
            encoder2_total_count += 65536;
        else if((encoder2_last_cnt < -30000) && (current_cnt > 30000))
            encoder2_total_count -= 65536;
        encoder2_last_cnt = current_cnt;
    }
    else if(htim->Instance == TIM4)
    {
        current_cnt = __HAL_TIM_GET_COUNTER(&htim4);
        if((encoder3_last_cnt > 30000) && (current_cnt < -30000))
            encoder3_total_count += 65536;
        else if((encoder3_last_cnt < -30000) && (current_cnt > 30000))
            encoder3_total_count -= 65536;
        encoder3_last_cnt = current_cnt;
    }
    else if(htim->Instance == TIM8)
    {
        current_cnt = __HAL_TIM_GET_COUNTER(&htim8);
        if((encoder4_last_cnt > 30000) && (current_cnt < -30000))
            encoder4_total_count += 65536;
        else if((encoder4_last_cnt < -30000) && (current_cnt > 30000))
            encoder4_total_count -= 65536;
        encoder4_last_cnt = current_cnt;
    }
}



