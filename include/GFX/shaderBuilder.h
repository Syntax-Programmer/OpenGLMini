#pragma once

#include <glad/glad.h>
#include <stdint.h>

extern int32_t shdr_Build(GLuint *pProgram, const char *vert_path,
                          const char *frag_path);
extern void shdr_Delete(GLuint *pProgram);