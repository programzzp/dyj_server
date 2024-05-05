#include "request/httpBody.h"

/**
 * @brief 解析key与value
 * @param body_buf  需要解析请求头中每一行的key与value
 * @return
 */
HttpBodyStruct_Def* getHttpBodyStruct_Def(const char* body_buf){

    char body_buf_array[255];
    strcpy(body_buf_array,body_buf);
    char *body_point = body_buf_array;
   // printf("getHttpBodyStruct_Def ------> %s\r\n",body_buf_array);

    HttpBodyStruct_Def * body = Malloc(HttpBodyStruct_Def,1);
    char *Header = body->Header;
    char *Value  = body->Value;

    while(*body_point != ':'){
        *Header = *body_point;
        ++Header;
        ++body_point;
    }
    *Header = '\0';

    //判断是否为 ": "
    if(*body_point == ':' && *(body_point+1) == ' '){
        body_point += 2;
    }

    while(*body_point != '\0'){
        *Value = *body_point;
        ++Value;
        ++body_point;
    }
    *Value = '\0';

    return body;
}


/**
 * @brief  添加http请求头消息key_value到链表
 * @param httpBodyStructDef 需要添加的数据
 * @param httpNodeList      链表结构头指针
 */
void add_HttpBodyStruct_Def_in_List(HttpBodyStruct_Def *httpBodyStructDef,HttpNodeListDef **httpNodeList){
    HttpNodeListDef *head = *httpNodeList;
    HttpNodeListDef *httpNodeList_Def = Malloc(HttpNodeListDef,1);
    strcpy(httpNodeList_Def->httpBodyStructDef.Header,httpBodyStructDef->Header);
    strcpy(httpNodeList_Def->httpBodyStructDef.Value,httpBodyStructDef->Value);
    httpNodeList_Def->next = NULL;

    MY_FREE(httpBodyStructDef);
    if(httpBodyStructDef != NULL){
        My_Error("error: HttpBodyStruct_Def Free error\r\n");
    }

    if(head == NULL){
        *httpNodeList = httpNodeList_Def;
    }else{
        HttpNodeListDef *current = *httpNodeList;
        while(current->next !=NULL){
            current = current->next;
        }
        current->next =httpNodeList_Def;
    }
}