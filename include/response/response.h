//
// Created by redmi on 2024/4/8.
//

#ifndef CDEMOS_RESPONSE_H
#define CDEMOS_RESPONSE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "response_mapping.h"
#include "request/requestInfoOutout.h"
#include <stdio.h>


enum URL_TYPE{
    URL_ERROR,
    URL_URL,
    UEL_INDEX,
    URL_PARAMETER
};

typedef struct { 
    char key[100];          /*响应头key值**/
    char value[100];        /*响应头value值**/
}HttpRespBody;

struct HttpRespBodyNode{
    HttpRespBody body;                /*响应头值**/
    struct HttpRespBodyNode* next;    /*链表指针**/
};

typedef struct HttpRespBodyNode RespHeadNode;

typedef char *(*LoopFunction)(Http_head_Def *http_head_Def,char *data,RespHeadNode **resp,int *len,HttpNodeListDef *request);

struct FunctionRegisterStructure{
    LoopFunction fun;
    char method[100];
    char path[1024];
};

typedef struct FunctionRegisterStructure RegisterStructure;

#define MAX 30

extern RegisterStructure loopFunctions[MAX];
extern int Count;

/**
 * @brief 注册函数
*/
#define RequestMapping(name,methods,paths) \
char *name##_init(Http_head_Def *http_head_Def,char *data,RespHeadNode **resp,int *len,HttpNodeListDef *request);                       \
__attribute__((constructor)) void init_##name() { \
loopFunctions[Count].fun = &name##_init;                               \
strcpy(loopFunctions[Count].method,#methods);                                   \
strcpy(loopFunctions[Count].path,#paths);                       \
++Count;                                   \
}                      \
char *name##_init(Http_head_Def *http_head_Def,char *data,RespHeadNode **resp,int *len,HttpNodeListDef *request) \



char *getResponseHeadString(Http_head_Def *httpHeadDef,char *resp,int len,char *file_type,int StateIndex,long file_len);

void setRespHeadNode(RespHeadNode **resp,char *key,char* value,int* len);

void getRespHeadNode(RespHeadNode *resp,char *respBodyData,int len);

void FreeResp(char *resp_buf);

int judge_register_exist(char *path,char *method);


#ifdef __cplusplus
}
#endif
#endif //CDEMOS_RESPONSE_H
