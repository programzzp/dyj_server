//
// Created by redmi on 2024/4/4.
//

#ifndef CDEMOS_HTTPHEADINFORMATION_H
#define CDEMOS_HTTPHEADINFORMATION_H
#ifdef __cplusplus
extern "C" {
#endif

#include "malloc.h"
#include "stdio.h"
#include "string.h"
#include "config.h"
#include "httpBody.h"

enum RequestMethod{
    GET, HEAD, POST, PUT, PATCH, DELETE, OPTIONS, TRACE
};

enum RequestVersion{
    HTTP_1_0,HTTP_1_1,HTTP_2_0
};


/**
 * @brief http请求头解析后的第一行数据
*/
typedef struct {
    char method[20];         /** @brief 请求方式*/
    char path[1024];         /** @brief 请求url*/
    char Version[40];        /** @brief http版本号*/
}Http_head_Def;

/**
 * @brief HttpHeadData请求头解析结构体
 */
typedef struct {
    char *head;   /** @brief http请求头分片数据*/
    char *data;   /** @brief post数据*/
}HttpHeadData;

Http_head_Def* Get_http_request_head(const char* http_head_buf);
HttpHeadData* http_buf_segmentation(const char* buf_string,int *number);

void http_request_free(HttpHeadData* httpHeadDataBuf,Http_head_Def *http_head_Def,HttpNodeListDef* httpNode);


#ifdef __cplusplus
}
#endif

#endif //CDEMOS_HTTPHEADINFORMATION_H
