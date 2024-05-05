#include "webserver.h"
#include "utile.h"
#include <pthread.h>
#include "threadPool.h"
#include "timer_task.h"
#include "request/requestInfoOutout.h"
#include "response/response.h"
#include "WebSocket/WebSocketConnect.h"
#include "WebSocket/WebSocketProcessor.h"

static void responseContext(REQUEST_DATA_OUTPUT *request,int fd);
static int return_url_file_type(Http_head_Def *httpHeadDef);
static void response_json(REQUEST_DATA_OUTPUT *request,int fd);
static int stringCompare(char* source,char *target,char *data);
static void response_file(REQUEST_DATA_OUTPUT *request,int fd);
static void response_root(REQUEST_DATA_OUTPUT *request,int fd);
static void response_error(REQUEST_DATA_OUTPUT *request,int fd);
static bool judgeWebSocket(HttpNodeListDef* httpNodeList,char *key,char *value);
static bool getRequestListKey(HttpNodeListDef* httpNodeList,char *key,char *value);
static void httpContextController(REQUEST_DATA_OUTPUT *request,int fd);
static void webSocketContextController(REQUEST_DATA_OUTPUT *request,int fd);

//socket文件描述符
int servfd;
//epoll文件描述符
int epfd;
//epoll结构体
EPOLL_DEF event;
//线程池



//404返回的页面
char Not_Found[] = "<!doctype html> \
    <html lang=\"en\"><head> \
    <meta charset=\"UTF-8\">\
    <meta name=\"viewport\"  \
    content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">\
    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\
    <title>404 Not Found</title>\
    </head>\
    <body>\
    <h1 align=\"center\" style=\"font-size: 50px\">404 Not Found</h1>\
    <h2 align=\"center\">你所访问的页面可能不存在!</h2>\
    <h2 align=\"center\">The page you are visiting may not exist!</h2>\
    </body>\
    </html>";

void webServer_Init(void){
    //timer_init();
    servfd = socket_init(IPV4,TCP);
    epfd = epoll_init(&event,servfd);

    //线程池初始化
    ThreadQueueInit(4);
    
}



void http_context(char *req_buf,int clientfd){
    REQUEST_DATA_OUTPUT *data = RequestInfoOutputInit();
    setRequestInfoOutput(data,req_buf);

    responseContext(data,clientfd);
    
   // DEBUG_LOG("%s %s %s\r\n",data->http_head_Def->method,data->http_head_Def->Version,data->http_head_Def->path);

    FreeRequestInfoOutput(data); 
}


void httpQueueService(int fd){
    char buf[4096];
    int n;
    int clientfd = fd;
    memset(buf,0,sizeof(buf));
    n = read(clientfd,buf,sizeof(buf));
    DEBUG_LOG("n = %d\r\n",n);

    if(n == 0){
        epoll_delete(epfd,clientfd);
    }

    if(buf[0] < 0){
        getWebSocketProcessor(buf,fd);
    }else{
        http_context(buf,fd);
    }
    // update_timer_task(fd);
}


/**
 * 
*/
void webServer_Loop(void){
    int epoll_loop;
    int clientfd;
 
    EPOLL_DEF epList[LISTEN_NUM];
    while(true){
        epoll_loop=epoll_wait(epfd,epList,LISTEN_NUM,-1);

        for (int i = 0; i < epoll_loop; i++)
        {
            clientfd = epList[i].data.fd;

            if(clientfd == servfd){
                epoll_accpet(servfd,&event,epfd);
            }else{
                pushQueue(clientfd);
            }
        }
        
    }
}


static bool judgeWebSocket(HttpNodeListDef* httpNodeList,char *key,char *value){
    HttpNodeListDef *node = httpNodeList;

    while(node != NULL){
        if(strcmp(key,node->httpBodyStructDef.Header) == 0 && strcmp(value,node->httpBodyStructDef.Value) == 0){
            return true;
        }
        node = node->next;
    }
    return false;
}



static bool getRequestListKey(HttpNodeListDef* httpNodeList,char *key,char *value){
    HttpNodeListDef *node = httpNodeList;

    while(node != NULL){
        if(strcmp(key,node->httpBodyStructDef.Header) == 0){
            strcpy(value,node->httpBodyStructDef.Value);
            return true;
        }
        node = node->next;
    }
    return false;
}

static void httpContextController(REQUEST_DATA_OUTPUT *request,int fd){
    int file_type = return_url_file_type(request->http_head_Def);

    switch (file_type)
    {
    case RESP_FILE:
        /* code */
        response_file(request,fd);
        break;
    case RESP_ROOT:
        response_root(request,fd);
        break;
    case RESP_JSON:
        response_json(request,fd);
        break;
    case RESP_ERROR:
        response_error(request,fd);
        break;
    default:
        break;
    }
}


static void webSocketContextController(REQUEST_DATA_OUTPUT *request,int fd){
    char webBuf[255];
    char *WebSocketHead;
    if(getRequestListKey(request->httpNodeList,"Sec-WebSocket-Key",webBuf)){
        DEBUG_LOG("%s\r\n",webBuf);
        WebSocketHead = WebSocketResponse_head(webBuf);
        write(fd,WebSocketHead,strlen(WebSocketHead));

        free(WebSocketHead);
    }
}

static void responseContext(REQUEST_DATA_OUTPUT *request,int fd){
    bool judgeWebsocket = judgeWebSocket(request->httpNodeList,"Upgrade","websocket");
    if(judgeWebsocket){
        webSocketContextController(request,fd);
    }else{
        httpContextController(request,fd);
        epoll_delete(epfd,fd);
    }
    // DEBUG_LOG("I = %d\r\n",i);
    // by_pass_file_suffix(request->http_head_Def->path,suffix);
    // DEBUG_LOG("suffix = %s\r\n",suffix);
}


static int return_url_file_type(Http_head_Def *httpHeadDef){
    if(judge_register_exist(httpHeadDef->path,httpHeadDef->method) > 0){
        return RESP_JSON;
    }

    if(indexHtmlfile(httpHeadDef->path)){
        return RESP_ROOT;
    }

    if(judgeFileWhetherExist(httpHeadDef->path) > 0){
        return RESP_FILE;
    }

    return RESP_ERROR;
}


static void response_json(REQUEST_DATA_OUTPUT *request,int fd){
    LoopFunction fun_data;
    //json构造返回头
    RespHeadNode *resp = NULL;
    
    int len = 0;
    //获取下标
    int resp_json_number = judge_register_exist(request->http_head_Def->path,request->http_head_Def->method);

     //获取注册函数
    RegisterStructure  registerFunction = loopFunctions[resp_json_number-1];
    fun_data = registerFunction.fun;

    //如果为GET请求
    if(strcmp(request->http_head_Def->method,"GET") == 0){
        char url_data[255];
        memset(url_data,0,sizeof(url_data));
        stringCompare(registerFunction.path,request->http_head_Def->path,url_data);
        char *data_json = fun_data(request->http_head_Def,url_data,&resp,&len,request->httpNodeList);

         //分配内存
        char *resp_head_list =(char *)malloc(sizeof(100*len));
        memset(resp_head_list,0,sizeof(100*len));
        //获取响应头key：value字符串
        getRespHeadNode(resp,resp_head_list,len);
        char *headString = getResponseHeadString(request->http_head_Def,resp_head_list,len,"json",OK,strlen(data_json));
        write(fd,headString,strlen(headString));
        write(fd,data_json,strlen(data_json));

        free(headString);
        free(data_json);
        free(resp_head_list);
    }else if(strcmp(request->http_head_Def->method,"POST") == 0){

    }
    
}

static int stringCompare(char* source,char *target,char *data){
    char *source_point = source;
    char *target_point = target;
    char *data_point = data;

    while(*source_point != '\0'){
        if(*target_point != *source_point){
            if(*source_point == '{')
                goto GET_USER_DATA;
            else if(*source_point == '[')
                goto GET_DATA;
            else
                return URL_ERROR;
        }
        ++target_point;++source_point;
    }

    return URL_URL;
    GET_USER_DATA:
    if(*target_point == 0){
        return URL_ERROR;
    }
    ++target_point;
    while(*target_point != '\0'){
        *data_point = *target_point;
        ++target_point;++data_point;
    }
    *data_point = '\0';
    return UEL_INDEX;
    GET_DATA:
    if(*target_point != '?'){
        return URL_ERROR;
    }
    ++target_point;
    while(*target_point != '\0'){
        *data_point = *target_point;
        ++target_point;++data_point;
    }
    *data_point = '\0';
    return URL_PARAMETER;
}


static void response_file(REQUEST_DATA_OUTPUT *request,int fd){
    char suffix[255];
    char buf_data[1024];
    int n = 0;
    memset(suffix,0,sizeof(suffix));

    long file_len = judgeFileWhetherExist(request->http_head_Def->path);
    by_pass_file_suffix(request->http_head_Def->path,suffix);
    //得到请求头
    char *headString = getResponseHeadString(request->http_head_Def,NULL,0,suffix,OK,file_len);
    write(fd,headString,strlen(headString));

    int file_fd = getFileFd(request->http_head_Def->path);

    while((n = read(file_fd,buf_data,sizeof(buf_data))) != 0){
        write(fd,buf_data,n);
        memset(buf_data,0,sizeof(buf_data));
    }


    close(file_fd);
    free(headString);
}



static void response_root(REQUEST_DATA_OUTPUT *request,int fd){
    char buf_data[1024];
    int n = 0;
    long file_len = judgeFileWhetherExist(ROOT_PATH);
    char *headString = getResponseHeadString(request->http_head_Def,NULL,0,"html",OK,file_len);

    write(fd,headString,strlen(headString));

    int file_fd = getFileFd(ROOT_PATH);
    while((n = read(file_fd,buf_data,sizeof(buf_data))) != 0){
        write(fd,buf_data,n);
        memset(buf_data,0,sizeof(buf_data));
    }


    close(file_fd);
    free(headString);
}
static void response_error(REQUEST_DATA_OUTPUT *request,int fd){

    char *headString = getResponseHeadString(request->http_head_Def,NULL,0,"html",NotFound,strlen(Not_Found));
    write(fd,headString,strlen(headString));
    write(fd,Not_Found,strlen(Not_Found));


    free(headString);
}


