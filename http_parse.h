//
// Created by genius on 18-8-7.
//

#ifndef ZING_HTTP_PARSE_H_H
#define ZING_HTTP_PARSE_H_H

#define CR '\r'
#define LF '\n'

#include "http_request.h"

int z_http_parse_request_line (z_http_request_t *request);
int z_http_parse_request_body();


#endif //ZING_HTTP_PARSE_H_H
