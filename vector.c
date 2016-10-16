/**
 * Machine Problem: Vector
 * CS 241 - Fall 2016
 */

/* An automatically-expanding array of strings. */
#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

/*
 * Vector structure
 * Do not modify the structure
 * array: Void pointer to the beginning of an array of void pointers to
 * arbitrary data.
 * size: The number of elements in the vector. This is the number of actual
 * objects held in the vector, which is not necessarily equal to its capacity.
 * capacity: The size of the storage space currently allocated for the vector,
 * expressed in terms of elements.
 * copyt_constructor: the function callback for you to define the way you want
 * to copy elements
 * destructor:  the function callback for you to define the way you want to
 * destroy elements
 */
struct Vector {
        copy_constructor_type copy_constructor;
        destructor_type destructor;

        void **array;
        size_t size;
        size_t capacity;
};

Vector *Vector_create(copy_constructor_type copy_constructor,
                      destructor_type destructor) {
        // your code here
        Vector* vec = malloc(sizeof(Vector));
        vec->size = 0;
        vec->capacity = 10;
        vec->array = calloc(INITIAL_CAPACITY, sizeof(void*));
        vec->copy_constructor = copy_constructor;
        vec->destructor = destructor;
        return vec;
}

void Vector_destroy(Vector *vector) {
        assert(vector);
        // your code here
        int i;
        for(i = 0; i < (int)Vector_size(vector); i++) {
                vector->destructor(vector->array[i]);
                vector->array[i] = NULL;
        }
        free(vector->array);
        free(vector);
}

size_t Vector_size(Vector *vector) {
        assert(vector);
        // your code here
        return vector->size;
}

size_t Vector_capacity(Vector *vector) {
        assert(vector);
        // your code here
        return vector->capacity;
}

void Vector_resize(Vector *vector, size_t new_size) {
        assert(vector);
        assert(new_size >= 0);
        // your code here
        int i = 0;
        if(vector->size < new_size)
        {
                i = vector->size;
                if(vector->capacity < new_size)
                {
                        while(vector->capacity < new_size)
                        {
                                vector->capacity *= 2;
                                vector->array = realloc(vector->array, vector->capacity * sizeof(void*));
                        }
                }
                while(i < (int)new_size)
                        vector->array[i++] = NULL;

        }
        else if(vector->size > new_size)
        {
                if(new_size <= (vector->capacity/4))
                {
                        while((vector->capacity > (2 * new_size)) && (vector->capacity > 10))
                        {
                                i = vector->capacity - 1;
                                vector->capacity /= 2;
                                for(; i >= (int)vector->capacity; i--) {
                                        vector->destructor(vector->array[i]);
                                        vector->array[i] = NULL;
                                }
                                vector->array = realloc(vector->array, vector->capacity);
                        }
                }
                if(vector->capacity > vector->size)
                {
                        for(i = vector->size - 1; i > (int)new_size; i--) {
                                vector->destructor(vector->array[i]);
                                vector->array[i] = NULL;
                        }
                }
        }
        vector->size = new_size;
}

void Vector_set(Vector *vector, size_t index, void *elem) {
        assert(vector);
        assert(index < vector->size && index >= 0);
        // your code here
        if(elem == NULL)
                vector->array[index] = NULL;
        else {
                vector->array[index] = vector->copy_constructor(elem);
        }
}

void *Vector_get(Vector *vector, size_t index) {
        assert(vector);
        assert(index < vector->size && index >= 0);
        // your code here
        if(vector->array[index] == NULL)
                return NULL;
        else
                return vector->array[index];
}

void Vector_insert(Vector *vector, size_t index, void *elem) {
        assert(vector);
        assert(index >= 0);
        // your code here
        if(index >= vector->size)
        {
                Vector_resize(vector, index + 1);
                Vector_set(vector, index, elem);
        }
        else if(vector->array[index] != NULL)
        {
                if(vector->size == vector->capacity)
                        Vector_resize(vector, vector->size + 1);
                for(int i = vector->size - 1; i > (int)index; i--)
                        vector->array[i] = vector->array[i - 1];
                Vector_set(vector, index, elem);
        }
        else
                Vector_set(vector, index, elem);
}

void Vector_delete(Vector *vector, size_t index) {
        assert(vector);
        assert(index < vector->size && index >= 0);
        // your code here, what asserts might you want?
        if(Vector_get(vector, index))
        {
                vector->destructor(vector->array[index]);
                vector->array[index] = NULL;
        }
        if(index < (vector->size - 1))
        {
                for(int i = index; i < (int)(vector->size - 1); i++)
                        vector->array[i] = vector->array[i + 1];
                vector->array[Vector_size(vector) - 1] = NULL;
        }
        Vector_resize(vector, vector->size - 1);
}

void Vector_append(Vector *vector, void *elem) {
        assert(vector);
        // your code here
        Vector_insert(vector, vector->size, elem);
}

void Vector_swap(Vector *vector, size_t index_1, size_t index_2) {
        void* temp = vector->array[index_1];
        vector->array[index_1] = vector->array[index_2];
        vector->array[index_2] = temp;
}
