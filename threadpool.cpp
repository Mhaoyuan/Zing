//
// Created by genius on 18-8-7.
//

#include "threadpool.h"
#include <pthread.h>
//释放线程池
static int threadpool_free(z_threadpool_t* pool) {
    if(pool == NULL|| pool->started > 0)  //判断线程池是否已经释放过,或者还有正在运行的任务
        return -1;
    if (pool->threads)                    //释放线程
        free(pool->threads);

    z_task_t *old;
    while(pool-> head->next){               //释放任务队列
        old = pool->head->next;
        pool->head->next = pool->head->next->next;
        free(old);
    }
    return 0;
};
static void* threadpool_worker(void *arg)
{
    if(arg == NULL)
        return NULL;
    z_threadpool_t* pool = (z_threadpool_t*) arg;
    z_task_t* task;
    while(1)
    {
        pthread_mutex_lock(&(pool->lock));                 //加互斥莎
        while((pool->queue_size == 0) && !(pool->shutdown))
            pthread_cond_wait(&(pool->cond),&(pool->lock));   // 当任务队列为空， 并且线程池没有关机 ，等待条件变量解锁

        if(pool->shutdown == immediate_shutdown)
            break;
        else if ((pool->shutdown == graceful_shutdown)&&(pool->queue_size==0))
            break;

        task = pool->head->next;                    // 取出head的任务
        if(task==NULL){
            pthread_mutex_unlock(&(pool->lock));
            continue;
        }
        pool->head = pool->head->next;              //摘掉head链表被取出的任务

        pool->queue_size--;
        pthread_mutex_unlock(&(pool->lock));

        (*(task->fun))(task->arg);
        free(task);
    }
    pool->started--;
    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);
    return NULL;
}

int thread_destory(z_threadpool_t *pool, int graceful){
    if(pool==NULL )
    {
        return z_tp_invalid;
    }
    if(pthread_mutex_lock(&(pool->lock))!= 0)
        return z_tp_lock_fail;
    int err = 0;

    do{
        if(pool->shutdown){
            err = z_tp_already_shutdown;
            break;
        }
        pool->shutdown = (graceful) ? graceful_shutdown : immediate_shutdown;

        if(pthread_cond_broadcast(&(pool->cond))!=0){
            err = z_tp_cond_broadcast;
            break;
        }

        if(pthread_mutex_unlock(&(pool->lock))!= 0)
        {
            err = z_tp_lock_fail;
            break;
        }
        for (int i = 0; i< pool-> thread_count; i++){
            if(pthread_join(pool->threads[i], NULL)!=0){
                err = z_tp_thread_fail;
            }
        }
    }while(0);
    if(!err){
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->cond));
        threadpool_free(pool);
    }
    return err;
}


z_threadpool_t * threadpool_init(int thread_num){

    // 分配线程池
   z_threadpool_t *pool;
   if((pool = (z_threadpool_t*)malloc(sizeof(z_threadpool_t))) == NULL)
       goto err;
   // threads 指针指向线程数组（tid），数组大小 = 线程数
   pool->thread_count = 0;
   pool->queue_size = 0;
   pool->started = 0;
   pool->shutdown = 0;
   pool->threads = (pthread_t*)malloc(sizeof(pthread_t)*thread_num);

   //分配初始化tast头节点
   pool->head = (z_task_t*)malloc(sizeof(z_task_t));
   if((pool->threads ==NULL)|| pool->head == NULL)
       goto err;
   pool->head->fun = NULL;
   pool->head->arg = NULL;
   pool->head->next = NULL;

   if(pthread_mutex_init(&(pool->lock),NULL)!= 0)
       goto err;
   if(pthread_cond_init(&(pool->cond), NULL) != 0)
       goto err;
   for (int i = 0; i< thread_num; ++i){
       if(pthread_create(&(pool->threads[i]), NULL, threadpool_worker,(void*)pool)!=0){
           thread_destory(pool,0);
           return NULL;
       }
       pool->thread_count++;
       pool->started++;
   }
   return pool;
   err:if(pool)
        threadpool_free(pool);
    return NULL;

}


int threadpool_add(z_threadpool_t* pool, void(*func)(void*), void *arg){
    int re, err = 0;
    if(pool ==NULL || func ==NULL)
        return -1;
    if(pthread_mutex_lock(&(pool->lock))!= 0){
        return -1;
    }
    if(pool->shutdown){
        err = z_tp_already_shutdown;
        return err;
    }

    z_task_t* task = (z_task_t*)malloc(sizeof(z_task_t));
    if(task ==NULL)
        return err;
    task -> fun = func;
    task -> arg = arg;
    task->next = pool->head->next;
    pool->head->next = task;
    pool->queue_size++;

    re = pthread_cond_signal(&(pool->cond));   //线程有空闲的时候， 激活threadpool 函数
    if(pthread_mutex_unlock(&(pool->lock))!=0)
    {
        err = z_tp_lock_fail;
        return err;
    }


}
