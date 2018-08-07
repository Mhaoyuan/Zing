#include <iostream>
#include "util.h"
#include "threadpool.h"
using namespace std;
Z_conf_t conf;
void fun(void* arg){
    cout << "pricess" << (int*)arg<< endl;
    sleep(1);
//    return NULL;
}
int main() {
//    cout << read_conf("../Zconf.conf",&conf);


    z_threadpool_t *p = threadpool_init(4);
    int a[10];
    for(int i = 0 ;i < 10;++i) {
        threadpool_add(p, fun, &a[i]);
    }
    sleep(5);
    return 0;
}