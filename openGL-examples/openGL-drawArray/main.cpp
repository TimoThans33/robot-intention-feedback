#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h> // will be included by Xlib.h
#include <X11/Xlib.h>

#include <GL/gl.h> // will be included by glx.h
#include <GL/glx.h>
#include <GL/glu.h>

#include <vector>
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
    0, -100, // vertex 1
    0, 100, // vertex 2
    100, 100 // vertex 3
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