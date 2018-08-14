//
// Created by genius on 18-8-7.
//

#ifndef ZING_EPOLL_H
#define ZING_EPOLL_H

#include <sys/epoll.h>
#include "http.h"
#include "threadpool.h"
#include "http_request.h"


#define MAXEVENTS 1638400

int z_epoll_create(int flags);
int z_epoll_add(int epoll_fd , int fd,z_http_request_t* request, int events);
int z_epoll_mod(int epoll_fd, int fd, z_http_request_t* request, int events);
int z_epoll_del(int epoll_fd, int fd, z_http_request_t* request,int events);
int z_epoll_wait(int epoll_fd, struct epoll_event *events, int max_events, int timeout);
void z_handle_events(int epoll_fd, int listen_fd, struct epoll_event* events,
                    int events_num, char* path, z_threadpool_t* tp);
#endif //ZING_EPOLL_H
