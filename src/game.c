#include "../include/game.h"
#include "../include/GFX/gfxMgr.h"

static uint32_t GetInputs(GLFWwindow *window);
static int32_t InitGame(GLFWwindow **pWindow, GLuint *pProgram,
                        Struct_GFXObjects *pGfx_object);
static void GameLoop(GLFWwindow *window, GLuint program,
                     Struct_GFXObjects *pGfx_object);
static void ExitGame(GLFWwindow **pWindow, GLuint *pProgram,
                     Struct_GFXObjects *pGfx_object);

static uint32_t GetInputs(GLFWwindow *window) {
   uint32_t input_flags = 0;

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        SET_FLAG(input_flags, UP);
   } if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
         glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        SET_FLAG(input_flags, DOWN);
   } if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
         glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        SET_FLAG(input_flags, LEFT);
   } if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
         glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        SET_FLAG(input_flags, RIGHT);
   } if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        SET_FLAG(input_flags, ESC);
   } if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        SET_FLAG(input_flags, SPACE);
   } if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS ||
         glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        SET_FLAG(input_flags, CTRL);
   } if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS ||
         glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        SET_FLAG(input_flags, SHIFT);
   } if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        SET_FLAG(input_flags, MSB_LEFT);
   } if( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
        SET_FLAG(input_flags, MSB_RIGHT);
   }

   return input_flags;
}

static int32_t InitGame(GLFWwindow **pWindow, GLuint *pProgram,
                        Struct_GFXObjects *pGfx_object) {
    return gfx_Init(pWindow, pProgram, pGfx_object);
}

static void GameLoop(GLFWwindow *window, GLuint program,
                     Struct_GFXObjects *pGfx_object) {
    uint32_t input_flags;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        input_flags = GetInputs(window);
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