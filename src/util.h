//
// Created by genius on 18-8-7.
//

#ifndef ZING_UTIL_H
#define ZING_UTIL_H

#define PATHLEN 128
#define LISTENQ 81920
#define BUFIEN 8192
#define DELIM "="
#define Z_CONF_OK  0
#define Z_CONF_ERROE -1

#define MIN(a,b) ((a) < (b) ? (a):(b))

typedef struct  Z_conf{
    char root[PATHLEN];
    int port;
    int thread_num;
}Z_conf_t;

int read_conf(char* filename,Z_conf_t* conf );
int socket_bind_listen(int port);
int make_socket_non_blocking(int fd);
void accept_connection(int listen_fd, int epoll_fd, char* path);
void handle_for_sigpipe();
#endif //ZING_UTIL_H

