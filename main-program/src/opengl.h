#ifndef OPENGL_H
#define OPENGL_H

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

class GL{
    GLFWwindow* window;
    GLuint vao, vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;
    public:
        // json declerations
        struct json_object *parsed_json;
        struct json_object *x;
        struct json_object *y;
        struct json_object *x_id;
        struct json_object *y_id;

        size_t data_points;
        void init_window(void);
        void json_parser(char *socket_data);
        void draw(void);
        void compile_shader(void);
};
#endif