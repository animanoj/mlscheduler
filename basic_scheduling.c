/**
 * @author Anirudh Manoj
 * A basic scheduler that takes a file of names and priorities and
 * sorts them using bubble sort according to priorities.
 *
 * The tasks are printed to standard out in order of execution
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

void* copy_int(void* elem) {
        int* x = malloc(sizeof(int));
        *x = *((int*)elem);
        return x;
}

void* copy_char(void* elem) {
        char* string = malloc(strlen((char*)elem) + 1);
        *string= '\0';
        strcpy(string, elem);
        return string;
}

void free_int(void* elem) {
        free(elem);
        elem = NULL;
}

void free_char(void* elem) {
        free(elem);
        elem = NULL;
}

int main(int argc, char* argv[]) {
        if(argc < 2) {
                fprintf(stderr, "Usage: ./basic <file name>\n");
                return 0;
        }
        FILE* file = fopen(argv[1], "r");
        if(!file) {
                fprintf(stderr, "Error with file name\n");
                return 0;
        }
        Vector* names = Vector_create(copy_char, free_char);
        Vector* priorities = Vector_create(copy_int, free_int);
        char* line = NULL;
        size_t buffer = 0;
        ssize_t bytesread;
        do {
                bytesread = getline(&line, &buffer, file);
                if(bytesread == -1)
                        continue;
                line[strlen(line) - 1] = '\0';
                char* line_temp = line;
                while(*line_temp != ' ')
                        line_temp++;
                char* username = malloc(strlen(line) - strlen(line_temp) + 1);
                strncpy(username, line, strlen(line) - strlen(line_temp));
                username[strlen(line) - strlen(line_temp)] = '\0';
                line_temp++;
                int* priority = malloc(sizeof(int));
                *priority = atoi(line_temp);
                Vector_append(names, (void*)username);
                Vector_append(priorities, (void*)priority);
                free(line);
                line = NULL;
                line_temp = NULL;
                buffer = 0;
                free(username);
                username = NULL;
                free(priority);
                priority = NULL;
        } while(bytesread != -1);
        free(line);
        line = NULL;
        buffer = 0;
        fclose(file);
        int swapped = 1, j = 0;
        int *int_1, *int_2, *temp_int;
        char *char_1, *char_2, *temp_char;
        while(swapped) {
                swapped = 0;
                j++;
                for(int i = 0; i < (int)Vector_size(priorities) - j; i++) {
                        if(*(int*)Vector_get(priorities, i) > *(int*)Vector_get(priorities, i + 1)) {
                                Vector_swap(priorities, i, i + 1);
                                Vector_swap(names, i, i + 1);
                                swapped = 1;
                        }
                }
        }
        int_1 = NULL;
        int_2 = NULL;
        temp_int = NULL;
        char_1 = NULL;
        char_2 = NULL;
        temp_char = NULL;
        for(int i = 0; i < (int)Vector_size(names); i++) {
                printf("%s %d\n", (char*)Vector_get(names, i), *(int*)Vector_get(priorities, i));
        }
        Vector_destroy(names);
        Vector_destroy(priorities);
        return 1;
}
