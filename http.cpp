//
// Created by genius on 18-8-7.
//

#include "http.h"
#include <errno.h>
//#include <xf86drm.h>

static const char* get_file_type(const char* type);
static void parse_uri(char *uri, int length, char*filename, char *query);
static void do_error(int fd, char* cause, char *err_num, char *short_msg, char* long_msg);
static void server_static(int fd, char* filename, size_t filesize, z_http_out_t* out);

static char* ROOT = NULL;

mime_type_t z_mime[] = {
        {".html", "text/html"},
        {".xml", "text/xml"},
        {".xhtml", "application/xhtml+xml"},
        {".txt", "text/plain"},
        {".rtf", "application/rtf"},
        {".pdf", "application/pdf"},
        {".word", "application/msword"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".au", "audio/basic"},
        {".mpeg", "video/mpeg"},
        {".mpg", "video/mpeg"},
        {".avi", "video/x-msvideo"},
        {".gz", "application/x-gzip"},
        {".tar", "application/x-tar"},
        {".css", "text/css"},
        {NULL ,"text/plain"}
};

static void parse_uri(char* uri_start, int uri_length, char* filename, char* query)
{
    uri_start[uri_length] = '\0';
    // 找到‘？’ 位置界定非参数部分
    char *delim_pos = strchr(uri_start,'?');
    int filename_length = (delim_pos!=NULL)? ((int)(delim_pos - uri_start)): uri_length;
    strcpy(filename,ROOT);
    //将uri中属于‘？’之前的部分内容追加到filename
    strncat(filename, uri_start,uri_length);
    //在请求中找到最后一个‘/’
    char *last_comp = strrchr(filename,'/');
    //找到最后一个‘.’ （用来判断请求是否具体文件类型）
    char *last_dot = strrchr(last_comp,'.');
    //文件目录 追加‘/’
    if((last_dot==NULL) && filename[strlen(filename)-1] != '/'){
        strcat(filename,"/");
    }

    if(filename[strlen(filename)-1] =='/'){
        strcat(filename,"index.html");
    }
    return;
}

const char*get_file_type(const char* type){
    for(int i = 0; z_mime[i].type!=NULL; ++i){
        if(strcmp(type,z_mime[i].type) == 0)
            return z_mime[i].value;
    }
    return "text/plain";
}

//响应错误信息
void do_error(int fd, char* cause, char* err_num, char* short_msg, char* long_msg){
    // 响应头缓冲（512字节）和数据缓冲（8192字节）
    char header[MAXLINE];
    char body[MAXLINE];

    //用log_msg和cause字节填充错误响应体
    sprintf(body, "<html<title>Zing Error<title>>");
    sprintf(body, "%s <body bgcolor = ""ffffff"">\n", body);
    sprintf(body, "%s%s : %s\n",body, err_num, short_msg);
    sprintf(body, "%s<p>%s : %s\n </p>",body, long_msg, cause);
    sprintf(body, "%s<hr><em>Zing web server </em>\n</bbody></html>", body);

    //返回错误码，组织错误响应头
    sprintf(header, "HTTP/1.1 %s %s \r\n", err_num, short_msg);
    sprintf(header, "%sServer: Zing\r\n",header);
    sprintf(header, "%sContent-type: text/html\r\n",header);
    sprintf(header, "%sConnection: close\r\n",header);
    sprintf(header, "%sContent-length: %d \r\n\r\n",header, (int)strlen(body));

    rio_writen(fd,header,strlen(header));
    rio_writen(fd,body,strlen(body));
}
//处理静态文件

void server_static(int fd, char* filename, size_t filesize, z_http_out_t* out){
        //响应头缓冲（512字节） 和数据缓冲（8192字节）
    char header[MAXLINE];
    char buff[SHORTLINE];
    struct tm tm;

    //返回响应报文头， 包含HTTP版本号状态码以及状态码对应的短描述

    sprintf(header,"HTTP/1.1 %d %s\r\n", out->status, get_shoring_from_status_code(out->status));

    //返回响应头
    //Connection, Keep-Alive, Content-type, Content-length, Last-Modified

    if(out->keep_alive){
        //返回默认的持续连接模式以及超时时间（默认500ms）
        sprintf(header,"%sConnection: keep-alive\r\n",header);
        sprintf(header, "%sKeep-Alive: timeout = %d\r\n", header, TIMEOUT_DEFAULT);
    }
    if(out->modified){
        //得到文件类型并填充Content-type字段
        const char* filetype = get_file_type(strrchr(filename,'.'));
        sprintf(header,"%sContent-type: %s\r\n", header,filetype);
        //通过Content-length返回文件大小
        sprintf(header,"%sContent-length: %zu\r\n",header,filesize);
        //得到最后修改时间并填充Lat_Modified字段

        localtime_r(&(out->mtime), &tm);
        strftime(buff, SHORTLINE, "%a, %d %b %Y %H:%M:%S GMT",&tm);
        sprintf(header,"%sLast-Modified: %s\r\n",header, buff);
    }
    sprintf(header,"%sServer : Zing\r\n",header);

    //空行（must）
    sprintf(header,"%s\r\n",header);
    size_t  send_len = (size_t)rio_writen(fd,header,strlen(header));
    if(send_len != strlen(header)){
        perror("Send header failed");
        return;
    }
    int src_fd = open(filename,O_RDONLY,0);
    char *src_addr = (char*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, src_fd,0);
    close(src_fd);

    //发送文件并校验完整性
    send_len = rio_writen(fd, src_addr, filesize);
    if(send_len != filesize){
        perror("Send file failed");
        return;
    }
    munmap(src_addr, filesize);
}
int error_process(struct stat* sbufptr, char* filename, int fd){
    //处理文件找不到错误
    if(stat(filename, sbufptr)< 0){
        do_error(fd, filename, "404", "Not Found", "Zing can't rend the file");
        return 1;
    }

    if(!S_ISREG((*sbufptr).st_mode) || !(S_IRUSR & (*sbufptr).st_mode)){
        do_error(fd, filename, "403", "forbidden", " Zing can't read the file");
        return 1;
    }

    return 0;
}

void do_request(void* ptr){
    z_http_request_t* request = (z_http_request_t*)ptr;
    int fd= request->fd;
    printf("asd");
    ROOT = request->root;
    char filename[SHORTLINE];
    struct stat sbuf;
    int rc, n_read;
    char* plast = NULL;
    size_t remain_size;

    z_del_timer(request);

    while(1){
        //plast 指向缓冲区buff当前可写入的第一个字节位置i， 这里取余为了实现循环缓冲
        plast = &request->buff[request->last % MAX_BUF];

        // remian_size 表示缓冲区当前剩余可写入字节数
        remain_size = MIN(MAX_BUF - (request->last - request->pos) - 1,MAX_BUF - request->last % MAX_BUF);
        //从连接描述符fd读取数据并复制到用户缓冲区plast 指向位置
        n_read = read(fd, plast, remain_size);
        perror("read");

        //已经读到文件尾或者没有数据可读，断开连接
        if(n_read==0)
            goto err;
        //非EAGIN 错误断开连接
        if(n_read < 0 && errno!=Z_AGAIN)
            goto err;
        //Non-blocking 下的errno 返回EAGAIN则重置定时器（进入此循环表示连接被激活），重新组册，在不断开TCP连接的情况下重新等待下一次用户请求
        if((n_read< 0)&& errno == Z_AGAIN)
            break;

        //更新读到的总字节数
        request->last+=n_read;

        //解析请求报文行
        rc = z_http_parse_request_line(request);
        if(rc == Z_AGAIN)
            continue;
        else if(rc != 0)
            goto  err;

        //解析请求报文体
        rc = z_http_parse_request_body(request);
        if(rc ==Z_AGAIN)
            continue;
        else if (rc!= 0)
            goto err;

        //分配并初始化返回数据库结构

        z_http_out_t* out = (z_http_out_t*)malloc(sizeof(z_http_out_t));
        z_init_out(out,fd);

        //解析URI， 并获取文件名
        parse_uri((char*)request->uri_start, (char*)request->uri_end - (char*)request->uri_start, filename, NULL);



        //处理响应错误
        if(error_process(&sbuf, filename,fd))
            continue;
        //获取文件最后一次修改时间
        out->mtime = sbuf.st_mtime;

        //处理静态请求文件类型
        server_static(fd,filename, sbuf.st_size,out);

        //释放返回数据结构
        free(out);

        //处理HTTP长连接，控制TCP是否断开连接
        if(!out->keep_alive)
            goto close;

    }

    //一次请求响应结束后并不直接断开TCP连接， 而是重置状态
    // 修改已经组册描述符的事件类型
    // 重置定时器，等待下一次请求生效
    z_epoll_mod(request->epoll_fd, request->fd, request, (EPOLLIN | EPOLLET | EPOLLONESHOT) );
    z_add_timer(request, TIMEOUT_DEFAULT, z_http_close_conn);


    err:
    close:
    //发生错误或正常关闭
    //关闭相应的连接描述符，释放用户请求数据结构
    z_http_close_conn(request);

}

