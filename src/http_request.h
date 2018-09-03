//
// Created by genius on 18-8-7.
//

#ifndef ZING_HTTP_REQUEST_H
#define ZING_HTTP_REQUEST_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "list.h"
#include "util.h"
//#include <list>
#define Z_AGAIN EAGAIN

#define Z_HTTP_PARSE_INVALID_METHOD 10
#define Z_HTTP_PARSE_INVALID_REQUEST 11
#define Z_HTTP_PARSE_INVALID_HEADER 12

#define Z_HTTP_UNKNOWN 0x0001
#define Z_HTTP_GET 0X0002
#define Z_HTTP_HEAD 0X0004
#define Z_HTTP_POST 0x0008

#define Z_HTTP_OK 200
#define Z_HTTP_NOT_MODIFID 304
#define Z_HTTP_NOT_FOUND 404
#define MAX_BUF 8124

typedef struct z_http_request{
    char* root;             //配置目录
    int fd;                 //描述符（监听、连接）
    int epoll_fd;           //epoll描述符
    char buff[MAX_BUF];      //用户缓存
    int method;             //请求方法
    int state;              //请求头解析状态

    size_t pos;             // 用来访问buff
    size_t last;            //同上


    void* request_start;
    void* method_end;
    void* uri_start;
    void* uri_end;
    void* path_start;
    void* path_end;
    void* query_start;
    void* query_end;
    int http_major;
    int http_minor;
    void* request_end;


    struct list_head list;          //存放请求头， list.h 中定义

    void* cur_header_key_start;
    void* cur_header_key_end;
    void* cur_header_value_start;
    void* cur_header_value_end;
    void* timer;
}z_http_request_t;

typedef struct z_http_out{       //响应
    int fd;
    int keep_alive;
    time_t mtime;
    int modified;
    int status;
}z_http_out_t;

typedef  struct z_http_header{
    void* key_start;
    void* key_end;
    void* value_start;
    void* value_end;

    struct list_head list;

}z_http_header_t;

typedef int (*z_http_header_handle_pt)(z_http_request_t* request, z_http_out_t* out, char* data, int len);

typedef struct z_http_header_handle{
    char* name;
    z_http_header_handle_pt handler;
}z_http_header_handle_t;

extern z_http_header_handle_t z_http_headers_in[];


int z_init_request_t(z_http_request_t *request ,int fd ,int epoll_fd, char* path);
void z_http_handle_header(z_http_request_t* request, z_http_out_t* out);
const char* get_shoring_from_status_code(int status_code);
int z_http_close_conn(z_http_request_t* request);
int z_init_out(z_http_out_t *out,int fd);



#endif //ZING_HTTP_REQUEST_H
