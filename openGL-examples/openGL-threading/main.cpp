#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/X.h> // will be included by Xlib.h
#include <X11/Xlib.h>
// openGL classes
#include <GL/gl.h> // will be included by glx.h
#include <GL/glx.h>
#include <GL/glu.h>
// c++ classes
#include <iostream>
#include <vector>
// threading
#include <pthread.h>
#include <queue>
// socket programming
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define MAX 10
#define DEBUG
using namespace std;

// shared queue
queue<int> Q;

// Getting the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataNotProduced = PTHREAD_COND_INITIALIZER;
pthread_cond_t dataNotConsumed = PTHREAD_COND_INITIALIZER;

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
int consumerCount = 0;

struct Data
{
    /* data */
    GLfloat x, y;
    GLfloat r, g, b;
};
std::vector< Data > data;

class socket_programming{
    public:
        int network_socket;
        void init_socket(void);
        void main_loop(void);
        void *run(void *parm);
        //static void *run_thread(void *);
};

void socket_programming::init_socket(void){
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    // check for error with the connection
    if (connection_status == -1){
        cout << "Socket creation error" <<endl;
    }
}

void socket_programming::main_loop(void){
    static int producerCount = 0;

    while(1){
        // Getting the lock on the queue using mutex
        pthread_mutex_lock(&mutex);

        if (Q.size() < MAX && producerCount < MAX)
        {
            // recieve data from the server
            int server_response;
            recv(network_socket, &server_response, sizeof(server_response), 0);

            #ifdef DEBUG
            cout << "The server sent the data:" << server_response << ".\n";
            #endif

            // Pushing the number into queue
            Q.push(server_response);

            producerCount++;

            pthread_cond_broadcast(&dataNotProduced);
        }

        // If queue is full, release the lock and return
        else if (producerCount == MAX) {
            pthread_mutex_unlock(&mutex);
        }

        // Get the mutex unlocked
        pthread_mutex_unlock(&mutex);
    }
}

void cleanupHandler(void* arg){
    close(*(int*) arg);
    cout << "closing networking thread" << endl;
}

void *socket_programming::run(void *parm){
    pthread_cleanup_push(cleanupHandler, &network_socket);
    init_socket();
    main_loop();
    pthread_cleanup_pop(0);
}
/*
void *socket_programming::run_thread(void *context){
    pthread_cleanup_push(cleanupHandler, &context::network_socket);
    socket_programming::init_socket();
    socket_programming.main_loop();
    pthread_cleanup_pop(0);
    return NULL;
}
*/

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

typedef void * (*THREADFUNCPTR)(void *);

int main(int argc, char** argv){
    // Declaring socket programming from non static member function
    socket_programming * socketPtr = new socket_programming();
    // Declaring the thread
    pthread_t socketThread;

    int rc;
    
    rc = pthread_create(&socketThread, NULL, (THREADFUNCPTR) &socket_programming::run, socketPtr );

    if (rc) {
        cout << "Error: unable to create thread, " << rc <<endl;
        exit(-1);
    }

    // Graphical output will be sent to the computer on which it is executed
    dpy = XOpenDisplay(NULL);
    
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
        // Getting the lock on queue using mutex
        pthread_mutex_lock(&mutex);

        XNextEvent(dpy, &xev);
        
        // Pop only when queue has at least 1 element
        if (Q.size() > 1){
            // Get the data from the front of queue
            for (int i; i<2; i++){
                Data data_points;
                data_points.x = Q.front();
                Q.pop();
                data_points.y = Q.front();
                Q.pop();
                data_points.r = 1.0;
                data_points.g = 1.0;
                data_points.b = 1.0;
                data.push_back(data_points);
            }
            if (data.size() == 100){
                delete data[0].x;
                delete data[0].y;
            }

            #ifdef DEBUG
            cout << "Message received from thread: " << data << endl;
            #endif

            // Create the rendering
            XGetWindowAttributes(dpy, win, &gwa);
            glViewport(0, 0, gwa.width, gwa.height);
            drawArray();
            glXSwapBuffers(dpy, win);

            // Pop the consumed data from queue
            Q.pop();

            consumerCount++;

            pthread_cond_signal(&dataNotConsumed);
        }

        if (consumerCount == MAX){
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        // Get the mutex unlocked
        pthread_mutex_unlock(&mutex);

        else if (xev.type == KeyPress) {
            glXMakeCurrent(dpy, None, NULL);
            glXDestroyContext(dpy, glc);
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
            pthread_exit(NULL);
            exit(0);
        }
    }
}