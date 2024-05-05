#ifndef INTERNET_H_
#define INTERNET_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include "config.h"
#include <unistd.h>
#include <fcntl.h>


#define IPV4     AF_INET
#define IPV6     AF_INET6


#define TCP     SOCK_STREAM
#define UDP     SOCK_DGRAM

#define NET_LOG(format, ...)    printf(format, ##__VA_ARGS__)


typedef struct sockaddr_in      SOCKETADDR_IN;
typedef struct sockaddr         SOCKETADDR;
typedef struct epoll_event      EPOLL_DEF;


extern EPOLL_DEF event,ep[LISTEN_NUM];
extern int epfd;

int socket_t(int domain, int type);

void bind_t(int server_fd,int type,int port);

void listen_t(int server_fd);

int accept_t(int server_fd);

int socket_init(int domain, int type);

int epoll_init(struct epoll_event *epoll_def_init,int server_fd);

void epoll_add(int epfd,int fd,EPOLL_DEF *epoll_def);

void epoll_delete(int epfd,int clientfd);

void epoll_accpet(int server_fd,EPOLL_DEF *epoll_def,int epfd);
#ifdef __cplusplus
}
#endif

#endif