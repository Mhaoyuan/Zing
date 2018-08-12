//
// Created by genius on 18-8-7.
//
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "util.h"
#include "http_request.h"
#include "epoll.h"
//#include "util.h"


int read_conf(char* filename,Z_conf_t* conf ){
    FILE* fp = fopen(filename, "r");
    if(!fp) {
        perror("fopen");
        return Z_CONF_ERROE;
    }
    char buff[BUFIEN];
    int buff_len = BUFIEN;
    char* curr_pos = buff;
    char *delim_pos = NULL;
    int i = 0;
    int pos = 0;
    int line_len = 0;
    while(fgets(curr_pos, buff_len - pos,fp))
    {
        delim_pos = strstr(curr_pos, DELIM);
        if(!delim_pos)
            return Z_CONF_ERROE;
        if(curr_pos[strlen(curr_pos)-1] == '\n'){
            curr_pos[strlen(curr_pos) -1] = '\0';
        }
        //get root
        if(strncmp("root", curr_pos,4)==0){
            delim_pos +=1;
            while(*delim_pos!='#'){
                conf->root[i++] = *delim_pos;
                ++delim_pos;
            }
        }
        //get port
        if(strncmp("port", curr_pos,4)==0) {
            conf->port = atoi(delim_pos+1);
        }
        if(strncmp("thread_num", curr_pos,9)==0) {
            conf->thread_num = atoi(delim_pos+1);
        }

        line_len = strlen(curr_pos);
        curr_pos+=line_len;
    }
    fclose(fp);
    return Z_CONF_OK;
}

int socket_bind_listen(int port){
    port = ((port<=1024)||(port >= 65535)? 6666: port);

    int listen_fd = 0;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        perror("socket:");
        return -1;
    }
    int optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET,SO_REUSEADDR ,(const void*)&optval, sizeof(int)) == -1){
        perror("setsockopt:");
        return -2;
    }

    struct  sockaddr_in serveraddr;
    bzero((char*)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listen_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("bind:");
        return -3;
    }

    if(listen(listen_fd, LISTENQ) == -1) {
        perror("listen");
        return -4;
    }
    return listen_fd;
}

int make_socket_non_blocking(int fd){
    int flag = fcntl(fd,F_GETFL,0);
    if(flag -1 )
        return -1;
    flag|=O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) ==-1)
        return -1;
}

void accept_connection(int listen_fd, int epoll_fd, char* path){
    struct sockaddr_in client_addr;
    memset(&client_addr, 0 , sizeof(struct sockaddr_in));
    socklen_t client_addr_len = 0;
    int accept_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(accept_fd==-1) {
        perror("accpt:");
    }
    //设置为非阻塞方式
//    int rc = make_socket_non_blocking(accept_fd);
    // 申请z_http_request_t 类型的结点并初始化
    z_http_request_t* request = (z_http_request_t*)malloc(sizeof(z_http_request_t));
    z_init_request_t(request, accept_fd, epoll_fd,path);

    //文件描述符可读,边缘触发,保证一个socket连接在任意时刻之被一个线程处理
    z_epoll_add(epoll_fd, accept_fd,request, (EPOLLIN| EPOLLET | EPOLLONESHOT));
    z_add_timer(request, TIMEOUT_DEFAULT, z_http_close_conn);


}

void handle_for_sigpipe(){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, NULL))
        return;
}
