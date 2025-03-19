#pragma once

#include <glad/glad.h>
#include "../global.h"

#define PER_LINE_VERT_ATTR_COUNT 6

typedef struct Struct_GFXObjects {
    Struct_Array vertices, indices;
    GLuint vao, vbo, ebo;
} Struct_GFXObjects;

extern void obj_DemoShaderAttrLinker(void);
extern int32_t obj_Build(Struct_GFXObjects *pGfx_object, const char *data_file_path,
                         void (*shader_attr_linker)(void));
extern void obj_Delete(Struct_GFXObjects *pGfx_object);