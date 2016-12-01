#include "pqueue.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void priqueue_init(priqueue_t *q) {
        q = malloc(sizeof(priqueue_t));
        q->size = 0;
        q->head = NULL;
}

int priqueue_offer(priqueue_t *q, void *ptr) {
        entry* new_node = malloc(sizeof(entry));
        new_node->value = ptr;
        q->size++;
        if(q->size == 1) {
                new_node->next = NULL;
                q->head = new_node;
                return 0;
        }
        entry* node;
        int i = 0;
        for(node = q->head; node->next != NULL; node = node->next, i++) {
                if(comparer_sjf(node->next->value, ptr) == 1) {
                        new_node->next = node->next;
                        node->next = new_node;
                        return i;
                }
        }
        new_node->next = NULL;
        node->next = new_node;
        return (q->size - 1);
}

void* priqueue_peek(priqueue_t *q) {
        return q->head;
}

void* priqueue_poll(priqueue_t *q) {
        if(q->head == NULL)
                return NULL;
        entry* result = q->head;
        q->head = q->head->next;
        q->size--;
        meta_t* process = result->value;
        free(result);
        result = NULL;
        return process;
}

int priqueue_size(priqueue_t *q) {
        return q->size;
}

void priqueue_destroy(priqueue_t *q) {
        free(q);
        q = NULL;
}
