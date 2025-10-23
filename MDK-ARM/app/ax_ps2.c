#include "ax_ps2.h"

///* 全局变量定义 */
//osMessageQueueId_t uartRxQueueHandle = NULL;
//osThreadId_t uuartReceiveTaskHandle = NULL;
//uint8_t uart_rx_byte = 0; // 中断接收单字节数据的缓冲区

///**
//  * @brief  初始化USART1（通常由CubeMX在main.c中生成，此处为示例）
//  * @param  无
//  * @retval 无
//  */
////void MX_USART1_UART_Init(void)
////{
////  // 此函数内容通常由CubeMX根据你的配置自动生成在main.c中
////  // 主要包括huart1的Instance、波特率、字长、停止位、校验位、模式等初始化
////  // 确保在NVIC Settings中使能了USART1全局中断
////}

///**
//  * @brief  创建串口接收相关的FreeRTOS组件（在main.c的初始化后调用）
//  * @param  无
//  * @retval 无
//  */
//void Create_UART_Related_Objects(void)
//{
//  /* 创建消息队列 */
//  const osMessageQueueAttr_t uart_rx_queue_attributes = {
//    .name = "uart_rx_queue"
//  };
//  uartRxQueueHandle = osMessageQueueNew(UART_RX_QUEUE_LENGTH, UART_RX_QUEUE_ITEM_SIZE, &uart_rx_queue_attributes);

//  /* 创建串口接收任务 */
//  const osThreadAttr_t uart_receive_task_attributes = {
//    .name = "UARTReceiveTask",
//    .stack_size = 1024 * 4, // 可根据需要调整堆栈大小
//    .priority = osPriorityAboveNormal, // 给予较高优先级以便及时处理数据
//  };
//  uartReceiveTaskHandle = osThreadNew(Start_UART_Receive_Task, NULL, &uart_receive_task_attributes);
//}

///**
//  * @brief  串口接收任务主体函数
//  * @param  argument: FreeRTOS任务参数
//  * @retval 无
//  */
//void Start_UART_Receive_Task(void *argument)
//{
//  uint8_t received_byte;
//  osStatus_t queue_status;
//  /* 初始化：启动第一次串口接收中断 */
//  HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);

//  /* 任务主循环 */
//  for (;;) {
//    /* 等待消息队列，最大等待时间可取portMAX_DELAY */
//    queue_status = osMessageQueueGet(uartRxQueueHandle, &received_byte, NULL, portMAX_DELAY);

//    if (queue_status == osOK) {
//      /* 在这里处理接收到的单个字节数据 'received_byte' */

//      // 示例1：简单回显（可用于测试）
//      HAL_UART_Transmit(&huart1, &received_byte, 1, 100);

//      // 示例2：自定义协议解析（关键部分）
//      // 您可以在此处实现状态机，根据帧头、帧尾、长度等规则
//      // 将连续的单个字节拼接成完整的一帧数据。
//      // 当一帧数据接收完成后，可以通过另一个队列、任务通知、事件标志组等
//      // 将完整的帧数据发送给等待处理的其他应用任务。
//      // 例如：xQueueSend(applicationTaskQueue, &complete_frame, 0);
//    }
//    // 注意：osDelay不应在此使用，因为osMessageQueueGet已经阻塞了任务
//    // 如果需要周期性的执行其他操作，可以考虑使用定时器或在协议解析逻辑中控制
//  }
//}

///**
//  * @brief  串口接收完成回调函数（在中断服务程序上下文被调用）
//  * @param  huart: 串口句柄
//  * @retval 无
//  */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  BaseType_t xHigherPriorityTaskWoken = pdFALSE; // 默认为假

//  if (huart->Instance == USART1) {
//    /* 1. 将接收到的数据发送到消息队列（从中断安全函数发送） */
//    if (osMessageQueuePut(uartRxQueueHandle, &uart_rx_byte, 0, 0) != osOK) {
//      // 处理队列已满的情况，例如可以丢弃数据或记录错误
//      // 例如：HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//    }

//    /* 2. 立即重新启动串口接收中断，准备接收下一个字节 */
//    HAL_UART_Receive_IT(huart, &uart_rx_byte, 1);

//    /* 如果需要，可以进行任务切换（由内核决定） */
//    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//  }
//}
