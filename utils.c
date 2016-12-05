#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "utils.h"
#include "pqueue.h"

double getTime() {
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        return t.tv_sec + 1e-9 * t.tv_nsec;
}

void *meta_ctor(void *elem) {
        meta_t* process = malloc(sizeof(meta_t));
        meta_t* elem_m = (meta_t*)elem;
        process->command = strdup(elem_m->command);
        process->running_time = elem_m->running_time;
        process->arrival_time = elem_m->arrival_time;
        return process;
}

void meta_dtor(void *elem) {
        meta_t* elem_m = (meta_t*)elem;
        free(elem_m->command);
        elem_m->command = NULL;
        free(elem_m);
        elem_m = NULL;
        elem = NULL;
}


int comparer_sjf(const void *a, const void *b) {
        meta_t* a_job = (meta_t*) a;
        meta_t* b_job = (meta_t*) b;
        if(a_job->running_time < b_job->running_time)
                return -1;
        else if(a_job->running_time > b_job->running_time)
                return 1;
        else {
                if(a_job->arrival_time < b_job->arrival_time)
                        return -1;
                else if(a_job->arrival_time > b_job->arrival_time)
                        return 1;
                else
                        return 0;
        }
}

Vector* map_parser() {
        Vector* map = Vector_create(meta_ctor, meta_dtor);
        FILE* file = fopen("time_data.txt", "r");
        if(file) {
                char* line = NULL;
                size_t buffer = 0;
                ssize_t bytesread;
                do {
                        bytesread = getline(&line, &buffer, file);
                        if(bytesread == -1)
                                continue;
                        meta_t* process = malloc(sizeof(meta_t));
                        if(line[strlen(line) - 1] == '\n')
                          line[strlen(line) - 1] = '\0';
                        process->command = line;
                        line = NULL;
                        buffer = 0;
                        bytesread = getline(&line, &buffer, file);
                        if(line[strlen(line) - 1] == '\n')
                          line[strlen(line) - 1] = '\0';
                        process->running_time = atoi(line);
                        Vector_append(map, process);
                        meta_dtor(process);
                        process = NULL;
                        free(line);
                        line = NULL;
                        buffer = 0;
                } while(bytesread != -1);
                free(line);
                line = NULL;
                fclose(file);
        }
        file = NULL;
        return map;
}

void Vector_to_file(Vector* map) {
        FILE* file = fopen("time_data.txt", "w");
        if(!file) {
                fprintf(stderr, "File to write failed\n");
                return;
        }
        for(int i = 0; i < (int)Vector_size(map); i++)
        {
                meta_t* line = Vector_get(map, i);
                fprintf(file, "%s\n%f\n", line->command, line->running_time);
        }
        fclose(file);
}

void example_generator(long total_seconds, long upper_limit) {
  FILE* file = fopen("example.in", "w");
  if(!file) {
    fprintf(stderr, "Example generator file open failed\n");
    return;
  }
  time_t t;
  srand((unsigned) time(&t));
  while(total_seconds >= upper_limit) {
    int r_num = rand() % upper_limit;
    fprintf(file, "sleep %d\n", r_num);
    total_seconds -= r_num;
  }
}
