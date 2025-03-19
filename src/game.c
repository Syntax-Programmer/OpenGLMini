#include "../include/game.h"
#include "../include/GFX/gfxMgr.h"

static int32_t InitGame(GLFWwindow **pWindow, GLuint *pProgram,
                        Struct_GFXObjects *pGfx_object);
static void GameLoop(GLFWwindow *window, GLuint program,
                     Struct_GFXObjects *pGfx_object);
static void ExitGame(GLFWwindow **pWindow, GLuint *pProgram,
                     Struct_GFXObjects *pGfx_object);

static int32_t InitGame(GLFWwindow **pWindow, GLuint *pProgram,
                        Struct_GFXObjects *pGfx_object) {
    return gfx_Init(pWindow, pProgram, pGfx_object);
}

static void GameLoop(GLFWwindow *window, GLuint program,
                     Struct_GFXObjects *pGfx_object) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        gfx_Render(program, pGfx_object);
        glfwSwapBuffers(window);
    }
}

static void ExitGame(GLFWwindow **pWindow, GLuint *pProgram,
                     Struct_GFXObjects *pGfx_object) {
    gfx_Exit(pWindow, pProgram, pGfx_object);
}

void game(void) {
    GLFWwindow *window = NULL;
    GLuint program = 0;
    Struct_GFXObjects objects;

    if (InitGame(&window, &program, &objects)) {
        GameLoop(window, program, &objects);
    }
    ExitGame(&window, &program, &objects);
}