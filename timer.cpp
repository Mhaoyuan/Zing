//
// Created by genius on 18-8-10.
//

#include "timer.h"
#include "sys/time.h"


z_pq_t z_timer;
size_t z_current_msec;

int timer_comp(void* ti, void* tj){
    z_timer_t* timeri = (z_timer_t*)ti;
    z_timer_t* timerj = (z_timer_t*)tj;
    return (timeri->key < timerj->key) ? 1:0;
}

void z_time_update(){
    //获取当前时间
    struct timeval tv;
    int rc = gettimeofday(&tv, NULL);
    z_current_msec = ((tv.tv_sec* 1000)+(tv.tv_usec / 1000));
}

int z_timer_init(){
    //建立连接后立即初始化
    //初始化队列大小 Z_PQ_DAFAULT_SIZE = 10
    int rc = z_pq_init(&z_timer,timer_comp,Z_PQ_DEFAULT_SIZE);
    //更新当前时间
    z_time_update();
    return 0;
}

int z_find_timer(){
    int time;
    while(!z_pq_is_empty(&z_timer)){
        // 更新当前时间
        z_time_update();
        //timer_node 指向最小结点
        z_timer_t* timer_node = (z_timer_t*)z_pq_min(&z_timer);
        //如果已经标记为删除，则释放次时间结点
        if(timer_node->deleted){
            int rc =  z_pq_delimin(&z_timer);
            free(timer_node);
            continue;
        }
        //此时timer_node 为时间最小结点
        time = (int)(timer_node->key - z_current_msec);
        time = (time > 0)?time:0;
        break;
    }
    return time;
}

void z_handle_expire_timers(){
    while(!z_pq_is_empty(&z_timer))
    {
        //更新当前时间
        z_time_update();
        z_timer_t* timer_node = (z_timer_t*)z_pq_min(&z_timer);
        //如果以删除释放此结点
        if(timer_node->deleted){
            int rc = z_pq_delimin(&z_timer);
            free(timer_node);
            continue;
        }
        //最早加入的结点大于最近时间（未超时）
        //结束超时检查
        if(timer_node->key > z_current_msec)
        {
            return;
        }
        //出现了没有被标记删除，但是超时， 调用handle处理（这是因为，任务一直没有执行do_request，可能是在任务队列中，也可能一直没被epoll唤醒）
        if(timer_node->handler){
            timer_node->handler(timer_node->request);
        }
        int rc = z_pq_delimin(&z_timer);
        free(timer_node);
    }
}

void z_add_timer(z_http_request_t *request, size_t timeout, time_handler_pt handler){
    z_time_update();
    //申请新的z_timer_t 结点， 并加入到z_http_request_timer下
    z_timer_t* timer_node = (z_timer_t*)malloc(sizeof(z_timer_t));
    request->timer = timer_node;
    // 加入时设置时间超时值与删除信息
    timer_node->key = z_current_msec+ timeout;
    timer_node->deleted = 0;
    timer_node->handler = handler;
    // 反向设置指到request 结点
    timer_node->request =request;
    //将新结点加入优先队列
    int rc = z_pa_insert(&z_timer, timer_node);
}

void z_del_timer(z_http_request_t* request){
    z_time_update();
    //惰性删除
    //标记为以删除， 在find_timer 和 handle_expire_timer 检查队列中删除，注意只是删除timer_node, request 还会复用 加入新时间结点
    z_timer_t* timer_node = (z_timer_t*)request->timer;
    timer_node->deleted = 1;
}


