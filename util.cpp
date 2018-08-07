//
// Created by genius on 18-8-7.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "util.h"
//#include "util.h"


int read_conf(char* filename,Z_conf_t* conf ){
    FILE* fp = fopen(filename, "r");
    if(!fp)
        return Z_CONF_ERROE;
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
        if(curr_pos[strlen(curr_pos)-1] == 'n'){
            curr_pos[strlen(curr_pos) -1] = '0';
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
            conf->port = atoi(delim_pos+1);
        }

        line_len = strlen(curr_pos);
        curr_pos+=line_len;
    }
    fclose(fp);
    return Z_CONF_OK;

}

