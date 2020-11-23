#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <vector>
#include <json-c/json.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fstream>

class Scene
{
    GLFWwindow* window;
    GLuint vbo, vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;
    const char* fragment_shader_text;
    const char* vertex_shader_text;
    public:
        // json declerations
        struct json_object *parsed_json;
        struct json_object *x;
        struct json_object *y;
        struct json_object *x_id;
        struct json_object *y_id;

        int compile_shader(void);
        void init_glfw(void);
        void draw(char *);
        // void json_parser(char *);
        void glfw_cleanup(void);
};
#endif