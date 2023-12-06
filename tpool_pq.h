//
// Created by nandhan on 8/10/23.
//

#ifndef THREAD_POOL_TPOOL_PQ_H
#define THREAD_POOL_TPOOL_PQ_H

#endif //THREAD_POOL_TPOOL_PQ_H

#include <pthread.h>

#ifndef TASK_QUEUE_TYPES

#define TASK_QUEUE_TYPES 1

typedef enum priority {HIGH = 1, MEDIUM = 3, LOW = 5} priority_t;

typedef struct task {
    void (*function)(int);
    int arguments;
    int deadline; // higher deadline number -> lower priority. Lower deadline number -> higher priority.
} task_t;

typedef struct tpool_pq {
    task_t *task_array;
    int n_tasks; // number of tasks currently in the queue.
    int queue_size; // size of task array
} tpool_pq_t;

#endif

// functions for deadline queue
int init_tpool_pq(tpool_pq_t **pq, int queue_size, task_t *task_list);

//int init_tpool_pq_list(tpool_pq_t *pq, int queue_size, tast_t *task_list);

int insert_task(tpool_pq_t **pq,task_t task);

task_t get_task(tpool_pq_t **tpool);

int get_num_tasks(const tpool_pq_t *tpool);

void print_tasks(tpool_pq_t *tpool_pq);
