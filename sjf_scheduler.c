#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "vector.h"
#include "pqueue.h"
#include "utils.h"

priqueue_t pqueue;
Vector* map;
int running = 1;
int jobs = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* scheduler_sjf(void* arg) {
        (void) arg;
        while(1) {
                usleep(1000);
                pthread_mutex_lock(&m);
                if(jobs == 0 && running) {
                        pthread_mutex_unlock(&m);
                        continue;
                }
                else if(jobs)
                        jobs--;
                else {
                        pthread_mutex_unlock(&m);
                        return NULL;
                }
                meta_t* process = (meta_t*)priqueue_poll(&pqueue);
                printf("taken : %s\n", process->command);
                pthread_mutex_unlock(&m);
                double time_process = getTime();
                system(process->command);
                fprintf(stderr, "done: %s\n", process->command);
                time_process = getTime() - time_process;
                if(process->running_time != (double)-1) {
                        for(int i = 0; i < (int)Vector_size(map); i++) {
                                meta_t* meta = Vector_get(map, i);
                                if(strcmp(meta->command, process->command) == 0) {
                                        pthread_mutex_lock(&m);
                                        meta->running_time = (meta->running_time + time_process) / 2;
                                        pthread_mutex_unlock(&m);
                                        break;
                                }
                        }
                }
                else {
                        process->running_time = time_process;
                        pthread_mutex_lock(&m);
                        Vector_append(map, process);
                        pthread_mutex_unlock(&m);
                }
                meta_dtor(process);
                process = NULL;
        }
        return NULL;
}

int main(int argc, char** argv) {
        if(argc < 3) {
                fprintf(stderr, "Usage: ./scheduler <file name> <thread count>\n");
                return -1;
        }
        priqueue_init(&pqueue);
        FILE* file = fopen(argv[1], "r");
        if(!file) {
                fprintf(stderr, "File not found\n");
                return -1;
        }
        map = map_parser();
        pthread_t threads[atoi(argv[2])];
        for(int i = 0; i < atoi(argv[2]); i++)
                pthread_create(&threads[i], NULL, scheduler_sjf, NULL);
        char* line = NULL;
        size_t buffer = 0;
        ssize_t bytesread;
        double total_time = getTime();
        do {
                bytesread = getline(&line, &buffer, file);
                if(bytesread == -1)
                        continue;
                if(line[strlen(line) - 1] == '\n') {
                        line[strlen(line) - 1] = '\0';
                }
                meta_t* process = malloc(sizeof(meta_t));
                process->command = line;
                process->arrival_time = getTime();
                int run_time = -1;
                for(int i = 0; i < (int)Vector_size(map); i++) {
                        meta_t* meta = Vector_get(map, i);
                        if(strcmp(meta->command, process->command) == 0) {
                                run_time = meta->running_time;
                                break;
                        }
                }
                process->running_time = run_time;
                pthread_mutex_lock(&m);
                priqueue_offer(&pqueue, (void*)process);
                pthread_mutex_unlock(&m);
                line = NULL;
                buffer = 0;
                pthread_mutex_lock(&m);
                jobs++;
                pthread_mutex_unlock(&m);
        } while(bytesread != -1);
        free(line);
        line = NULL;
        pthread_mutex_lock(&m);
        running = 0;
        pthread_mutex_unlock(&m);
        for(int i = 0; i < atoi(argv[2]); i++)
                pthread_join(threads[i], NULL);
        total_time = getTime() - total_time;
        Vector_to_file(map);
        Vector_destroy(map);
        // priqueue_destroy(&pqueue);
        fclose(file);
        printf("Time taken: %f\n", total_time);
        return 0;
}
