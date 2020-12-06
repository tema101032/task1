#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include "init.h"

//структура, описывающая кольцевой буфер
typedef struct
{
    uint8_t *buffer;           ///< Указатель на буфер-хранилище
    uint16_t idxIn;            ///< Номер ячейки для записи
    uint16_t idxOut;           ///< Номер ячейки для чтения
    uint16_t size;             ///< Размер Буфера-хранилища
} ringBuffer_t;

// Функция инициализации кольцевого буфера
void ringInit (ringBuffer_t *ring, uint8_t *buf, uint16_t size);
// Функция очистки кольцевого буфера
void ringClear(ringBuffer_t* buf);
// Функция загрузки байта в буфер
void ringPut( ringBuffer_t* buf, uint8_t symbol);
// Функция получения байта из буфера
uint8_t ringPop(ringBuffer_t *buf);
// Функция загрузки из кольцевого буфера в массив
void ringPopBuffr(ringBuffer_t *ringbuf, uint8_t *destination, uint16_t len);
// Функция для просмотра текущего элемента без его удаления
uint8_t ringShowCurrentByte(ringBuffer_t *ringbuf);
// Функция для просмотра количества полезных(непросмотренных) данных в буфере
uint16_t ringGetCount(const ringBuffer_t *buf);

#endif
