#include "protocol.h"

//структуры
packet_t gPacket;
ringBuffer_t gRingBuf;

//массив для хранения данных, пришедших по UART
uint8_t gDataBuf[UART_BUFFER_SIZE] = {0};

//статические функции
static void sendAnswer(uint8_t *data, uint16_t len);
static uint8_t calcCrc8(uint8_t *mas, uint8_t Len);

/**
 * @brief инициализация кольцевого буфера
 * 
 */
void initProtocolBuf(){
    ringInit(&gRingBuf, gDataBuf, UART_BUFFER_SIZE);
}


/**
 * @brief Функция, обрабатывающая принятые данные
 * 
 */
void parseProtocolMsg(){
    uint8_t crc8, calculatedCrc8;
    static uint8_t parsingInProcess = 0;
    static uint8_t packetWithoutDataWasRead = 0;
    // Если неверный заголовок, то дальше с сообщением не работаем
    if ((ringGetCount(&gRingBuf) > 0) && (parsingInProcess == 0)){
        if (ringShowCurrentByte(&gRingBuf) == PROTOCOL_SYNC_DATA){
            parsingInProcess = 1;
        }
        else{
            ringPop(&gRingBuf); //скипаем байт, который на соответствует преамбуле
            return;
        }
    }
    // Если мало данных - ничего не парсим
    if (ringGetCount(&gRingBuf) < (PROTOCOL_SIZE_WITHOUT_DATA)){
        return;
    }
    //Запись пакета без данных в струру, проверка на перезапись данных
    if (packetWithoutDataWasRead == 0){
        ringPopBuffr(&gRingBuf, (uint8_t*)&gPacket, PROTOCOL_SIZE_WITHOUT_DATA);
        packetWithoutDataWasRead = 1;
    }
    // Ждём приёма данных + crc
    if (ringGetCount(&gRingBuf) <= gPacket.lenght){
        return;
    }
    ringPopBuffr(&gRingBuf, gPacket.data, gPacket.lenght + PROTOCOL_SIZE_CRC);
    crc8 = gPacket.data[gPacket.lenght];//последний байт в поле data
    calculatedCrc8 = calcCrc8((uint8_t*)&gPacket, gPacket.lenght + PROTOCOL_SIZE_WITHOUT_DATA);
    if (crc8 == calculatedCrc8){
        gPacket.type |= 0x80; //установка старшего бита в 1
        crc8 = calcCrc8((uint8_t*)&gPacket, gPacket.lenght + PROTOCOL_SIZE_WITHOUT_DATA);//новый crc
        gPacket.data[gPacket.lenght] = crc8; //запись в структуру нового crc
        sendAnswer((uint8_t*)&gPacket, gPacket.lenght + PROTOCOL_SIZE_WITHOUT_DATA + PROTOCOL_SIZE_CRC); //отправка пакета назад к ведущему
    }
    parsingInProcess = 0;
    packetWithoutDataWasRead = 0;
}


/**
 * @brief Функция, отправляющая массив данных по UART1
 * 
 * @param data Адрес массива
 * @param len Размер массива
 */
static void sendAnswer(uint8_t *data, uint16_t len){
    uint16_t i = 0;
    for (i = 0; i < len; i++){
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, data[i]);
    }
}


/**
 * @brief Функция, рассчитывающая crc8 по полиному 0x31 
 * 
 * @param mas Адрес массива
 * @param Len Размер массива
 * @return Рассчитанный crc8
 */
static uint8_t calcCrc8(uint8_t *mas, uint8_t Len)
{
  uint8_t i,dat,crc,fb,st_byt;
  st_byt=0; crc=0;
  do{
    dat=mas[st_byt];
    for( i=0; i<8; i++) {  // счетчик битов в байте
      fb = crc ^ dat;
      fb &= 1;
      crc >>= 1;
      dat >>= 1;
      if( fb == 1 ) crc ^= 0x8c; // полином
    }
    st_byt++;
  } while( st_byt < Len ); // счетчик байтов в массиве
  return crc;
}


/**
 * @brief Обработчик прерывания по флагу RXNE UART1
 *  
 */
void USART1_IRQHandler(){
    if(USART_GetITStatus(USART1, USART_IT_RXNE)){
        uint8_t data = USART_ReceiveData(USART1);
        ringPut(&gRingBuf, data);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
