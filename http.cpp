//
// Created by genius on 18-8-7.
//

#include "http.h"
#include <errno.h>

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
        if(strcmp(type,z_mime[i].value) == 0)
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

void server_static(int fd){

}



