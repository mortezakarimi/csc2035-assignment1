/*
 * Replace the following string of 0s with your student number
 * 000000000
 */
#include <stdlib.h>
#include <stdbool.h>
#include "pri_jobqueue.h"
#include "test/munit.h"

/* 
 * TODO: you must implement this function that allocates a job queue and 
 * initialise it.
 * Hint:
 * - see job_new in job.c
 */
pri_jobqueue_t *pri_jobqueue_new() {
    pri_jobqueue_t *q = (pri_jobqueue_t *) malloc(sizeof(pri_jobqueue_t));
    pri_jobqueue_init(q);
    return q;
}

/* 
 * TODO: you must implement this function.
 */
void pri_jobqueue_init(pri_jobqueue_t *pjq) {
    pjq->buf_size = JOB_BUFFER_SIZE;
    pjq->size = 0;
    for (int i = 0; i < pjq->buf_size; ++i) {
        pjq->jobs[i] = *job_new(0, 0, 0, NULL);
    }
}

/* 
 * TODO: you must implement this function.
 * Hint:
 *      - if a queue is not empty, and the highest priority job is not in the 
 *      last used slot on the queue, dequeueing a job will result in the 
 *      jobs on the queue having to be re-arranged
 *      - remember that the job returned by this function is a copy of the job
 *      that was on the queue
 */
job_t *pri_jobqueue_dequeue(pri_jobqueue_t *pjq, job_t *dst) {
    if (pri_jobqueue_is_empty(pjq)) {
        return NULL;
    }
    dst = job_copy(&pjq->jobs[0], dst);

    for (int i = 0; i < pjq->size - 1; i++) {
        pjq->jobs[i] = pjq->jobs[i + 1];
    }
    pjq->jobs[--pjq->size] = *job_new(0, 0, 0, NULL);

    return dst;
}

void counting_sort_by_priority(job_t *arr, int size) {
    // Find the maximum priority in the jobs
    unsigned int maxPriority = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i].priority > maxPriority) {
            maxPriority = arr[i].priority;
        }
    }

    // Create a counting array to store count of jobs for each priority
    int count[maxPriority + 1];

    // Initialize count of jobs for each priority
    memset(count, 0, sizeof(count));
    for (int i = 0; i < size; i++) {
        count[arr[i].priority]++;
    }

    // Modify count array so that each element stores the sum of previous counts
    for (int i = 1; i <= maxPriority; i++) {
        count[i] += count[i - 1];
    }

    // Create an output array to store sorted jobs
    job_t output[size];

    // Fill the output array with jobs sorted by priority
    for (int i = size - 1; i >= 0; i--) {
        output[count[arr[i].priority] - 1] = arr[i];
        count[arr[i].priority]--;
    }

    // Copy the sorted jobs back to the original array
    for (int i = 0; i < size; i++) {
        arr[i] = output[i];
    }
}

/*
 * TODO: you must implement this function.
 * Hints:
 * - if a queue is not full, and if you decide to store the jobs in 
 *      priority order on the queue, enqueuing a job will result in the jobs 
 *      on the queue having to be re-arranged. However, it is not essential to
 *      store jobs in priority order (it simplifies implementation of dequeue
 *      at the expense of extra work in enqueue). It is your choice how 
 *      you implement dequeue (and enqueue) to ensure that jobs are dequeued
 *      by highest priority job first (see pri_jobqueue.h)
 * - remember that the job passed to this function is copied to the 
 *      queue
 */
void pri_jobqueue_enqueue(pri_jobqueue_t *pjq, job_t *job) {
    if (pri_jobqueue_is_full(pjq) || job == NULL) {
        return;
    }
    if (job->priority > 0) {
        job_copy(job, &pjq->jobs[pjq->size++]);
    }
    counting_sort_by_priority(pjq->jobs, pjq->size);
}

/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_empty(pri_jobqueue_t *pjq) {
    return pjq == NULL || pjq->size == 0;
}

/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_full(pri_jobqueue_t *pjq) {
    return pjq == NULL || pjq->buf_size == pjq->size;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 *      - remember that the job returned by this function is a copy of the 
 *      highest priority job on the queue.
 *      - both pri_jobqueue_peek and pri_jobqueue_dequeue require copying of 
 *      the highest priority job on the queue
 */
job_t *pri_jobqueue_peek(pri_jobqueue_t *pjq, job_t *dst) {
    if (pri_jobqueue_is_empty(pjq)) {
        return NULL;
    }
    dst = job_copy(&pjq->jobs[0], dst);
    return dst;
}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_size(pri_jobqueue_t *pjq) {
    return pjq == NULL ? 0 : pjq->size;
}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_space(pri_jobqueue_t *pjq) {
    return pjq == NULL ? 0 : pjq->buf_size - pjq->size;
}

/* 
 * TODO: you must implement this function.
 *  Hint:
 *      - see pri_jobqeue_new
 */
void pri_jobqueue_delete(pri_jobqueue_t *pjq) {
    if (pjq == NULL) {
        return;
    }
    for (int i = 0; i < pjq->buf_size; ++i) {
        job_delete(&pjq->jobs[i]);
    }
    free(pjq);
}
