//
// Created by genius on 18-8-8.
//

#ifndef ZING_RIO_H
#define ZING_RIO_H

#include<sys/types.h>
#define  RIO_BUFSIZE 8192

typedef struct{
    int rio_fd;                     // dsecriptor for this internal buf
    ssize_t rio_cnt;                //unread bytes in internal buf
    char *rio_bufptr;               //next unread byte in internal buf
    char rio_buf[RIO_BUFSIZE];      //internal buffer
}rio_t;

ssize_t rio_readn(int fd, void* usrbuf, size_t n);
ssize_t rio_writen(int fd, void* uerbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t* rp, void *usrbuf, size_t maxlen);


#endif //ZING_RIO_H
