#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h> // will be included by Xlib.h
#include <X11/Xlib.h>

#include <GL/gl.h> // will be included by glx.h
#include <GL/glx.h>
#include <GL/glu.h>

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

GLfloat ctrlpoints[4][3] = {
    { -1.0, -1.0, 0.0}, {-0.5, 1.0, 0.0}, {0.5, -1.0, 0.0}, {1.0, 1.0, 0.0}};

void DrawAQuad() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1., 1., -1., 1., 1., 20.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
    
    glBegin(GL_QUADS);
        glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
        glColor3f(0., 1., 0.); glVertex3f(.75, -.75, 0.);
        glColor3f(0., 0., 1.); glVertex3f( .75, .75, 0.);
        glColor3f(1., 1., 0.); glVertex3f(-.75, .75, 0.);
    glEnd();
}

void drawBezier(void)
{
    int i;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_FLAT);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1., 1., -1., 1., 1., 20.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0., 0., 1., 0., 0., 0., 0., 1., 0.);

    glColor3f(0., 1.0, 0.);
    glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++)
            glEvalCoord1f((GLfloat) i/30.0);
    glEnd();
}


int main(int argc, char** argv) {
    // Graphical output will be sent to the computer on which it is executed
    dpy = XOpenDisplay(NULL);

    if(dpy == NULL) {
        printf("\n\t cannot connect to X server\n\n");
        exit(0);
    }
    // The root window is the desktop background window
    root = DefaultRootWindow(dpy);

    // Configure OpenGL capabilities
    vi = glXChooseVisual(dpy, 0, att);

    if(vi == NULL) {
        printf("\n\tno appropriate visual found\n\n");
        exit(0);
    }
    else {
        printf("\n\tvisual %p selected\n", (void *)vi->visualid);
        /* %p creates hexadecimal outpu like in glxinfo */
    }

    // Create a colormap for the window
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

    // This tells the X server that the colormap we created before shall
    // be used for the window, and the window shall respond to Exposure and
    // KeyPress events
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;
    // Here we create the window
    win = XCreateWindow(dpy, root, 0, 0, 640, 360, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XMapWindow(dpy, win);
    XStoreName(dpy, win, "VERY SIMPLE APPLICATION");

    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    glEnable(GL_DEPTH_TEST);
    while(1) {
        XNextEvent(dpy, &xev);

        if(xev.type == Expose) {
            XGetWindowAttributes(dpy, win, &gwa);
            glViewport(0, 0, gwa.width, gwa.height);
            drawBezier();
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