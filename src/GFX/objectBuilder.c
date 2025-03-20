#include "../../include/GFX/objectBuilder.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#define MAX_PARSABLE_LINE_LEN 1024
#define DATA_DELIM " \n"
#define PER_LINE_INDICES_COUNT 3

/**
 * Currently using a single simple data storage format.
 * Since this game needs one native data format, we can just make parser for it only,
 * and then change the parser if the data format changes.
 **/

static int32_t ParseVAttr(char *line_to_parse, Struct_Array *pVert);
static int32_t ParseIAttr(char *line_to_parse, Struct_Array *pIndices);
static int32_t ParseDataFile(Struct_GFXObjects *pGfx_object,
                             const char *data_file_path);
static void GenBuffers(Struct_GFXObjects *pGfx_object,
                       void (*attr_linker)(void));

static int32_t ParseVAttr(char *line_to_parse, Struct_Array *pVert) {
    char *token = strtok(&line_to_parse[2], DATA_DELIM);
    uint32_t i = 0;

    while (token && i < PER_LINE_VERT_ATTR_COUNT) {
        char *end_ptr;
        int32_t data_token = strtol(token, &end_ptr, 10);

        if (end_ptr[0] != '\0' || end_ptr == token) {
            fprintf(stderr, "Invalid data in given vertex line.\n");
            return 0;
        } else if (!g_AddArrayElem(pVert, (void*)&data_token,
                   sizeof(data_token))) {
            return 0;
        }

        token = strtok(NULL, DATA_DELIM);
        i++;
    }

    if (i != PER_LINE_VERT_ATTR_COUNT) {
        fprintf(stderr, "Not enough data in the line to define a vertex.\n");
        return 0;
    }

    return 1;
}

static int32_t ParseIAttr(char *line_to_parse, Struct_Array *pIndices) {
    char *token = strtok(&line_to_parse[2], DATA_DELIM);
    uint32_t i = 0;

    while (token && i < PER_LINE_INDICES_COUNT) {
        char *end_ptr;
        uint32_t data_token = strtol(token, &end_ptr, 10);

        if (end_ptr[0] != '\0' || end_ptr == token) {
            fprintf(stderr, "Invalid data in given indices line.\n");
            return 0;
        } else if (!g_AddArrayElem(pIndices, (void*)&data_token,
                   sizeof(data_token))) {
            return 0;
        }

        token = strtok(NULL, DATA_DELIM);
        i++;
    }

    if (i != PER_LINE_INDICES_COUNT) {
        fprintf(stderr, "Not enough data in the line to define a face.\n");
        return 0;
    }

    return 1;
}

static int32_t ParseDataFile(Struct_GFXObjects *pGfx_object,
                             const char *data_file_path) {
    FILE *data_file = fopen(data_file_path, "r");
    if (!data_file) {
        fprintf(stderr, "Couldn't open the data file to parse at: %s\n",
                data_file_path);
        return 0;
    }

    char line_buffer[MAX_PARSABLE_LINE_LEN];
    uint32_t line_c = 0;

    if (!g_AllocArray(&pGfx_object->vertices, 3, sizeof(int32_t)) ||
        !g_AllocArray(&pGfx_object->indices, 3, sizeof(uint32_t))) {
        return 0;
    }
    // Commenting any lines that are empty or doesn't start with a valid declarator.
    while (fgets(line_buffer, sizeof(line_buffer), data_file)) {
        line_c++;
        if (line_buffer[strlen(line_buffer) - 1] != '\n' && !feof(data_file)) {
            fprintf(stderr, "Encountered line that exceeds size limit. "
                    "Line %d exceeds the max line len of %d, Ignoring it.\n",
                    line_c, MAX_PARSABLE_LINE_LEN);
            // Digesting the entire line to ignore.
            char temp;
            while ((temp = fgetc(data_file)) != '\n' && temp != EOF);
            continue;
        } else if (line_buffer[0] == 'v' && line_buffer[1] == ' ') {
            if (!ParseVAttr(line_buffer, &pGfx_object->vertices)) {
                fprintf(stderr, "Error found in line %d\n", line_c);
                fclose(data_file);
                return 0;
            }
        } else if (line_buffer[0] == 'i' && line_buffer[1] == ' ') {
            if (!ParseIAttr(line_buffer, &pGfx_object->indices)) {
                fprintf(stderr, "Error found in line %d\n", line_c);
                fclose(data_file);
                return 0;
            }
        }
    }

    fclose(data_file);

    return 1;
}

static void GenBuffers(Struct_GFXObjects *pGfx_object,
                       void (*shader_attr_linker)(void)) {
    glGenVertexArrays(1, &pGfx_object->vao);
    glBindVertexArray(pGfx_object->vao);

    glGenBuffers(1, &pGfx_object->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pGfx_object->vbo);
    glBufferData(GL_ARRAY_BUFFER, pGfx_object->vertices.elem_size *
                 pGfx_object->vertices.size, pGfx_object->vertices.arr,
                 GL_STATIC_DRAW);

    if (pGfx_object->indices.size) {
        glGenBuffers(1, &pGfx_object->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pGfx_object->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, pGfx_object->vertices.elem_size *
                     pGfx_object->indices.size, pGfx_object->indices.arr,
                     GL_STATIC_DRAW);
    } else {
        pGfx_object->ebo = 0;
    }

    shader_attr_linker();
}

void obj_DemoShaderAttrLinker(void) {
    glVertexAttribIPointer(0, 3, GL_INT, 6 * sizeof(int32_t),
                           (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(1, 3, GL_INT, 6 * sizeof(int32_t),
                           (void*)(3 * sizeof(int32_t)));
    glEnableVertexAttribArray(1);  
}

int32_t obj_Build(Struct_GFXObjects *pGfx_object, const char *data_file_path,
                  void (*shader_attr_linker)(void)) {
    if (!ParseDataFile(pGfx_object, data_file_path)) { return 0; }
    GenBuffers(pGfx_object, shader_attr_linker);

    return 1;
}

void obj_Delete(Struct_GFXObjects *pGfx_object) {
    g_FreeArray(&pGfx_object->vertices);
    g_FreeArray(&pGfx_object->indices);

    glDeleteBuffers(1, &pGfx_object->vbo);
    glDeleteBuffers(1, &pGfx_object->ebo);
    glDeleteVertexArrays(1, &pGfx_object->vao);

    pGfx_object->vbo = 0;
    pGfx_object->ebo = 0;
    pGfx_object->vao = 0;
}
