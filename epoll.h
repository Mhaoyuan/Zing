//
// Created by genius on 18-8-7.
//

#ifndef ZING_EPOLL_H
#define ZING_EPOLL_H

#include <sys/epoll.h>

#include "threadpool.h"


#define MAXEVENTS 1024

int z_epoll_create(int flags);
int z_epoll_add(int epoll_fd , int fd,);
#endif //ZING_EPOLL_H
