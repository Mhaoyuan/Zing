#include <iostream>
#include "util.h"
#include "threadpool.h"
#include "list.h"
#include "epoll.h"
using namespace std;

#define DEFAULT_CONFIG "Zconf.conf"

extern struct epoll_event* events;
char* conf_file = DEFAULT_CONFIG;
Z_conf_t conf;

int main() {

    //读取配置文件
    read_conf(conf_file,&conf);

    handle_for_sigpipe();
    //初始化嵌套字开始监听
    int listen_fd = socket_bind_listen(conf.port);

    //设置socket为非阻塞
    int rc = make_socket_non_blocking(listen_fd);

    //创建epoll并组册监听描述符
    int epoll_fd = z_epoll_create(16384);
    z_http_request_t* request = (z_http_request_t*)malloc(sizeof(z_http_request_t));
    z_init_request_t(request,listen_fd, epoll_fd, conf.root);
    z_epoll_add(epoll_fd, listen_fd, request, (EPOLLIN | EPOLLET));

    // 初始化线程池
    z_threadpool_t* tp = threadpool_init(conf.thread_num);

    //初始化计时器
            z_timer_init();
    while(1){
        //得到最近为删除时间和当前时间(等待时间)
        int time = z_find_timer();

        //调用epoll——wait函数，返回接受到事件的数量
        int events_num = z_epoll_wait(epoll_fd, events, MAXEVENTS, -1);

        //处理已经超时的请求
        z_handle_expire_timers();


        z_handle_events(epoll_fd, listen_fd, events, events_num, conf.root, tp);

    }
    return 0;
}