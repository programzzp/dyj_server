/**
 * 
*/
#include "response/response.h"
#include  <time.h>
#include "log/system_log.h"
#include "utile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int stringCompareJudge(char* source,char *target);

RegisterStructure loopFunctions[MAX];
int Count = 0;


typedef struct
{
	char week[5];
	char month[5];
	char day[4];
	char time[10];
	char year[5];
} HttpTimes;


/**
 * @brief 此代码有点垃圾勿看,获取时间 Date: Tue, 16 Apr 19:30:35 2024 GMT
*/
static void getrespTime(char* times){
    time_t timep;
	time(&timep);
	HttpTimes http_times;
	char *http_time_point = http_times.week;
	char time_buf[30];
	strcpy(time_buf,ctime(&timep));
	char *time_buf_point = time_buf;

	//星期
	while(*time_buf_point != ' '){
		*http_time_point = *time_buf_point;
		++http_time_point;
		++time_buf_point;
	}
	*http_time_point = '\0';
	++time_buf_point;
	//月份
	http_time_point = http_times.month;
	while(*time_buf_point != ' '){
		*http_time_point = *time_buf_point;
		++http_time_point;
		++time_buf_point;
	}
	*http_time_point = '\0';
	++time_buf_point;
	//天
	http_time_point = http_times.day;
	while(*time_buf_point != ' '){
		*http_time_point = *time_buf_point;
		++http_time_point;
		++time_buf_point;
	}
	*http_time_point = '\0';
	++time_buf_point;
	//时间
	http_time_point = http_times.time;
	while(*time_buf_point != ' '){
		*http_time_point = *time_buf_point;
		++http_time_point;
		++time_buf_point;
	}
	*http_time_point = '\0';
	++time_buf_point;
	//年
	http_time_point = http_times.year;
	while(*time_buf_point != '\n'){
		*http_time_point = *time_buf_point;
		++http_time_point;
		++time_buf_point;
	}
	*http_time_point = '\0';
	++time_buf_point;

	sprintf(times,"Date: %s, %s %s %s %s GMT\n",http_times.week,http_times.day,http_times.month,http_times.time,http_times.year);

}




/**
 * @brief  {{"html","text/html"}, \
                {"css","text/css"}, \
                {"js","application/x-javascript"}, \
                {"plain","text/plain"}, \
                {"xml","text/xml"}, \
                {"ico","image/ico"}, \
                {"gif","image/gif"}, \
                {"jpeg","image/jpeg"}, \
                {"png","image/png"}, \
                {"jpg","image/jpg"}, \
                {"json","application/json"}};
 * @param file_type   文件类型字符串
 * @return            文件类型的数字下标
 */
static int setContextType(char* file_type){
    if(strcmp(file_type,"html") == 0){
        return HTML;
    }else if(strcmp(file_type,"css") == 0){
        return CSS;
    }else if(strcmp(file_type,"js") == 0){
        return JS;
    }else if(strcmp(file_type,"plain") == 0){
        return PLAIN;
    }else if(strcmp(file_type,"xml") == 0){
        return XML;
    }else if(strcmp(file_type,"ico") == 0){
        return ICO;
    }else if(strcmp(file_type,"gif") == 0){
        return GIF;
    }else if(strcmp(file_type,"jpeg") == 0){
        return JPEG;
    }else if(strcmp(file_type,"png") == 0){
        return PNG;
    }else if(strcmp(file_type,"jpg") == 0){
        return JPG;
    }else if(strcmp(file_type,"json") == 0){
        return JSON;
    }else if(strcmp(file_type,"mp4") == 0){
        return MP4;
    }else{
        return -1;
    }

}



static void resp_memset(char *head,char *server_name,char* AcceptRanges,char *ContextType){
    memset(head,0,100);
    memset(server_name,0,100);
    memset(AcceptRanges,0,50);
    memset(ContextType,0,100);
}

/**
 * @brief   HTTP/1.1 200 OK
 *          Server: Nginx_Server
 *          Accept-Ranges: bytes
 *          Context-Type: application/json
 * @param httpHeadDef  http请求的参数
 * @param resp         响应体字符串
 * @param len          链表长度
 * @param file_type    响应文件类型
 * @param StateIndex   状态  
 * @return             响应头字符串
 * 
 */
char *getResponseHeadString(Http_head_Def *httpHeadDef,char *resp,int len,char *file_type,int StateIndex,long file_len){
    //时间字符串
    char date_buf[100];
    memset(date_buf,0,sizeof(date_buf));

    //设置返回字符出长度
    int resp_len = RESPONSE_HEAD_ROW*(len+RESPONSE_HEAD_LIST);

    //动态分配空间用于返回字符串
    char *responseHeadString = malloc(sizeof(char) * resp_len);
    memset(responseHeadString,'\0',resp_len);

    /**
     * resp_head        HTTP/1.1 200 OK
     * Server_NAME      Server: Nginx_Server
     * AcceptRanges     Accept-Ranges: bytes
     * ContextType      application/json
    */
    char resp_head[100];
    char Server_NAME[100];
    char AcceptRanges[50];
    char ContextType[100];
    resp_memset(resp_head,Server_NAME,AcceptRanges,ContextType);
    char Length_buf[100];
    memset(Length_buf,0,sizeof(Length_buf));

    sprintf(resp_head,"%s %s %s\r\n",httpHeadDef->Version,httpStateCode[StateIndex].Code,httpStateCode[StateIndex].StateExplain);

    sprintf(Server_NAME,"%s %s:%s\r\n","Server:",SERVER_NAME,SERVER_VERSION);
    sprintf(AcceptRanges,"Accept-Ranges: bytes\r\n");
    sprintf(ContextType,"Content-Type: %s\r\n",httpResponseContextType[setContextType(file_type)].res_type);
    sprintf(Length_buf,"Content-Length: %ld\r\n",file_len);
    getrespTime(date_buf);

    strcat(responseHeadString,resp_head);
    strcat(responseHeadString,Server_NAME);
    strcat(responseHeadString,AcceptRanges);
    strcat(responseHeadString,ContextType);
    strcat(responseHeadString,date_buf);
    strcat(responseHeadString,Length_buf);
    

    //添加响应头的数据转化为字符串
    if(len > 0){
        strcat(responseHeadString,resp);
    }
    strcat(responseHeadString,"\r\n");

    return responseHeadString;

}


/**
 * @brief 添加响应头的key ：value
 * @param resp     传入结构体
 * @param key      响应头key
 * @param value    响应头value
 * @param len      传出参数：总长度
*/
void setRespHeadNode(RespHeadNode **resp,char *key,char* value,int* len){
    
    RespHeadNode *head = *resp;
    RespHeadNode *node = Malloc(RespHeadNode,1);
    strcpy(node->body.key,key);
    strcpy(node->body.value,value);
    node->next = NULL;
    ++(*len);

    if(head == NULL){
        *resp = node;
    }else{
        RespHeadNode *current = *resp;
        while(current->next !=NULL){
            current = current->next;
        }
        current->next =node;
    }
}

/**
 * @brief 返回响应头字符串
 * @param resp             传入结构体
 * @param respBodyData     传出参数
 * @param len              链表长度
*/
void getRespHeadNode(RespHeadNode *resp,char *respBodyData,int len){
    RespHeadNode *head = resp;
    RespHeadNode *del = resp;
    char buf[RESPONSE_HEAD_ROW];
    memset(respBodyData,0,strlen(respBodyData));

    if(resp == NULL){
        respBodyData = NULL;
    }else{
        /*将数据转化为key: value格式*/
        while(head != NULL){
            memset(buf,0,RESPONSE_HEAD_ROW);
            sprintf(buf,"%s: %s\r\n",head->body.key,head->body.value);
            strcat(respBodyData,buf);
            head = head->next;
        }
    }

    //释放链表内存
    while(del != NULL){
        free(del);
        del = del->next;
    }

    //赋值为空
    resp = NULL;
}

/**
 * 
*/
void FreeResp(char *resp_buf){
    if(resp_buf != NULL){
        free(resp_buf);
        resp_buf = NULL;
    }
}




/**
 * @brief 判断注册表中是否存在
 * @param path             路径
 * @param method           请求方式
 * @return                 返回是否存在 存在返回数组下标+1
*/
int judge_register_exist(char *path,char *method){
    char *data_bufs = (char *)malloc(sizeof(char)*255);
    memset(data_bufs,0,255);
    RegisterStructure RegisterList;
    for (int i = 0; i < Count; i++)
    {
        RegisterList = loopFunctions[i];
        if(stringCompareJudge(RegisterList.path,path) > 0 && strcmp(method,RegisterList.method) == 0){
            return i+1;
        }
    }

    return false;
}


/**
 * stringCompare字符串比较函数
 * 当注册函数为  /url/{url} {url}中的参数为请求数据
 * 当注册函数为  /url/[url] [url]数据类型为?username=redmi&password=redmi
 * 当注册函数为  /url/url   url为请求路径
*/
static int stringCompareJudge(char* source,char *target){
    char *source_point = source;
    char *target_point = target;

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
    return UEL_INDEX;
    GET_DATA:
    if(*target_point != '?'){
        return URL_ERROR;
    }
    ++target_point;
    return URL_PARAMETER;
}