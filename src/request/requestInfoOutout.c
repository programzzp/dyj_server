#include "request/requestInfoOutout.h"

/**
 * @brief   初始化REQUEST_DATA_OUTPUT结构体
 * @return  初始化后REQUEST_DATA_OUTPUT指针
*/
REQUEST_DATA_OUTPUT* RequestInfoOutputInit(){
    REQUEST_DATA_OUTPUT *request_data = (REQUEST_DATA_OUTPUT *) malloc(sizeof(REQUEST_DATA_OUTPUT));
    request_data->data = NULL;
    request_data->httpNodeList = NULL;
    request_data->http_head_Def = NULL;
    request_data->buf = NULL;
    return request_data;
}



/**
 * @brief                   将http请求的源数据信息分解
 * @param data              初始化后REQUEST_DATA_OUTPUT指针
 * @param request_source    http请求的请求体字符串
*/
void setRequestInfoOutput(REQUEST_DATA_OUTPUT *data,char* request_source){
    int number = 0;
    //元数据结构体
    HttpHeadData* buf;
    /**
     * 对request原数据进行分析
     * buf->head 头部信息
     * buf->data 数据信息
     */
    buf = http_buf_segmentation(request_source,&number);

    /**
     * 需要释放的元素
     */
    data->buf = buf;

    /**
     * 源数据 buf->head 头数据中第一行数据 “GET /spring/boot HTTP/1.1”
     */
    char *request_head = buf->head;
    Http_head_Def *http_head_Def = Get_http_request_head(request_head);


    /**
     * 解析请求头第一行外，其他数据，将字符串解析为key value保存到httpBodyStructDef链表中
     */
    HttpBodyStruct_Def *httpBodyStructDef;
    HttpNodeListDef* httpNode = NULL;
    for (int i = 1; i < number; ++i) {
        httpBodyStructDef = getHttpBodyStruct_Def(request_head+(OFFSET*i));
        add_HttpBodyStruct_Def_in_List(httpBodyStructDef,&httpNode);
    }

    data->data = buf->data;
    data->http_head_Def = http_head_Def;
    data->httpNodeList = httpNode;


}


/**
 * 释放内存
*/
void FreeRequestInfoOutput(REQUEST_DATA_OUTPUT *data){


    MY_FREE(data->http_head_Def);
    HttpNodeListDef* node = data->httpNodeList;
    while(node!=NULL){
        free(node);
        node = node->next;
    }

    node = NULL;


    MY_FREE(data->buf->head);
    MY_FREE(data->buf->data);
    MY_FREE(data->buf);
    MY_FREE(data);
}