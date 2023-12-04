/*
 * Replace the following string of 0s with your student number
 * 000000000
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "job.h"
#include "errno.h"
#include "test/munit.h"

/* 
 * DO NOT EDIT the job_new function.
 */
job_t *job_new(pid_t pid, unsigned int id, unsigned int priority,
               const char *label) {
    return job_set((job_t *) malloc(sizeof(job_t)), pid, id, priority, label);
}

/* 
 * TODO: you must implement this function
 */
job_t *job_copy(job_t *src, job_t *dst) {
    if (src == NULL) {
        return NULL;
    }
    if (strlen(src->label) != MAX_NAME_SIZE - 1) {
        errno = MAX_NAME_SIZE;
        return NULL;
    }
    if (job_is_equal(src, dst)) {
        return dst;
    }
    if (dst == NULL) {
        dst = job_new(src->pid, src->id, src->priority, src->label);
        return dst;
    }
    dst = job_set(dst, src->pid, src->id, src->priority, src->label);
    return dst;
}

/* 
 * TODO: you must implement this function
 */
void job_init(job_t *job) {
    if (job != NULL) {
        job_set(job, 0, 0, 0, NULL);
    }
}

/* 
 * TODO: you must implement this function
 */
bool job_is_equal(job_t *j1, job_t *j2) {
    if ((j1 == NULL && j2 == NULL) || j1 == j2) {
        return true;
    } else if (j1 != NULL && j2 != NULL) {
        return j1->pid == j2->pid && j1->id == j2->id && j1->priority == j2->priority &&
               strcmp(j1->label, j2->label) == 0;
    }
    return false;
}

/*
 * TODO: you must implement this function.
 * Hint:
 * - read the information in job.h about padding and truncation of job
 *      labels
 */
job_t *job_set(job_t *job, pid_t pid, unsigned int id, unsigned int priority,
               const char *label) {

    if (job == NULL) {
        return NULL;
    }

    job->pid = pid;
    job->id = id;
    job->priority = priority;

    if (label == NULL || strlen(label) == 0) {
        strncpy(job->label, PAD_STRING, MAX_NAME_SIZE);
    } else {
        strncpy(job->label, label, MAX_NAME_SIZE - 1);
        if (strlen(job->label) < MAX_NAME_SIZE - 1) {
            strncat(job->label, PAD_STRING, MAX_NAME_SIZE - strlen(job->label) - 1);
        }
        job->label[MAX_NAME_SIZE - 1] = '\0';
    }

    return job;
}

/*
 * TODO: you must implement this function.
 * Hint:
 * - see malloc and calloc system library functions for dynamic allocation, 
 *      and the documentation in job.h for when to do dynamic allocation
 */
char *job_to_str(job_t *job, char *str) {
    if (job == NULL || strlen(job->label) != MAX_NAME_SIZE - 1) {
        return NULL;
    }
    if (str == NULL) {
        str = calloc(JOB_STR_SIZE, sizeof(char));
        if (str == NULL) {
            return NULL;
        }
    }
    (void) snprintf(str, JOB_STR_SIZE, JOB_STR_FMT, job->pid, job->id, job->priority, job->label);
    return str;
}

int char_index(char c, char *string) {
    for (int i = 0; string[i] != '\0'; i++)
        if (string[i] == c)
            return i;

    return -1;

}

/*
 * TODO: you must implement this function.
 * Hint:
 * - see the hint for job_to_str
 */
job_t *str_to_job(char *str, job_t *job) {
    int pid;
    unsigned int id;
    unsigned int priority;
    char label[MAX_NAME_SIZE];

    if (str == NULL || strlen(str) != JOB_STR_SIZE - 1) {
        return NULL;
    }
    int res = sscanf(str, JOB_STR_FMT, &pid, &id, &priority, label);

    if (res != 4) {
        return NULL;
    }
    int star_index = char_index('*', label);

    if (star_index != -1) {
        label[star_index] = '\0';
    }
    if (job == NULL) {
        job = job_new(pid, id, priority, label);
    } else {
        job = job_set(job, pid, id, priority, label);
    }

    return job;
}

/* 
 * TODO: you must implement this function
 * Hint:
 * - look at the allocation of a job in job_new
 */
void job_delete(job_t *job) {
    return;
}