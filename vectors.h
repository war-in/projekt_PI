#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "vector.h"

struct Vector{
    char* data;
    size_t size;
    size_t capacity;
};

Vector* constructVector(void){              // OK
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
    return vec;
}

void destruct(Vector* v){               // 26 linijka (nie pewne)
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

//printchar(Vector* v){
//for(size_t i = 0; i < v->size; i++){
//printf("%d ", v->data[i]);
//}
//printf("\n");
//}

Vector* cloneVector(const Vector* v){       // OK
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    vec->size = v->size;
    vec->capacity = v->size;
    if(v->size == 0) vec->data = NULL;
    else {
        vec->data = (char *)malloc(v->size * sizeof(char));
        for (size_t i = 0; i < vec->size; i++) {
            vec->data[i] = v->data[i];
        }
    }
    return vec;
}

size_t size(const Vector* v){           // OK
    return v->size;
}

void resize(Vector* v, size_t newSize){             // OK
    v->data = (char*)realloc(v->data, sizeof(char)*newSize);
    for(size_t i = v->size; i < newSize; i++){
        v->data[i] = 0;
    }
    v->size = newSize;
}

size_t capacity(const Vector* v){           // OK
    return v->capacity;
}

bool empty(const Vector* v){                // OK
    return (v->size == 0) ? 1 : 0;
}

bool reserve(Vector* v, size_t size){               // OK
    if(size > v->capacity) {
        v->data = (char*)realloc(v->data, size);
        v->capacity = size;
        return 1;
    }
    else return 0;
}

void push_back(Vector* v, char element){               // OK
    if(v->size >= v->capacity) reserve(v, v->size+1);
    v->data[v->size] = element;
    v->size += 1;
}

void pop_back(Vector* v){               // OK
    if(v->size == 0) return;
    char* ptr = v->data;
    for(size_t i = 0; i < v->size - 1; i++){
        ++ptr;
    }
    *ptr = 0;
    v->size -= 1;
}


char* insert(Vector* v, char element, size_t index){              // OK
    push_back(v, v->data[v->size - 1]);
    for (size_t i = v->size - 1; i > index; i--){
        v->data[i] = v->data[i - 1];
    }
    v->data[index] = element;
    return &(v->data[index]);
}



