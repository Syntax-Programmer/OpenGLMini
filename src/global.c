#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/global.h"

int32_t g_AllocArray(Struct_Array *pArr, uint32_t capacity,
                  size_t elem_size) {
    pArr->arr = NULL;    
    g_FreeArray(pArr);
    pArr->arr = malloc(elem_size * capacity);
    if (!(pArr->arr)) {
        fprintf(stderr, "Couldn't allocate space for Struct_Array.\n");
        g_FreeArray(pArr);
        return 0;
    }
    pArr->capacity = capacity;
    pArr->elem_size = elem_size;

    return 1;
}

void g_FreeArray(Struct_Array *pArr) {
    if (pArr && pArr->arr) {
        free(pArr->arr);
        pArr->arr = NULL;
    }
    pArr->capacity = pArr->elem_size = pArr->size = 0;
}

int32_t g_ReallocArray(Struct_Array *pArr, uint32_t new_cap) {
    void *new_arr = realloc(pArr->arr, new_cap * pArr->elem_size);
    if (!new_arr) {
        fprintf(stderr, "Reallocation of array failed.\n");
        g_FreeArray(pArr);
        return 0;
    }

    pArr->arr = new_arr;
    pArr->capacity = new_cap;

    return 1;
}

void *g_GetArrayElem(Struct_Array *pArr, uint32_t index) {
    if (index >= pArr->capacity) {
        fprintf(stderr, "Trying to access memory outside the array's bounds.\n");
        return NULL;
    }

    return (void *)((char *)pArr->arr + (pArr->elem_size * index));
}

/* Use this to add to the array in whatever way you want.
but then you can't the first_empty_index field reliably. */
int32_t g_SetArrayElem(Struct_Array *pArr, uint32_t index, void *pElem,
                    size_t elem_size) {
    if (elem_size != pArr->elem_size) {
        fprintf(stderr, "Tried to set element of a size that doesn't match"
                " the size of an element in the array.\n");
        return 0;
    } else if (index >= pArr->capacity) {
        fprintf(stderr, "Trying to edit memory outside the array's bounds.\n");
        return 0;
    }

    memcpy((char*)pArr->arr + (index * pArr->elem_size), pElem, pArr->elem_size);

    return 1;
}

/* Use this to treat the array as a stack or have predicable filling.
This also auto adjusts the size of the array if its filled. */
int32_t g_AddArrayElem(Struct_Array *pArr,void *pElem, size_t elem_size) {
    if (pArr->size == pArr->capacity) { // Meaning the array is full, and the size has gone beyond the allowed indices.
        if (!g_ReallocArray(pArr, pArr->capacity * 2)) {
            return 0;
        }
    }

    return g_SetArrayElem(pArr, pArr->size++, pElem, elem_size);
}