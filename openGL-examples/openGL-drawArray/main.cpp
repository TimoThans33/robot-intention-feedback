#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h> // will be included by Xlib.h
#include <X11/Xlib.h>

#include <GL/gl.h> // will be included by glx.h
#include <GL/glx.h>
#include <GL/glu.h>

#include <vector>
#include <iostream>
// variables used in main
Display *dpy;
Window root;
GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext glc;
XWindowAttributes gwa;
XEvent xev;

struct Data
{
    /* data */
    GLfloat x, y;
    GLfloat r, g, b;
};

std::vector< Data > data;

float array_vertex[] =
{
    -3.69333,-3.8,15.0579,0.1,0,0,-0.115523,
    -3.68659,-3.80078,15.1097,0.162165,1.2,0,-0.115523,
    -3.67984,-3.80157,15.1466,0.206385,1.2,0,-0.115523,
    -3.6731,-3.80235,15.1768,0.242677,1.2,0,-0.115523,
    -3.66635,-3.80313,15.2031,0.274208,1.2,0,-0.115523,
    -3.65961,-3.80391,15.2267,0.302469,1.2,0,-0.115523,
    -3.65286,-3.8047,15.2482,0.328306,1.2,0,-0.115523,
    -3.64612,-3.80548,15.2681,0.352253,1.2,0,-0.115523,
    -3.63937,-3.80626,15.2868,0.374673,1.2,0,-0.115523,
    -3.63263,-3.80704,15.3045,0.395824,1.2,0,-0.115523,
    -3.62588,-3.80783,15.3212,0.415902,1.2,0,-0.115523,
    -3.61913,-3.80861,15.3371,0.435054,1.2,0,-0.115523,
    -3.61239,-3.80939,15.3524,0.453398,1.2,0,-0.115523,
    -3.60564,-3.81018,15.3671,0.471027,1.2,0,-0.115523,
    -3.5989,-3.81096,15.3813,0.488021,1.2,0,-0.115523,
    -3.59215,-3.81174,15.395,0.504442,1.2,0,-0.115523,
    -3.58541,-3.81252,15.4082,0.520345,1.2,0,-0.115523,
    -3.57866,-3.81331,15.4211,0.535777,1.2,0,-0.115523,
    -3.57192,-3.81409,15.4336,0.550776,1.2,0,-0.115523,
    -3.56517,-3.81487,15.4458,0.565377,1.2,0,-0.115523,
    -3.55843,-3.81565,15.4576,0.579611,1.2,0,-0.115523,
    -3.55168,-3.81644,15.4692,0.593503,1.2,0,-0.115523,
    -3.54494,-3.81722,15.4805,0.607078,1.2,0,-0.115523,
    -3.53819,-3.818,15.4916,0.620356,1.2,0,-0.115523,
    -3.53145,-3.81879,15.5024,0.633355,1.2,0,-0.115523,
    -3.5247,-3.81957,15.513,0.646093,1.2,0,-0.115523,
    -3.51795,-3.82035,15.5234,0.658584,1.2,0,-0.115523,
    -3.51121,-3.82113,15.5336,0.670843,1.2,0,-0.115523,
    -3.50446,-3.82192,15.5437,0.682882,1.2,0,-0.115523,
    -3.49772,-3.8227,15.5535,0.694713,1.2,0,-0.115523,
    -3.49097,-3.82348,15.5632,0.706345,1.2,0,-0.115523
};

float array_color[] = 
{
    1, 0, 0, // red
};


// Draw a triangle with an array, instead of manually calling the functions
void drawArray(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-320., 320., -180., 180., 0.0f, 100.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // We need to enable access to RAM where the vertex array is stored
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    std::cout<<"Size of array = "<<sizeof(Data)<<std::endl;
    glVertexPointer(2, GL_FLOAT, sizeof(Data), &data[0].x );
    glColorPointer(3, GL_FLOAT, sizeof(Data), &data[0].r);
    glPointSize( 3.0 );
    glDrawArrays(GL_LINE_LOOP, 0, data.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glFlush();
}


int main(int argc, char** argv){
    // Graphical output will be sent to the computer on which it is executed
    dpy = XOpenDisplay(NULL);

    int i = 0;
    printf("%d",sizeof(array_vertex) / sizeof(array_vertex[0]));
    while (i <= sizeof(array_vertex) / sizeof(array_vertex[0]) ){
        Data data_points;
        data_points.x = array_vertex[i];
        i++;
        data_points.y = array_vertex[i];
        data_points.r = array_color[0];
        data_points.g = array_color[1];
        data_points.b = array_color[2];
        data.push_back(data_points);
        i++;
    }
    
    if(dpy == NULL){
        printf("\n\t cannot connect to X server\n\n");
        exit(0);
    }

    // The root window is the window in which our window is bounded by ;p
    root = DefaultRootWindow(dpy);

    // Configure OpenGL capabilities
    vi = glXChooseVisual(dpy, 0, att);

    if(vi == NULL){
        printf("\n\tno appropriate visual found\n\n");
        exit(0);
    }
    else{
        printf("\n\tvisual %p selected\n", (void *)vi->visualid);
    }
    // Create a colormap for the window
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

    // This tells the X that the colormap we created before shall
    // be used for the window
    swa.colormap = cmap;
    // We don't want to interact with the window so:
    swa.event_mask = ExposureMask | KeyPressMask;
    //swa.event_mask = KeyPressMask;

    // Here we create the window
    win = XCreateWindow(dpy, root, 0, 0, 640, 360, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XMapWindow(dpy, win);
    // We don't need a name
    // XStoreName(dpy, win, "very simple application");

    // render the opengl pipeline on the x window
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    glEnable(GL_DEPTH_TEST);
    while(1) {
        XNextEvent(dpy, &xev);

        if(xev.type == Expose) {
            XGetWindowAttributes(dpy, win, &gwa);
            glViewport(0, 0, gwa.width, gwa.height);
            // request for data
            // recieve data
            // decode data
            // draw data
            drawArray();
            glXSwapBuffers(dpy, win);
        }

        else if(xev.type == KeyPress) {
            glXMakeCurrent(dpy, None, NULL);
            glXDestroyContext(dpy, glc);
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
            exit(0);
        }
    }


}