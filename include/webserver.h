#ifndef WEBSERVER_H
#define WEBSERVER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "internet.h"
#include <pthread.h>
#include "utile.h"
#include "log/system_log.h"

#define CONNECT_LENGTH 4096

typedef struct SocketConFd{
    uint8_t connect_flag; //
    uint8_t read_flag;//

}SocketConFd;

enum RESP_TYPE{
    RESP_ERROR = 0,     /**文件不存在*/
    RESP_FILE,          /**返回为file文件*/
    RESP_JSON,          /**返回为注册的JSON文件*/
    RESP_ROOT           /**跟目录*/
};

void webServer_Init(void);
void webServer_Loop(void);
void httpQueueService(int fd);


#ifdef __cplusplus
}
#endif

#endif