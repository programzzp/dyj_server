#ifndef WEBSOCKETPROCESSOR_H
#define WEBSOCKETPROCESSOR_H
#ifdef __cplusplus
extern "C" {
#endif
#include "utile.h"


#define  CONTINUOUS         0x0 //表示一个持续帧
#define  TEXT               0x1 //表示一个文本帧
#define  BIT                0x2 //表示一个二进制帧
#define  CLOSE              0x8 //表示一个连接关闭包
#define  PING               0x9 //表示一个 ping 包
#define  PONG               0xa // 表示一个 pong 包


typedef struct DataInformationFraming{
    uint8_t FIN : 1;
    uint8_t RSV1 : 1;
    uint8_t RSV2 : 1;
    uint8_t RSV3 : 1;
    uint8_t opcode : 4;
    uint8_t MASK : 1;
    uint64_t Payload_len;
}DataInformationFraming;


typedef struct DataFraming{
    uint8_t opcode : 4;
    uint8_t RSV3 : 1;
    uint8_t RSV2 : 1;
    uint8_t RSV1 : 1;
    uint8_t FIN : 1;
    uint8_t Payload_len : 7;
    uint8_t MASK : 1;
}DataFraming;

typedef union WebSocketStructureDef{
    uint8_t data[2];
    DataFraming dataFraming;
}WebSocketStructureDef;


void getWebSocketProcessor(char *chat,int fd);


#ifdef __cplusplus
}
#endif

#endif