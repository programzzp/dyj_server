//
// Created by redmi on 2024/4/5.
//

#ifndef TIMER_H
#define TIMER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "utile.h"
#include "internet.h"


struct TimerTask{
    int timer;
    int clientfd;
};


#define MAX_TIMER_TASK  100
#define MAX_TIME_OUT    10

typedef struct TimerTask TimerTaskDef;


typedef uint32_t timer_count_t;

void timer_init(void);
void add_timer_task(int clientfd);
void delete_timer_task(int clientfd);
void print_task_timer(void);
void update_timer_task(int clientfd);

#ifdef __cplusplus
}
#endif
#endif //CDEMOS_CONFIG_H
