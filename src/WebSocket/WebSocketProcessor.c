#include "WebSocket/WebSocketProcessor.h"
#include "utile.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "internet.h"


#define MASK_LEN (4)

uint8_t mask_position = 0;


static void get_frame_data(DataInformationFraming *frame_data,uint8_t *data);
static uint64_t Payload_len(int  Payload_len,uint8_t *data);
static void getSendData(DataInformationFraming *frame_data,char *new_Data,uint8_t *data,int len,int data_len);
static void getMask_data(uint8_t *data,uint8_t *target,int start);
static void get_source_data(uint8_t *data,uint8_t *target,int start,int end);


void getWebSocketProcessor(char *chat,int fd){
    uint8_t *chat_buf = (uint8_t *)chat;
    mask_position = 0;

    DataInformationFraming frameData = {0};
    get_frame_data(&frameData,chat_buf);

    if(frameData.opcode == CLOSE){
        epoll_delete(epfd,fd);
    }

    char new_buf[frameData.Payload_len];
    memset(new_buf,0,frameData.Payload_len);

    getSendData(&frameData,new_buf,chat_buf,strlen(chat),(int)frameData.Payload_len);

    DEBUG_LOG("%s\r\n",new_buf);
    
}



static void get_frame_data(DataInformationFraming *frame_data,uint8_t *data){
    WebSocketStructureDef frame = {0};
    frame.data[0] = data[0];
    frame.data[1] = data[1];
    frame_data->FIN = frame.dataFraming.FIN;
    frame_data->RSV1 = frame.dataFraming.RSV1;
    frame_data->RSV2 = frame.dataFraming.RSV2;
    frame_data->RSV3 = frame.dataFraming.RSV3;
    frame_data->opcode = frame.dataFraming.opcode;
    frame_data->MASK = frame.dataFraming.MASK;
    frame_data->Payload_len = Payload_len(frame.dataFraming.Payload_len,data);

}

static uint64_t Payload_len(int  Payload_len,uint8_t *data){
    if(Payload_len <= 125){
        mask_position += 2;
        return Payload_len;
    }else if(Payload_len == 126){   //16为数字
        mask_position += 4;
        uint16_t len = 0;
        len |= (data[2] << 8);
        len |= data[3];
        return len;
    }else if(Payload_len == 127){  //64位数字
        mask_position += 11;
        uint64_t len;
        for (int i = 7; i >= 0; --i) {
            len |= (data[(7+2)-i] << (i*8));
        }
        return len;
    }
    return 0;
}


//获取掩码
static void getMask_data(uint8_t *data,uint8_t *target,int start){
    int start_conut = 0;

    for (int i = start; i < start+MASK_LEN; i++)
    {
        target[start_conut] = data[i];
        ++start_conut;
    }
}

//获取原始数据
static void get_source_data(uint8_t *data,uint8_t *target,int start,int end){
    int start_conut = 0;

    for (int i = start; i < start+end; i++)
    {
        target[start_conut] = data[i];
        ++start_conut;
    }

}


static void getSendData(DataInformationFraming *frame_data,char *new_Data,uint8_t *data,int len,int data_len){
    uint8_t mask[4];
    uint8_t datamask[data_len];
    char buf[data_len];
    memset(mask,0,MASK_LEN);
    memset(datamask,0,data_len);
    memset(buf,0,data_len);


    getMask_data(data,mask,mask_position);

    get_source_data(data,datamask,mask_position+MASK_LEN,data_len);

    for (int i = 0; i < data_len; i++)
    {
        buf[i] = (char)(mask[i%4]^datamask[i]);
    }

    buf[data_len] = '\0';

    strcpy(new_Data,buf);
}
