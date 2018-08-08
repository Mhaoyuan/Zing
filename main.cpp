#include <iostream>
#include "util.h"
#include "threadpool.h"
#include "list.h"
using namespace std;
Z_conf_t conf;
void fun(void* arg){
    cout << "pricess" << (int*)arg<< endl;
    sleep(1);
//    return NULL;
}
typedef struct A {
    int a;
    int b;
    struct list_head list;
}a;
 typedef struct B{
   int a ;
   int b ;
   struct list_head list;
}b;
int main() {
//    cout << read_conf("../Zconf.conf",&conf);


//    z_threadpool_t *p = threadpool_init(4);
//    int a[10];
//    for(int i = 0 ;i < 10;++i) {
//        threadpool_add(p, fun, &a[i]);
//    }
//    sleep(5);
     a *lista = (a*)malloc(sizeof(a));
     b *listb = (b*)malloc(sizeof(b));
     a *hd;
     lista->a=1;
     lista->b =2;
    INIT_LIST_HEAD(&(listb->list));
    list_add(&(lista->list),&(listb->list));
    list_head *pos;
//    list_for_each(pos,&(listb->list)){
//        hd = list_entry(pos, b,list);
//    }


    return 0;
}