#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct Struct_Array {
    void *arr;
    uint32_t capacity, size;
    size_t elem_size;
} Struct_Array;

extern void g_InitArray(Struct_Array *pArr);
extern int32_t g_AllocArray(Struct_Array *pArr, uint32_t capacity,
                         size_t elem_size);
extern void g_FreeArray(Struct_Array *pArr);
extern int32_t g_ReallocArray(Struct_Array *pArr, uint32_t new_cap);
extern void *g_GetArrayElem(Struct_Array *pArr, uint32_t index);
extern int32_t g_SetArrayElem(Struct_Array *pArr, uint32_t index, void *pElem,
                           size_t elem_size);
extern int32_t g_AddArrayElem(Struct_Array *pArr,void *pElem, size_t elem_size);