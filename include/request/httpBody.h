//
// Created by redmi on 2024/4/4.
//

#ifndef CDEMOS_HTTPBODY_H
#define CDEMOS_HTTPBODY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "config.h"
#include "myMalloc.h"


/**
 * @brief 请求体的key value解析
*/
typedef struct {
    char Header[HTTP_HEADER];   /** @brief key值 */
    char Value[HTTP_VALUE];     /** @brief value值 */
}HttpBodyStruct_Def;

/**
 * 请求体的key value链表结构体
*/
struct HttpNodeList{
    HttpBodyStruct_Def httpBodyStructDef; /** @brief 上一个结构体 */
    struct HttpNodeList *next;            /** @brief 指向下一个结构体的指针 */
};

typedef struct HttpNodeList HttpNodeListDef;

HttpBodyStruct_Def* getHttpBodyStruct_Def(const char* body_buf);

void add_HttpBodyStruct_Def_in_List(HttpBodyStruct_Def *httpBodyStructDef,HttpNodeListDef **httpNodeList);

#ifdef __cplusplus
}
#endif
#endif //CDEMOS_HTTPBODY_H
