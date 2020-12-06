#include "init.h"
#include "protocol.h"


/**
 * @brief точка входа
 * 
 */
int main(){
    initMcu();
    initProtocolBuf();
    while(1){
        parseProtocolMsg();
    }
}
