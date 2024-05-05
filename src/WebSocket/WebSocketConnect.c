#include "WebSocket/WebSocketConnect.h"
#include <string.h>
#include <malloc.h>



#define STRLEN (20)
#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

static int base64_encode(char *in_str, int in_len, char *out_str);



void base64AndCha1_Code(char *sou_string,char *tar_string){
    char sou_buf[255];
    strcpy(sou_buf,sou_string);

    strcat(sou_buf,GUID);

    unsigned char cha1_buf[STRLEN];
    SHA1((unsigned char *)sou_buf,strlen((char *)sou_buf),cha1_buf);
    base64_encode((char *)cha1_buf,STRLEN,tar_string);
}



static int base64_encode(char *in_str, int in_len, char *out_str)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    size_t size = 0;
    if (in_str == NULL || out_str == NULL)
        return -1;
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_write(bio, in_str, in_len);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bptr);
    memcpy(out_str, bptr->data, bptr->length);
    out_str[bptr->length] = '\0';
    size = bptr->length;
    BIO_free_all(bio);
    return size;
}



char *WebSocketResponse_head(char *WebSocket_Key){
    char *WebSocket_Accept = (char *)malloc(sizeof(char)*1024);
    memset(WebSocket_Accept,0,1024);
    char WebSocketAccept[255];
    char code_key[150];

    strcpy(WebSocket_Accept,"HTTP/1.1 101 Switching Protocols\r\n");
    strcat(WebSocket_Accept,"Upgrade: websocket\r\n");
    strcat(WebSocket_Accept,"Connection: Upgrade\r\n");
    base64AndCha1_Code(WebSocket_Key,code_key);
    sprintf(WebSocketAccept,"%s: %s\r\n","Sec-WebSocket-Accept",code_key);
    strcat(WebSocket_Accept,WebSocketAccept);
    strcat(WebSocket_Accept,"\r\n");

    return WebSocket_Accept;

}