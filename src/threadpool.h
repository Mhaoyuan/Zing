//
// Created by genius on 18-8-7.
//

#ifndef ZING_THREADPOOL_H
#define ZING_THREADPOOL_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
// task node
typedef struct Z_task{
    void (*fun)(void*);
    void* arg;
    struct Z_task* next; // next tast
}z_task_t;

typedef struct threadpool{
    pthread_mutex_t lock;  //互斥锁
    pthread_cond_t cond; //条件变量
    pthread_t *threads; // 线程
    z_task_t *head;    // 任务队列
    int thread_count;  //线程数
    int queue_size; //任务队列长度
    int shutdown; //关机模式
    int started;
}z_threadpool_t;

typedef  enum{
    z_tp_invalid = -1,
            z_tp_lock_fail = -2,
                    z_tp_already_shutdown = -3,
                            z_tp_cond_broadcast = -4,
                                    z_tp_thread_fail = -5,
}z_threadpool_error_t;

typedef enum{
    immediate_shutdown = 1,
    graceful_shutdown =2,
}z_threadpool_sd_t;

static void* threadpool_worker(void *arg);
int thread_destory(z_threadpool_t *pool, int graceful);
z_threadpool_t * threadpool_init(int thread_num);
int threadpool_add(z_threadpool_t* pool, void(*func)(void*), void *arg);




#endif //ZING_THREADPOOL_H
