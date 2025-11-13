#include "ax_ps2.h"

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  /* 首先判断是哪个串口触发的中断，这在有多个串口时非常必要 */
  if (huart->Instance == USART1) // 假设是USART1
  {
    /* 这里是您的应用逻辑实现区 */
    // 1. 此时，DMA已经将数据填充至接收缓冲区的前半部分
    // 2. 您可以安全地处理这前半部分数据
    // 例如：Process_Received_Data(rx_buffer, RX_BUFFER_SIZE / 2);
	uart_rx_complete = 1;
	  
	 
    // 示例：点亮一个LED指示半传输发生
    // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
  }

  /* 如果还有其他串口（如USART2）也使用了半传输中断，可以继续添加判断 */
  // else if (huart->Instance == USART2)
  // {
  //   // 处理USART2的数据
  // }
}
// 解析摇杆数据函数
uint8_t parse_joystick_data(const char* data, JOYSTICK_TypeDef* joy) {
    const char* tokens[] = {"RJOY_LR:", "RJOY_UD:", "LJOY_LR:", "LJOY_UD:"};
    uint8_t* values[] = {&joy->RJoy_LR, &joy->RJoy_UD, &joy->LJoy_LR, &joy->LJoy_UD};
    int token_count = 4;
    int found_count = 0;
    
    // 确保字符串以空字符结尾[5](@ref)
    char working_data[RX_BUFFER_SIZE];
    memcpy(working_data, data, RX_BUFFER_SIZE);
    working_data[255] = '\0';
    
    for (int i = 0; i < token_count; i++) {
        char* pos = strstr(working_data, tokens[i]);
        if (pos != NULL) {
            // 跳过令牌文本，指向十六进制值[3](@ref)
            pos += strlen(tokens[i]);
            
            // 将十六进制字符串转换为数值[3](@ref)
            char hex_str[3] = {pos[0], pos[1],'\0'};
            *values[i] = (uint8_t)strtol(hex_str, NULL, 16);
            found_count++;
        }
    }
    memset(working_data, 0, RX_BUFFER_SIZE);
    // 如果成功找到并解析了所有4个值，返回成功[3](@ref)
    return 0;
}
