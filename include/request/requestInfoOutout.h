#ifndef CDEMOS_REQUESTINFOOUTOUT_H
#define CDEMOS_REQUESTINFOOUTOUT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "httpHeadInformation.h"
#include "httpBody.h"
#include "httpMapping.h"
#include "myMalloc.h"


/**
* RequestInfoOutput Request的消息输出
* Http_head_Def *http_head_Def;   http协议请求头第一行数据
* HttpNodeListDef* httpNode;      http协议key: value数据
* char *data;                     http \r\n\r\n 后数据
*/
struct RequestInfoOutput{
    Http_head_Def *http_head_Def;
    HttpNodeListDef* httpNodeList;
    char *data;
    HttpHeadData* buf;
};

typedef struct RequestInfoOutput REQUEST_DATA_OUTPUT;

REQUEST_DATA_OUTPUT* RequestInfoOutputInit();

void setRequestInfoOutput(REQUEST_DATA_OUTPUT *data,char* request_source);

void FreeRequestInfoOutput(REQUEST_DATA_OUTPUT *data);

#ifdef __cplusplus
}
#endif
#endif //CDEMOS_REQUESTINFOOUTOUT_H
