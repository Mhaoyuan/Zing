//
// Created by genius on 18-8-7.
//

#include "http_request.h"

static int z_http_process_ignore(z_http_request_t* request, z_http_out_t* out, char* data, int len);
static int z_http_process_connection(z_http_request_t* request, z_http_out_t* out,char *data, int len);
static int z_http_process_if_modified_since(z_http_request_t *request, z_http_out_t*out, char* data, int len);

z_http_header_handle_t z_http_headers_in[] = {
        {"Host",z_http_process_ignore},
        {"Connection", z_http_process_connection},
        {"If-Modified-Sice",z_http_process_if_modified_since},
        {"",z_http_process_ignore}
};

static int z_http_process_ignore(z_http_request_t* request, z_http_out_t* out, char* data, int len){
    (void) request;
    (void) out;
    (void) len;
    return 0;
}

//处理连接方式
static int z_http_process_connection(z_http_request_t* request, z_http_out_t* out, char* data, int len){
    (void) request;
    // 记录请求是否为keep-alive
    if(strncasecmp("keep-alive", data, len) ==0){
        out->keep_alive =1;
    }
    return 0;
}

static int z_http_process_if_modified_since(z_http_request_t* request,z_http_out_t* out, char* data, int len){
    (void) request;
    (void) len;
    struct tm tm;
    //转换data格式为GMT
    if(strptime(data,"%a,%d %b %Y %H:%M:%S GMT",&tm) == (char*)NULL){
        return 0;
    }
    //将时间转换为1970年1月1日以来的持续秒数

    time_t client_time = mktime(&tm);
    double  time_diff = difftime(out->mtime, client_time);

    // 微妙时间内为修改status显示未修改， modify字段设置为1
    if(fabs(time_diff)< 1e-6){
        out->modified = 0;
        out->status = Z_HTTP_NOT_MODIFID;
    }
    return 0;
}

//初始化请求数据
int z_init_request_t(z_http_request_t *request ,int fd ,int epoll_fd, char* path){
    request->fd = fd;
    request->epoll_fd = epoll_fd;
    request->pos = 0;
    request->last = 0;
    request->state = 0;
    request->root = path;
    INIT_LIST_HEAD(&(request->list));
    return 0;
}
//初始化响应数据结构
int z_init_out(z_http_out_t *out,int fd){
    out->fd = fd;
    out-> keep_alive = 1; //默认为1 连接不断开
    out->modified = 1;
    out->status = 200;
}

//取出request->list 中的头信息 根就头信息执行不同的函数 对out 内容进行修改
void z_http_handle_header(z_http_request_t* request, z_http_out_t* out){
    list_head* pos;
    z_http_header_t *hd;
    z_http_header_handle_t *header_in;
    int len;
    list_for_each(pos,&(request->list)){
        hd = list_entry(pos,z_http_header_t,list);
        for(header_in = z_http_headers_in; strlen(header_in->name) > 0; header_in++){
            if(strncmp((char*)hd->key_start,header_in->name, (char*)hd->key_end - (char*)hd->key_start)==0)
            {
                len = (char*)hd->value_end - (char*)hd->value_start;
                (*(header_in->handler))(request, out, (char*)hd->value_start,len);
            }
        }
        list_del(pos);
        free(hd);
    }
}

const char* get_shoring_from_status_code(int status_code){
    if(status_code == Z_HTTP_OK){
        return "OK";
    }
    if(status_code == Z_HTTP_NOT_MODIFID){
        return "Not Modified";
    }
    if(status_code == Z_HTTP_NOT_FOUND){
        return "Not Found";
    }
    return "Unknown";
}

int z_http_close_conn(z_http_request_t* request){
    close(request->fd);
    free(request);
    return 0;
}