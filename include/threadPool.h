#ifndef THREADPOOL_H
#define THREADPOOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include "webserver.h"

typedef struct ThreadPool{
    int fd;
    struct ThreadPool *next;
}ThreadPool;



void pushQueueThread(ThreadPool **task,int fd);
int popQueueThread(ThreadPool **task);
int popQueue(void);
void pushQueue(int fd);

void ThreadQueueInit(int size);

#ifdef __cplusplus
}
#endif

#endif