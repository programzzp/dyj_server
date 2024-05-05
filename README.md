# DYJ_SERVER

## 介绍

基于 Linux POSIX接口实现的http与websocket服务器

可是实现动态的服务注册，比springboot更加高效的web服务器

### 用到的库函

- pthread
- openssl

## 软件框架

### 消息处理

![](https://raw.githubusercontent.com/programzzp/dyj_server/main/img/%E6%B6%88%E6%81%AF%E5%A4%84%E7%90%86%E6%B5%81%E7%A8%8B.png)

### 软件初始化

![](https://raw.githubusercontent.com/programzzp/dyj_server/main/img/%E5%88%9D%E5%A7%8B%E5%8C%96.png)

## 软件的下载与使用

### 启动软件

```bash
git clone https://github.com/programzzp/dyj_server.git
#进入root用户
cd dyj_server
./build.sh
./app
```

### 任务注册与使用

**RequestMapping(name,GET,/user)**

- name      函数名
- GET         请求方式 GET, HEAD, POST, PUT, PATCH, DELETE, OPTIONS, TRACE
- /user      路径

案例:

```c
#include "dyj_server.h"

RequestMapping(name,GET,/user{user}){
    DEBUG_LOG("data = %s\r\n",data);
    #需要动态申请内存，来返回json字符串内容
    char *ret = (char *)malloc(sizeof(char)*100);
    strcpy(ret,"springboot\r\n");

    return ret;
}

```



可使用的参数

```c
typedef char *(*LoopFunction)(Http_head_Def *http_head_Def,char *data,RespHeadNode **resp,int *len,HttpNodeListDef *request);
```

- http_head_Def  请求头消息包括  http协议版本   http请求方式     请求路径
- data   请求数据字符串
- resp 响应头链表指针
- len   响应头链表长度
- request 请求头key-value链表



### 响应头设置

```c
RequestMapping(password,GET,/password){
    DEBUG_LOG("data = %s\r\n",data);
    char *ret = (char *)malloc(sizeof(char)*100);
    //添加resp头部消息
    setRespHeadNode(resp,"name","zhouzhanpei",len);
    
    strcpy(ret,"password\r\n");

    return ret;
}


```

![](https://raw.githubusercontent.com/programzzp/dyj_server/main/img/resp_head.PNG)

### 获取请求头全部消息

```c
RequestMapping(password,GET,/password){
    DEBUG_LOG("data = %s\r\n",data);
    char *ret = (char *)malloc(sizeof(char)*100);

    HttpNodeListDef *head = request;

    while(head != NULL){
        DEBUG_LOG("key=%s\tvalue=%s\r\n",head->httpBodyStructDef.Header,head->httpBodyStructDef.Value);
        head = head->next;
    }
    
    strcpy(ret,"password\r\n");

    return ret;
}


```

![](https://raw.githubusercontent.com/programzzp/dyj_server/main/img/req_head.PNG)

### 通过请求链表中key获取value

```c
RequestMapping(password,GET,/password){
    DEBUG_LOG("data = %s\r\n",data);
    char *ret = (char *)malloc(sizeof(char)*100);
    char value[255];
    //参数3--> value 传出参数
    if(getRequestValue(request,"Host",value)){
        DEBUG_LOG("%s\r\n",value);
    }
    //添加resp头部消息
    setRespHeadNode(resp,"name","zhouzhanpei",len);
    
    strcpy(ret,"password\r\n");

    return ret;
}
//运行结果
192.168.0.105
```



