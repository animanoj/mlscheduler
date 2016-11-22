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
        process->name = strdup(elem_m->name);
        process->running_time = elem_m->running_time;
        process->arrival_time = elem_m->arrival_time;
        return process;
}

void meta_dtor(void *elem) {
        meta_t* elem_m = (meta_t*)elem;
        free(elem_m->command);
        elem_m->command = NULL;
        free(elem_m->name);
        elem_m->name = NULL;
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

char **strsplit(const char *str, const char *delim, size_t *numtokens) {
        // copy the original string so that we don't overwrite parts of it
        // (don't do this if you don't need to keep the old line,
        // as this is less efficient)
        char *s = strdup(str);
        // these three variables are part of a very common idiom to
        // implement a dynamically-growing array
        size_t tokens_alloc = 1;
        size_t tokens_used = 0;
        char **tokens = calloc(tokens_alloc, sizeof(char *));
        char *token, *strtok_ctx;
        for (token = strtok_r(s, delim, &strtok_ctx); token != NULL;
             token = strtok_r(NULL, delim, &strtok_ctx)) {
                // check if we need to allocate more space for tokens
                if (tokens_used == tokens_alloc) {
                        tokens_alloc *= 2;
                        tokens = realloc(tokens, tokens_alloc * sizeof(char *));
                }
                tokens[tokens_used++] = strdup(token);
        }
        // cleanup
        if (tokens_used == 0) {
                free(tokens);
                tokens = NULL;
        } else {
                tokens = realloc(tokens, tokens_used * sizeof(char *));
        }
        *numtokens = tokens_used;
        free(s);
        // Adding a null terminator
        tokens = realloc(tokens, sizeof(char *) * (tokens_used + 1));
        tokens[tokens_used] = NULL;
        return tokens;
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

void Vector_to_file(Vector* map) {
        FILE* file = fopen("time_data.txt", "w");
        if(!file) {
                fprintf(stderr, "File to write failed\n");
                return;
        }
        for(int i = 0; i < (int)Vector_size(map); i++)
        {
                meta_t* line = Vector_get(map, i);
                for(int j = 0; j < (int)strlen(line->name); j++) {
                        fprintf(file, "%c", line->name[j]);
                }
                fprintf(file, "%c", ' ');
                fprintf(file, "%f", line->running_time);
                fprintf(file, "%c", '\n');
        }
        fclose(file);
}
