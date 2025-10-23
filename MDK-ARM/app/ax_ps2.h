
///* Define to prevent recursive inclusion -------------------------------------*/
//#ifndef __AX_PS2_H
//#define __AX_PS2_H

//#include "main.h"
//#include "cmsis_os.h" // 提供FreeRTOS类型和API
//#include "usart.h"

///* 通过宏定义选择接收方式，当前使用中断方式 */
//#define UART_RX_MODE_IT 1

///* 消息队列相关定义 */
//#define UART_RX_QUEUE_LENGTH 20   // 队列深度，可存放20个单字节数据
//#define UART_RX_QUEUE_ITEM_SIZE sizeof(uint8_t)

///* 全局句柄声明 */
//extern osMessageQueueId_t uartRxQueueHandle; // 串口接收消息队列句柄
//extern osThreadId_t uartReceiveTaskHandle;    // 串口接收任务句柄

///* 函数声明 */
////void MX_USART1_UART_Init(void); // 串口初始化（CubeMX生成）
//void Start_UART_Receive_Task(void *argument); // 接收任务入口函数
//void UART_Rx_Callback_Handler(UART_HandleTypeDef *huart); // 回调函数封装处理

//#endif
