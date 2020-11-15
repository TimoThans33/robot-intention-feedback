#ifndef OPENGL_H
#define OPENGL_H

#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h> // will be included by Xlib.h
#include <X11/Xlib.h>

#include <GL/gl.h> // will be included by glx.h
#include <GL/glx.h>
#include <GL/glu.h>

#include <vector>
#include <json-c/json.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fstream>

class window{
    public:
        Display *dpy;
        Window root;
        XVisualInfo *vi;
        Colormap cmap;
        XSetWindowAttributes swa;
        Window win;
        GLXContext glc;
        XWindowAttributes gwa;
        XEvent xev;
        // json declerations
        struct json_object *parsed_json;
        struct json_object *x;
        struct json_object *y;
        struct json_object *x_id;
        struct json_object *y_id;

        size_t data_points;
        GLint att[5] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        void init_window(void);
        void json_parser(char *socket_data);
        void run(void);
        void get_shader(void);
};

#endif