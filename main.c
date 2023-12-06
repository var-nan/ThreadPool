
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tpool_pq.h"
#include "thread_pool.h"


int fibonacci(int x) {

    //sleep(3);
    if (x == 1 || x == 2) {
        return 1;
    } else
        return fibonacci(x-1) + fibonacci(x-2);
}

void function(int i) {

    sleep(2);
    int val = i*i;
    printf("Square of %d is %d by thread %d\n",i, val, gettid());
    //printf("Printing task %d\n", i);
}

int main() {

    int queue_size = 10;
    int n_threads = 3;
    int n_tasks = 6;

    /* initialize thread pool. */
    // task_t *tasks = (task_t *) malloc(sizeof(task_t)*256);

    // why breakpoints are not working?
    printf("Starting thread Pool.\n");
    thread_pool_t *tpool;

    task_t *list = malloc(sizeof(task_t)*queue_size);

    printf("Queue before heap\n");
    for (int i = 0; i < (queue_size); i++) {
        task_t task = {function, (i+1) , 100-(i)};
        *(list+i) = task;
        printf("Task %d, deadline %d, added to queue\n",task.arguments, task.deadline);
        //task_t task2 = {function, i, 5};
        //*(list+i+1) = task2;
    }



    //printf("initializing thread Pool. \n");
    thread_pool_init_list(&tpool, n_threads, queue_size, list);

    /* submit tasks to pool. */

    sleep(5);
    /*
    for (int i =0; i < n_tasks; i++) {
        task_t task = {function, i, MEDIUM};

        if (!thread_pool_submit_task(&tpool, task))
            printf("Not submitted task: %d\n", i);

        task_t task2 = {function, i, HIGH};
        if (!thread_pool_submit_task(&tpool, task2))
            printf("Not submitted task: %d\n", i);

    }
    */


    /* shutdown thread pool. */
    thread_pool_shutdown(&tpool, 1);
    printf("shutdown complete...\n");

    free(list);
    return 0;
}


void temp() {
    /*
    tpool_pq_t *pq;
    int task_queue_size = 8;
    init_tpool_pq(&pq, task_queue_size);
    printf("size: %d \n", pq->queue_size);

    for (int i = 0; i < (int)task_queue_size/2; i++) {
        task_t task = {(void * (*)(void *))&function, (void *) &i, MEDIUM};
        int status;
        if ((status = insert_task(&pq,task))!= 1)
            printf("not submitted task %d\n",i);
    }


    for (int i = (int)task_queue_size/2; i< task_queue_size; i++ ) {
        task_t task = {(void * (*)(void *))&function, (void *) &i, HIGH};
        int status;
        if ((status = insert_task(&pq,task))!= 1)
            printf("not submitted task %d\n",i);
    }

    printf("printing tasks in the list..\n");
    print_tasks(pq);
    */
    /* should expect the high deadline functions become first. */

}
