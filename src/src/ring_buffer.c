#include "ring_buffer.h"

/**
 * @brief Функция инициализации кольцевого буфера
 * 
 * @param ringbuf указатель на кольцевой буфер
 * @param buf указатель на буфер хранения 
 * @param size размер буфера
 */
void ringInit (ringBuffer_t *ringbuf, uint8_t *buf, uint16_t size){
    ringbuf->size = size;
    ringbuf->buffer = buf;
    ringClear(ringbuf);
}

/**
 * @brief Функция очистки кольцевого буфера
 * 
 * @param ringbuf указатель на кольцевой буфер
 */
void ringClear(ringBuffer_t* ringbuf){
    ringbuf->idxIn = 0;
    ringbuf->idxOut = 0;   
}

/**
 * @brief Функция загрузки байта в буфер
 * 
 * @param ringbuf указатель на кольцевой буфер
 * @param symbol байт для загрузки
 */
void ringPut(ringBuffer_t* ringbuf, uint8_t symbol){
    ringbuf->buffer[ringbuf->idxIn++] = symbol;
    if (ringbuf->idxIn >= ringbuf->size) ringbuf->idxIn = 0;   
}

/**
 * @brief Функция получения байта из буфера
 * 
 * @param ringbuf указатель на кольцевой буфер
 * @return uint8_t значение полученного байта
 */
uint8_t ringPop(ringBuffer_t *ringbuf){
    uint8_t retval = ringbuf->buffer[ringbuf->idxOut++];
    if (ringbuf->idxOut >= ringbuf->size) ringbuf->idxOut = 0;
    return retval;    
}

/**
 * @brief Функция загрузки из кольцевого буфера в массив
 * 
 * @param ringbuf указатель на кольцевой буфер
 * @param destination указатель массив
 * @param len количество получаемых байтов
 */
void ringPopBuffr(ringBuffer_t *ringbuf, uint8_t *destination, uint16_t len){
    while(len--) *(destination++) = ringPop(ringbuf);   
}

/**
 * @brief Функция для просмотра текущего элемента без его удаления
 * 
 * @param ringbuf указатель на кольцевой буфер
 * @return uint8_t значение текущего элемента
 */
uint8_t ringShowCurrentByte(ringBuffer_t *ringbuf){
    return ringbuf->buffer[ringbuf->idxOut];
}

/**
 * @brief Функция для просмотра количества полезных(непросмотренных) данных в буфере
 * 
 * @param ringbuf указатель на кольцевой буфер
 * @return uint16_t количество полезных(непросмотренных) данных в буфере
 */

uint16_t ringGetCount(const ringBuffer_t *ringbuf){
    uint16_t retval = 0;
    if (ringbuf->idxIn < ringbuf->idxOut) retval = ringbuf->size + ringbuf->idxIn - ringbuf->idxOut;
    else retval = ringbuf->idxIn - ringbuf->idxOut;
    return retval;
}
