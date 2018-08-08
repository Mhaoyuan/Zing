//
// Created by genius on 18-8-7.
//

#ifndef ZING_HTTP_PARSE_H_H
#define ZING_HTTP_PARSE_H_H

#define CR '\r'
#define LF '\n'

#include "http_request.h"
// http 请求行解析
int z_http_parse_request_line (z_http_request_t *request);
// http请求体解析
int z_http_parse_request_body(z_http_request_t *request);

#endif //ZING_HTTP_PARSE_H_H
