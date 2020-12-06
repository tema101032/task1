#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "stm32f10x_usart.h"
#include "string.h"
#include "ring_buffer.h"

#define UART_BUFFER_SIZE (1024U)

#define PROTOCOL_SYNC_DATA (0xFF)
#define PROTOCOL_SIZE_WITHOUT_DATA (4)
#define PROTOCOL_SIZE_CRC (1)


/**
 * Структура для хранения принятого корректного пакета
 */
typedef __packed struct{
    uint8_t preamble;      // преамбула = 0xFF
    uint8_t cnt;           // счетчик пакетов
    uint8_t type;          // тип пакета
    uint8_t lenght;        // длина поля данных
    uint8_t data[255 + 1]; // массив данных, плюс место под CRC8
} packet_t;

// функция обработки принятых сообщений
void parseProtocolMsg(void);
// Инициализация кольцевого буфера
void initProtocolBuf(void);

#endif
