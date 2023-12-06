//
// Created by nandhan on 8/10/23.
//

#include "tpool_pq.h"
#include <stdlib.h>
#include <stdio.h>


/* TODO: add logging. */


void swap(task_t **tasks, int i, int j) {
    task_t temp = *(*(tasks+i));
    (*(tasks+i)) = (*(tasks+j));
    *(*(tasks+j)) = temp;
}

/* functions to maintain heap property*/
void heapify_down(task_t **tasks, int heap_size, int i ) {
    int left = 2* i;
    int right = left + 1;
    int largest = i;

    task_t *t = *tasks;
    (t+left)->deadline = 9;
    /* compare priorities of children and swap*/
    if (left <= heap_size && ((*(tasks) + left)->deadline < (*(tasks) + i)->deadline) )
        largest = left;

    if (right <= heap_size && ((*(tasks)+right)->deadline >= (*(tasks) + largest)->deadline) )
        largest = right;

    //if (left <= heap_size && (((tasks+left))->deadline < ((tasks+i))->deadline))
    //    largest = left;

    //if (right <= heap_size && (((tasks+right))->deadline < ((tasks+largest))->deadline))
    //    largest = right;

    if (largest != i && largest < heap_size){
        swap(tasks, i, largest);
        heapify_down(tasks, heap_size, largest);
    }
}

void build_heap(task_t **tasks, int n_tasks) {
    int start = 1 + (n_tasks)/2;

    for (int i = start; i > 0; i--) {
        int current = i;
        int left = 2*current;
        int right = left+1;
        int largest = current;

        // loop while current tasks' deadline is less than left's or right child's deadline
        while( current < n_tasks && left < n_tasks && right <= n_tasks
             && (
                    ((*tasks+current)->deadline < (*tasks + left)->deadline)
                    || ((*tasks+current)->deadline < (*tasks + right)->deadline)
                )
        ) {

            // get the largest one
            if (left < n_tasks && (*(tasks) + left)->deadline > (*(tasks) + largest)->deadline)
                largest = left;
            if (right <= n_tasks && ((*tasks) + right)->deadline > ((*tasks) + largest)->deadline)
                largest = right;
            // if largest is not current, swap and level down.
            if (largest != current) {
                // swap current and largest
                task_t temp = *(*tasks + left);
                *((*tasks) + left) = *(*tasks + right);
                *(*tasks + right) = temp;
                // compute new current and left and right.
                current = largest;
                left = current*2;
                right = left +1;
            } else break;
        }
    }
}

void heapify_up(task_t **tasks, int heap_size, int i) {
    int parent = (int) i/2;
    int largest = parent;

    if (parent <= heap_size && i <= heap_size
        && ((*(tasks)+i))->deadline < ((*(tasks) + parent))->deadline)
        largest = i;

    if (largest != parent) {
        swap(tasks, parent, largest);
        /* heapify up if largest is not root*/
        if (largest > 1)
            heapify_up(tasks, heap_size, largest);
    }
}

void rec_build_heap(task_t **tasks, int n_tasks){
    int start = 1 + (int) (n_tasks)/2;

    for (int i = start; i > 0; i--) {
        heapify_down(tasks, n_tasks, i);
    }
}

int init_tpool_pq(tpool_pq_t **tpool_pqs, int queue_size, task_t *task_list) {
    // number of tasks in the task_list will be the size of queue. for sure.
    tpool_pq_t *tpool_pq;
    if ((tpool_pq = (tpool_pq_t *) malloc(sizeof(struct tpool_pq))) == NULL)
        printf("malloc returned null\n");

    tpool_pq->n_tasks = 0;
    tpool_pq->queue_size = queue_size;
    /* Allocate size for n+1 elements. 0th element is empty */
    tpool_pq->task_array = (task_t *)malloc(sizeof(task_t)*(queue_size+1));
    // insert at root

    if (task_list != NULL){ /* TODO: check logic*/
        for (int i = 1; i <= queue_size; i++){
            *(tpool_pq->task_array + i) = *(task_list+(i-1));
        }
        // set n_tasks to queue_size
        tpool_pq->n_tasks = queue_size;
    }

    /* call build heap only if some tasks available */
    if (tpool_pq->n_tasks > 0)
        build_heap(&tpool_pq->task_array, tpool_pq->n_tasks); // TODO: UNDO THIS.

    // print the tasks to see the order
    printf("printing the tasks after building the heap\n");

    for (int i = 1; i <=queue_size; i++) {
        printf("Task at %d has deadline %d \n",i, (tpool_pq->task_array+i)->deadline);
    }

    *tpool_pqs = tpool_pq;
    // free memory
    free(tpool_pq);
    return 1;
}


int insert_task(tpool_pq_t **tpool_queue, task_t task) {

    if ((*tpool_queue)->n_tasks < (*tpool_queue)->queue_size) {
        // insert task into queue
        (*tpool_queue)->task_array[(*tpool_queue)->n_tasks++ ] = task;
        // TODO: restore heap property.
        int last_inserted = (*tpool_queue)->n_tasks - 1;
        heapify_up(&(*tpool_queue)->task_array, (*tpool_queue)->queue_size, last_inserted);
        return 1;
    }
    /* insert not successful. */
    return -1;
}

task_t get_task(tpool_pq_t **tpool_queue) {

    // swap root task with last task.
    // perform iterative heapify down
    // return last task

    int queue_size = (*tpool_queue)->queue_size;
    int n_tasks = (*tpool_queue)->n_tasks;

    int root = 1;
    int swap_index = n_tasks;// TODO VERIFY THIS.
    // swap first and last task;
    task_t temp = *((*tpool_queue)->task_array + root);
    *((*tpool_queue)->task_array+ swap_index) = *((*tpool_queue)->task_array + root);
    *((*tpool_queue)->task_array + root) = temp;

    // restore heapify property.

    // decrease n_tasks.
    (*tpool_queue)->n_tasks--;
    n_tasks--;

    int current = root;
    int left = root *2;
    int right = left + 1;
    int largest = current;

    while(
            left < n_tasks && right < n_tasks && (
                                                         (((*tpool_queue)->task_array + current)->deadline < ((*tpool_queue)->task_array + left)->deadline ) ||
                                                         (((*tpool_queue)->task_array + current)->deadline < ((*tpool_queue)->task_array + right)->deadline )
            )
            ) {
        // get largest and swap current with largest.

        if (((*tpool_queue)->task_array + largest)->deadline < ((*tpool_queue)->task_array + left)->deadline ) {
            largest = left;
        }
        if (((*tpool_queue)->task_array + largest)->deadline < ((*tpool_queue)->task_array + right)->deadline ) {
            largest = right;
        }

        if (largest != current) {
            // swap current and largest
            //task_t temp = *(*tasks + left);
            //*((*tasks) + left) = *(*tasks + right);
            //*(*tasks + right) = temp;
            task_t temp2 = *((*tpool_queue)->task_array + current);
            *((*tpool_queue)->task_array+ current) = *((*tpool_queue)->task_array + largest);
            *((*tpool_queue)->task_array + largest) = temp2;
            // compute new current and left and right.
            current = largest;
            left = current*2;
            right = left +1;
        } else break;
    }

    return temp;
}

task_t get_task_rec(tpool_pq_t **tpool_queue) {
    printf("Getting tasks");
    task_t task;

    //int current_n_tasks = (*tpool_queue)->n_tasks;
    //task = *((*tpool_queue)->task_array+current_n_tasks);
    //(*tpool_queue)->n_tasks = current_n_tasks-1;

    int queue_size = (*tpool_queue)->queue_size;

    int root = 1;
    int last_inserted_index = (*tpool_queue)->n_tasks - 1;

    if ((*tpool_queue)->n_tasks < queue_size) {
        // insert task into queue
        task = (*tpool_queue)->task_array[root];

        // swap root task with last task.
        swap(&(*tpool_queue)->task_array, root, last_inserted_index);
        // decrease counter
        (*tpool_queue)->n_tasks--;

        // TODO: restore heap property.
        heapify_down(&(*tpool_queue)->task_array, (*tpool_queue)->n_tasks, root);
    }
    printf("Returning task, deadline:%d, arguments:%d ", task.deadline, task.arguments);
    return task;
}

int get_num_tasks(const tpool_pq_t *tpool_queue) {

    int n = tpool_queue->n_tasks;
    return n;
}

void print_tasks(tpool_pq_t *tpool_pq) {
    int n = tpool_pq->n_tasks;

    for (int i = 0; i < n; i++) {
        task_t t = tpool_pq->task_array[i];
        //(*t.function)(t.arguments);
        //printf("Task, deadline: %d\n", t.deadline);

        //printf("Task: %d, deadline: %d\n", i, tpool_pq->task_array[i].deadline);
    }
}