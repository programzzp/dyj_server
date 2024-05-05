#include "threadPool.h"
//锁
pthread_mutex_t queueLock;
ThreadPool *queue = NULL; 


//
void pushQueueThread(ThreadPool **task,int fd){


    pthread_mutex_lock(&queueLock);
    ThreadPool *task_node = (ThreadPool *)malloc(sizeof (ThreadPool));
    if(task_node == NULL){
        printf("task_node == NULL\r\n");
        exit(1);
    }
    task_node->fd = fd;
    task_node->next = NULL;
    ThreadPool *head = *task;

    if(head == NULL){
        *task = task_node;
        pthread_mutex_unlock(&queueLock);
    }else{
        task_node->next = head;
        *task = task_node;
        pthread_mutex_unlock(&queueLock);
    }

}

int popQueueThread(ThreadPool **task){

    int fd = -1;
    if(*task == NULL){
        return -1;
    }

    ThreadPool *current = *task;
     if(current->next == NULL){
        fd = current->fd;
        free(current);
        current = NULL;
        *task = NULL;
        return fd;
    }


    ThreadPool *previous = NULL;

    while(current->next != NULL){
        previous = current;
        current = current->next;
    }

    fd = current->fd;
    free(current);
    previous->next = NULL;
    current = NULL;

    return fd;
}



int popQueue(void){
    
    int fd;
    while(1){
        pthread_mutex_lock(&queueLock);
        fd = popQueueThread(&queue);
        pthread_mutex_unlock(&queueLock);
        if(fd != -1){
            break;
        }
    }
   
    return fd;
}




void pushQueue(int fd){
    pushQueueThread(&queue,fd);
}



void *popTask(void *arg){
    int fd;

    while(true){
        fd = popQueue();
        httpQueueService(fd);
    }


    printf("end\r\n");
    return NULL;
}


void ThreadQueueInit(int size){
    //初始化锁
    pthread_mutex_init(&queueLock,NULL);

    pthread_t delete_pid;

    for (int i = 0; i < size; i++)
    {
        pthread_create(&delete_pid,NULL,popTask,NULL);
        pthread_detach(delete_pid);
    }

}