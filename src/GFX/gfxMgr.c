#include <stdio.h>
#include "../../include/GFX/shaderBuilder.h"
#include "../../include/GFX/gfxMgr.h"

static void FramebufferSizeCallback(GLFWwindow *window, int32_t w, int32_t h);

static void FramebufferSizeCallback(GLFWwindow *window, int32_t w, int32_t h) {
    glViewport(0, 0, w, h);
}

int32_t gfx_Init(GLFWwindow **pWindow, GLuint *pProgram,
                 Struct_GFXObjects *pGfx_object) {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW initialization failed.\n");
        return 0;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6 );
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // This is the inital dimensions. New screen size could be found using glfwGetWindowSize().
    *pWindow = glfwCreateWindow(1000, 800, "My Game", NULL, NULL);
    if (!(*pWindow)) {
        fprintf(stderr, "Creation of a GLFW window failed.\n");
        return 0;
    }

    glfwMakeContextCurrent(*pWindow);
    glfwSetFramebufferSizeCallback(*pWindow, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "GLAD initialization failed.\n");
        return 0;
    }
    if (!shdr_Build(pProgram, "shaders/Demo.vert", "shaders/Demo.frag") ||
        !obj_Build(pGfx_object, "Demo.obj", obj_DemoShaderAttrLinker)) {
        return 0;
    }

    return 1;
}

void gfx_Exit(GLFWwindow **pWindow, GLuint *pProgram,
              Struct_GFXObjects *pGfx_object) {
    shdr_Delete(pProgram);
    obj_Delete(pGfx_object);
    if (pWindow && *pWindow) {
        glfwDestroyWindow(*pWindow);
        *pWindow = NULL;
    }
    glfwTerminate();
}

void gfx_Render(GLuint program, Struct_GFXObjects *pGfx_object) {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(pGfx_object->vao);
    glUseProgram(program);

    if (pGfx_object->indices.size) {
        glDrawElements(GL_TRIANGLES, pGfx_object->indices.size, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, pGfx_object->vertices.size / PER_LINE_VERT_ATTR_COUNT);
    }

    glUseProgram(0);
    glBindVertexArray(0);
}
