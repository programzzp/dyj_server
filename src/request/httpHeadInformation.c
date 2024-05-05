#include "request/httpHeadInformation.h"
#include "utile.h"

/**
 * @brief http第一行头部的解析解析如下格式
 *        method  = GET;         
 *        path    = /user;         
 *        Version = HTTP/1.1;        
 * @param http_head_buf   解析前的字符串数据
 * @return                返回解析后的数据
 */
Http_head_Def* Get_http_request_head(const char* http_head_buf){
    char head_buf_def[1024]; //

    Http_head_Def *httpHeadDef = (Http_head_Def *) malloc(sizeof(Http_head_Def));

    char *buf_point;
    char *method = httpHeadDef->method;
    char *path = httpHeadDef->path;
    char *Version = httpHeadDef->Version;


    strcpy(head_buf_def,http_head_buf);

    buf_point = head_buf_def;

    //获取method
    while(*buf_point != ' '){
        *method = *buf_point;
        ++method;
        ++buf_point;
    }
    ++buf_point;
    *method = '\0';

    //获取path
    //获取method
    while(*buf_point != ' '){
        *path = *buf_point;
        ++path;
        ++buf_point;
    }
    ++buf_point;
    *path = '\0';

    while(*buf_point != '\0'){
        *Version = *buf_point;
        ++buf_point;
        ++Version;
    }
    *Version = '\0';

    return httpHeadDef;
}

/**
 * @brief 将字符串解析为一行为主的字符串数组与数据
 *        每一个数据条目分配OFFSET个字节，也就是0xff个字节
 * @param buf_string     字符串
 * @param number         字符串总长度
 * @return               HttpHeadData数据
 */
HttpHeadData* http_buf_segmentation(const char* buf_string,int *number){

    //定义请求头数据
    HttpHeadData *httpHeadData = (HttpHeadData *) malloc(sizeof(HttpHeadData));

    //数据总条目
    *number = 0;
    //申请请求头数据大小，没有条数据大小通过OFFSET
    char *base_address_http = (char *) malloc(sizeof(char)*OFFSET*20);  //基地址
    //申请请求数据大小，通过REQUEST_DATA设置
    char *data = (char *) malloc(sizeof (char)*REQUEST_DATA);
    //请求中的总数据
    char buf_http[REQUEST_TOTAL_DATA];
    strcpy(buf_http,buf_string);
    char *http_string_point = buf_http;

    char *http_buf_array;
    int offset = 0;
    int array_number_point = 0;
    while(true){
        http_buf_array = base_address_http+(OFFSET*offset);
        while(true){
            *(http_buf_array+array_number_point) = *http_string_point;
            if(*http_string_point == '\r' && *(http_string_point + 1) == '\n'){
                break;
            }
            ++http_string_point;
            ++array_number_point;
        }

        http_string_point += 2;
        if(*http_string_point == '\r' && *(http_string_point+1) == '\n'){
            *(http_buf_array+array_number_point) = '\0';
            array_number_point = 0;
            ++offset;
            break;
        }else{
            *(http_buf_array+array_number_point) = '\0';
            array_number_point = 0;
            ++offset;
        }
    }

    *number = offset;

    http_string_point += 2;
    array_number_point = 0;
    while(*http_string_point != '\0'){
        *(data + array_number_point) = *http_string_point;
        ++http_string_point;
        ++array_number_point;
    }
    *(data+array_number_point) = '\0';

    /**
     * 解析后需要返回的数据
     */
    httpHeadData->head = base_address_http;
    httpHeadData->data = data;

    return httpHeadData;
}


/**
 * @brief 释放内存(已舍弃)
*/
void http_request_free(HttpHeadData* httpHeadDataBuf,Http_head_Def *http_head_Def,HttpNodeListDef* httpNode){
    MY_FREE(httpHeadDataBuf->head);
    if(httpHeadDataBuf->head != NULL){
        My_Error("error: httpHeadDataBuf->head Free error\r\n");
    }
    MY_FREE(httpHeadDataBuf->data);
    if(httpHeadDataBuf->data != NULL){
        My_Error("error: httpHeadDataBuf->data Free error\r\n");
    }
    MY_FREE(httpHeadDataBuf);
    if(httpHeadDataBuf != NULL){
        My_Error("error: httpHeadDataBuf Free error\r\n");
    }

    MY_FREE(http_head_Def);
    if(http_head_Def != NULL){
        My_Error("error: http_head_Def Free error\r\n");
    }

    HttpNodeListDef* node = httpNode;
    while(node != NULL){
        node = node->next;
        free(node);
    }
    node = NULL;



}



