//
// Created by anton on 29.04.2024.
//

#ifndef FONTMANAGER_DEMOWINDOW_H
#define FONTMANAGER_DEMOWINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class DemoWindow {
public:
    void Init();

    void Draw();

    static void create_triangle();

    static void add_shader(GLuint program, const char* shader_code, GLenum type);

    static void create_shaders();

    static void create_framebuffer();

    static void bind_framebuffer();

    static void unbind_framebuffer();

    static void rescale_framebuffer(float width, float height);

    static void terminate();

    static GLuint texture_id;

    static GLuint VAO;

    static GLuint shader;

private:
    GLFWwindow *mywindow = nullptr;
};


#endif //FONTMANAGER_DEMOWINDOW_H
