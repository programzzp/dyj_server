//
// Created by redmi on 2024/4/5.
//

#ifndef CDEMOS_CONFIG_H
#define CDEMOS_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif


#define SERVER_VERSION  "1.1.2"
#define SERVER_NAME     "DYJ_SERVER"
#define PORT        80
#define LISTEN_NUM  128

#define HTML_RESOURCE     "html"
#define SUCCESS_LOG       "log/success.log"
#define ROOT_PATH         "/index.html"

/**
 * Host: localhost:8080
 * header --> Host
 * value  --> localhost:8080
 * HTTP_HEADER  header大小
 * HTTP_VALUE   value大小
 */
#define HTTP_HEADER    100
#define HTTP_VALUE     512


//每个请求头条目大小
#define OFFSET   0xff      /*偏移量*/
//请求真实数据大小
#define  REQUEST_DATA    1024
//请求总数居大小
#define  REQUEST_TOTAL_DATA   4096

//响应行大小
#define RESPONSE_HEAD_ROW  0xff
//响应条目
#define RESPONSE_HEAD_LIST  5


#ifdef __cplusplus
}
#endif
#endif //CDEMOS_CONFIG_H
