//
// Created by genius on 18-8-9.
//

#ifndef ZING_PRIORITY_QUEUE_H
#define ZING_PRIORITY_QUEUE_H

#include<stdlib.h>

#define Z_PQ_DEFAULT_SIZE 10

typedef int (*z_pq_comparator_pt)(void* l, void* r);

typedef struct priority_queue{
    void **pq;                      // 指向item的指针
    size_t size;                    // 元素个数
    size_t capacity;                // 容量大小
    z_pq_comparator_pt comp;        //比较函数
}z_pq_t;

int z_pq_init(z_pq_t *z_pq, z_pq_comparator_pt comp, size_t capacity);
int z_pq_is_empty(z_pq_t *z_pq);
int z_pa_insert(z_pq_t *z_pq, void *item);
int z_pq_delimin(z_pq_t *z_pq);
size_t resize(z_pq_t* z_pq, size_t new_capacity);
void* z_pq_min(z_pq_t* z_pq);
void z_pq_shifup(z_pq_t* z_pq, size_t k);
int z_pq_size(z_pq_t *z_pq);
#endif //ZING_PRIORITY_QUEUE_H
