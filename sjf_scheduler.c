#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vector.h"
#include "pqueue.h"

typedef struct _job_t job_t;
typedef struct _meta_t meta_t;

struct _job_t {
        int id;
        int priority;
        int arrival_time;
        int running_time;
};

struct _meta_t {
        char* name;
        int running_time;
};

void *meta_ctor(void *elem) {
        meta_t* process = malloc(sizeof(meta_t));
        meta_t* elem_m = (meta_t*)elem;
        process->name = strdup(elem_m->name);
        process->running_time = elem_m->running_time;
        return process;
}

void meta_dtor(void *elem) {
        meta_t* elem_m = (meta_t*)elem;
        free(elem_m->name);
        elem_m->name = NULL;
        free(elem_m);
        elem_m = NULL;
        elem = NULL;
}

Vector* map_parser() {
        Vector* map = Vector_create(meta_ctor, meta_dtor);
        FILE* file = fopen("time_data.txt", "r");
        if(file) {
                char* line = NULL;
                size_t buffer = 0;
                ssize_t bytesread;
                do {
                        bytesread = getline(&line, &buffer, stdin);
                        if(bytesread == -1)
                                continue;
                        meta_t* process = malloc(sizeof(meta_t));
                        process->name = malloc(bytesread);
                        char* name_temp = process->name;
                        char* run_time = NULL;
                        char* run_time_temp = NULL;
                        char* line_temp = line;
                        int input = 0;
                        while(input < 2) {
                                if(*line_temp == ' ' || *line_temp == '\n') {
                                        line_temp++;
                                        input++;
                                        run_time = malloc(strlen(line_temp));
                                        run_time_temp = run_time;
                                }
                                switch(input) {
                                case 0:  *name_temp = *line_temp;
                                        name_temp++;
                                        break;
                                case 1:  *run_time_temp = *line_temp;
                                        run_time_temp++;
                                        break;
                                default: break;
                                }
                                line_temp++;
                        }
                        *run_time_temp = '\0';
                        run_time_temp = NULL;
                        *name_temp = '\0';
                        name_temp = NULL;
                        line_temp = NULL;
                        process->running_time = atoi(run_time);
                        free(run_time);
                        run_time = NULL;
                        process->name = realloc(process->name, strlen(process->name) + 1);
                        Vector_append(map, process);
                        meta_dtor(process);
                        process = NULL;
                } while(bytesread != -1);
                fclose(file);
        }
        file = NULL;
        return map;
}

int comparer_sjf(const void *a, const void *b) {
        job_t* a_job = (job_t*) a;
        job_t* b_job = (job_t*) b;
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

void scheduler_sjf(char* filename) {

}

int main(int argc, char** argv) {
        return 0;
}
