#pragma once

#include <stdint.h>
#include "objectBuilder.h"
#include <GLFW/glfw3.h>

extern int32_t gfx_Init(GLFWwindow **pWindow, GLuint *pProgram,
                        Struct_GFXObjects *pGfx_object);
extern void gfx_Exit(GLFWwindow **pWindow, GLuint *pProgram,
                     Struct_GFXObjects *pGfx_object);
extern void gfx_Render(GLuint program, Struct_GFXObjects *pGfx_object);