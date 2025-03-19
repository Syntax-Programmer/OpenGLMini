#include <stdio.h>
#include <stdlib.h>
#include "../../include/GFX/shaderBuilder.h"

#define INFO_LOG_SIZE 512

typedef enum Enum_ShaderType {
    VERTEX, FRAGMENT
} Enum_ShaderType;

static int32_t LoadShaderContents(const char *shader_path, char **pShader_src);

static int32_t LoadShaderContents(const char *shader_path, char **pShader_src) {
    FILE *shdr_file = fopen(shader_path, "r");
    if (!shdr_file) {
        fprintf(stderr, "Couldn't open/find the shader file at: %s\n", shader_path);
        return 0;
    }

    uint32_t file_size = 0;
    // Finding the file size by using the fseek/ftell method.
    fseek(shdr_file, 0, SEEK_END);
    file_size = ftell(shdr_file);
    fseek(shdr_file, 0, SEEK_SET);

    *pShader_src = malloc(sizeof(char) * (file_size + 1)); // To account for '\0'
    if (!(*pShader_src)) {
        fprintf(stderr, "Couldn't allocate memory to store the shader buffer.\n");
        fclose(shdr_file);
        return 0;
    }

    if (!fread(*pShader_src, 1, file_size, shdr_file)) {
        fclose(shdr_file);
        free(*pShader_src);
        *pShader_src = NULL;
        return 0;
    }
    (*pShader_src)[file_size] = '\0';

    fclose(shdr_file);

    return 1;
}

static int32_t CreateShader(const char *shader_path, GLuint *pShader,
                            Enum_ShaderType type) {
    *pShader = 0;

    char *shader_buffer = NULL;
    if (!LoadShaderContents(shader_path, &shader_buffer)) { return 0; }

    *pShader = glCreateShader((type == VERTEX) ?
                              GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(*pShader, 1,
                   (const GLchar *const *)&shader_buffer, NULL);

    free(shader_buffer); // We dont need it anymore.

    int32_t success;
    char info_log[INFO_LOG_SIZE];

    glCompileShader(*pShader);
    glGetShaderiv(*pShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(*pShader, INFO_LOG_SIZE, NULL, info_log);
        fprintf(stderr, "Couldn't compile the %s shader : %s\n",
                (type == VERTEX) ? "vertex" : "fragment", info_log);
        glDeleteShader(*pShader);
        *pShader = 0;
        return 0;
    }

    return 1;
}

int32_t shdr_Build(GLuint *pProgram, const char *vert_path,
                   const char *frag_path) {
    GLuint vert = 0;
    if (!CreateShader(vert_path, &vert, VERTEX)) { return 0; }
    GLuint frag = 0;
    if (!CreateShader(frag_path, &frag, FRAGMENT)) {
        glDeleteShader(vert);
        vert = 0;
        return 0;
    }

    *pProgram = glCreateProgram();
    glAttachShader(*pProgram, vert);
    glAttachShader(*pProgram, frag);

    glLinkProgram(*pProgram);

    glDetachShader(*pProgram, vert);
    glDetachShader(*pProgram, frag);


    glDeleteShader(vert);
    vert = 0;
    glDeleteShader(frag);
    frag = 0;

    int32_t success;
    char info_log[INFO_LOG_SIZE];

    glGetProgramiv(*pProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*pProgram, INFO_LOG_SIZE, NULL, info_log);
        fprintf(stderr, "Failed to link the shaders in the shader program.\n");
        return 0;
    }

    return 1;
}

void shdr_Delete(GLuint *pProgram) {
    if (pProgram && *pProgram) {
        glDeleteProgram(*pProgram);
        *pProgram = 0;
    }
}
