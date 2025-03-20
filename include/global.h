#pragma once

#include <stdint.h>
#include <stddef.h>

#define SET_FLAG(var, flag)   ((var) |= (flag))
#define CLEAR_FLAG(var, flag) ((var) &= ~(flag))
#define TOGGLE_FLAG(var, flag) ((var) ^= (flag))
#define HAS_FLAG(var, flag)   (((var) & (flag)) ? 1 : 0)

typedef struct Struct_Array {
    void *arr;
    uint32_t capacity, size;
    size_t elem_size;
} Struct_Array;
typedef enum Enum_InputFlags {
    UP = (1 << 0),
    DOWN = (1 << 1),
    LEFT = (1 << 2),
    RIGHT = (1 << 3),
    ESC = (1 << 4),
    SPACE = (1 << 5),
    CTRL = (1 << 6),
    SHIFT = (1 << 7),
    QUIT = (1 << 8),
    MSB_RIGHT = (1 << 9),
    MSB_LEFT = (1 << 10)
} Enum_InputFlags;

extern void g_InitArray(Struct_Array *pArr);
extern int32_t g_AllocArray(Struct_Array *pArr, uint32_t capacity,
                         size_t elem_size);
extern void g_FreeArray(Struct_Array *pArr);
extern int32_t g_ReallocArray(Struct_Array *pArr, uint32_t new_cap);
extern void *g_GetArrayElem(Struct_Array *pArr, uint32_t index);
extern int32_t g_SetArrayElem(Struct_Array *pArr, uint32_t index, void *pElem,
                           size_t elem_size);
extern int32_t g_AddArrayElem(Struct_Array *pArr,void *pElem, size_t elem_size);