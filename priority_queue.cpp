//
// Created by genius on 18-8-9.
//

#include<stdlib.h>
#include <string.h>
#include "priority_queue.h"
//初始化
int z_pq_init(z_pq_t *z_pq, z_pq_comparator_pt comp, size_t capacity){
    // 为 指向元素的指针分配空间， pq 为指向指针的的指针， 若元素为结构体， 直接传入结构体地址即可
    z_pq->pq = (void**)malloc(sizeof(void*) * (capacity+1));
    if(!z_pq->pq)
        return -1;
    z_pq->size = 0;
    //默认从第二个位置存第一个元素， 便于操作
    z_pq->capacity= capacity+1;
    z_pq->comp = comp;
    return 0;
}

//通过size的值判断是否为空
int z_pq_is_empty(z_pq_t *z_pq){
    return (z_pq->size==0) ? 1 : 0;
}
//获得队列大小
int z_pq_size(z_pq_t *z_pq){
    return z_pq->size;
}
//交换元素
void exch(z_pq_t* z_pq, size_t i, size_t j){
    void *tmp = z_pq->pq[i];
    z_pq->pq[i] = z_pq->pq[j];
    z_pq->pq[j] = tmp;
}
//将小于父亲结点的 结点上移
void shiftup(z_pq_t* z_pq, size_t k){
    while(k > 1 && z_pq->comp(z_pq->pq[k], z_pq->pq[k/2])) {
        exch (z_pq, k, k/2);
        k/=2;
    }
}

//大于孩子结点的元素向下移动
int shiftdown(z_pq_t *z_pq, size_t k){
    size_t j;
    size_t size = z_pq->size;
    while((k<<1) <= size){              // 孩子结点若存在必然有左孩子
        j = k<<1;
        if((j < size) && (z_pq->comp(z_pq->pq[j+1],z_pq->pq[j])))    // 若有孩子存在， 比较左右孩子大小， 和较小的元素交换
            j++;
        if(!z_pq->comp(z_pq->pq[j],z_pq->pq[k]))                    // 若孩子结点比父亲结点小， 不交换
            break;
        exch(z_pq, j ,k);                                           //
        k = j;                                                      //父亲结点等于被交换的那个结点 ， 开始新的循环
    }
    return k;
}

void z_pq_shifup(z_pq_t* z_pq, size_t k){
    return shiftup(z_pq,k);
}

void* z_pq_min(z_pq_t* z_pq){
    if(z_pq_is_empty(z_pq))
        return (void*)(-1);
    return z_pq->pq[1];
}
//扩容
size_t resize(z_pq_t* z_pq, size_t new_capacity){
    if(new_capacity <= z_pq->size)
        return -1;
    void **new_ptr = (void**)malloc(sizeof(void*)* new_capacity);
    if(!new_ptr)
        return -1;
    memcpy(new_ptr,z_pq->pq, sizeof(void*) * (z_pq->size + 1));
    free(z_pq->pq);
    z_pq->pq = new_ptr;
    z_pq->capacity = new_capacity;
    return 0;
}


//删除最小元素
int z_pq_delimin(z_pq_t *z_pq){
    if(z_pq_is_empty(z_pq))
        return 0;
    exch(z_pq, 1, z_pq->size);          // 把第一个元素与最后一个交换， size --
    --z_pq->size;
    shiftdown(z_pq,1);                  // 对第一个元素进行shiftdown 操作，保证最小堆性质
    if((z_pq->size> 0)&& (z_pq->size <=(z_pq->capacity-1)/4))  // 若元素个数小于空间的1/4， 进行缩容至原本大小的的1/2；
    {
        if(resize(z_pq, z_pq->size/2)< 0)
            return  -1;
    }
    return 0;
}

int z_pa_insert(z_pq_t *z_pq, void *item){
    if(z_pq->size+1 == z_pq->capacity){
        if(resize(z_pq,z_pq->size*2) < 0)
            return -1;                         //空间不足时，空间扩展到之前的2倍
    }
    z_pq->pq[++z_pq->size] = item;
    z_pq_shifup(z_pq,z_pq->size);
    return 0;
}