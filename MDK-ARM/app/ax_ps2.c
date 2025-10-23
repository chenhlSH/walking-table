#include "ax_ps2.h"


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
