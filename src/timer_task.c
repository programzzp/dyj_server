#include "timer_task.h"
#include <stdio.h>
#include <stdlib.h>

TimerTaskDef timerTask[MAX_TIMER_TASK];


volatile timer_count_t count = 0;


pthread_mutex_t timer_task_lock;

/**
 * 定时线程
*/
void *timer_hander(void *arg){
    while(1){
        if(count > 0){
            usleep(900);
            for (int i = 0; i < count; i++)
            {
                ++timerTask[i].timer; 
                if(timerTask[i].timer >= MAX_TIME_OUT){
                    print_task_timer();
                    epoll_ctl(epfd,EPOLL_CTL_DEL,timerTask[i].clientfd,NULL);
                    close(timerTask[i].clientfd);
                    delete_timer_task(timerTask[i].clientfd);
                }
            }
            
        }
    }
}

void add_timer_task(int clientfd){
    pthread_mutex_lock(&timer_task_lock);

    timerTask[count].clientfd = clientfd; 
    timerTask[count].timer = 0; 
    ++count;

    pthread_mutex_unlock(&timer_task_lock);
}

void delete_timer_task(int clientfd){
    DEBUG_LOG("delete fd = %d\r\n",clientfd);
    int count_index = -1;
    //查询任务队列中的元素是否存在
    for (int i = 0; i < count; i++)
    {
        if(timerTask[i].clientfd == clientfd){
            count_index = i;
        }
    }

    if (count_index == -1)
    {
        DEBUG_LOG("delete_timer_task error\r\n");
        exit(1);
        return;
    }

    //如果为最后一个
    if(count_index == count-1){
        timerTask[count_index].clientfd = -1; 
        timerTask[count_index].timer = -1;
        --count; 
        return;
    }else
    {
        for (int i = count_index; i < count - 1; i++)
        {
            timerTask[i].clientfd = timerTask[i+1].clientfd;
            timerTask[i].timer = timerTask[i+1].timer;
        }
        timerTask[count-1].clientfd = -1; 
        timerTask[count-1].timer = -1;
        --count; 
        return;
    }
    
}


void print_task_timer(void){
    for (int i = 0; i < count; i++)
    {
        DEBUG_LOG("ret = %d\r\n",timerTask[i].clientfd);
    }
}


void timer_init(void){

    pthread_t timer_tid;

    pthread_mutex_init(&timer_task_lock,NULL);

    for (int i = 0; i < MAX_TIMER_TASK; i++)
    {
        timerTask[i].clientfd = -1; 
        timerTask[i].timer = -1; 
    }

    //创建定时线程
    pthread_create(&timer_tid, NULL, timer_hander, (void*)NULL);
    pthread_detach(timer_tid); 

    DEBUG_LOG("%s\r\n",__FILE__);
}


//修改时间
void update_timer_task(int clientfd){
   // DEBUG_LOG("update fd = %d\r\n",clientfd);
    int count_index = -1;

    //查询任务队列中的元素是否存在
    for (int i = 0; i < count; i++)
    {
        if(timerTask[i].clientfd == clientfd){
            count_index = i;
        }
    }

    if (count_index == -1)
    {
        printf("update_timer_task error\r\n");
        exit(1);
        return;
    }
    timerTask[count_index].timer = 0;
}