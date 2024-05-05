#ifndef WEBSOCKETCONNECT_H
#define WEBSOCKETCONNECT_H

#ifdef __cplusplus
extern "C" {
#endif
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>



void base64AndCha1_Code(char *sou_string,char *tar_string);
char *WebSocketResponse_head(char *WebSocket_Key);

#ifdef __cplusplus
}
#endif
#endif