#include "init.h"

static void initGpio(void);
static void initUart(void);


/**
 * @brief Функция, инициализирующая периферию
 * 
 */
void initMcu(){
    initGpio();
    initUart();
}


/**
 * @brief Функция, инициализирующая GPIO для USART1
 * 
 */
void initGpio(){
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_DeInit(GPIOA);
    //tx  - PA9
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    //rx  - PA10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/**
 * @brief Функция, инициализирующая UART со следующими характеристиками:
 *  460800/8-N-1.
 * 
 */
void initUart(){
    USART_InitTypeDef UART_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    USART_Cmd(USART1, DISABLE);
    
    USART_DeInit(USART1);
    UART_InitStruct.USART_BaudRate = 460800;
    UART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    UART_InitStruct.USART_Parity = USART_Parity_No;
    UART_InitStruct.USART_StopBits = USART_StopBits_1;
    UART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &UART_InitStruct);    
    USART_Cmd(USART1, ENABLE);	
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);
}
