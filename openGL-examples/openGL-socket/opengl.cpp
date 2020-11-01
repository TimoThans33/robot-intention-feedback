#include "opengl.h"


struct Data
{
    /* data */
    GLfloat x, y;
    GLfloat r, g, b;
};
std::vector< Data > data;

float array_color[] = 
{
    1.0, 0.0, 0.0, // red
};

void drawArray(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 0.4, -0.2, 0.2, 0.0f, 100.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // We need to enable access to RAM where the vertex array is stored
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    /*
    std::cout<<"data.size = "<<data.size()<<std::endl;
    for (int i=0; i<data.size(); i++){
        std::cout<<"x = "<<data[i].x<<std::endl;
        std::cout<<"y = "<<data[i].y<<std::endl;
    }
    */
    glVertexPointer(2, GL_FLOAT, sizeof(Data), &data[0].x );
    glColorPointer(3, GL_FLOAT, sizeof(Data), &data[0].r);
    glPointSize( 3.0 );
    glDrawArrays(GL_LINE_LOOP, 0, data.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glFlush();
}

void window::init_window(void){
    // Graphical output will be sent to the computer on which it is executed
    dpy = XOpenDisplay(NULL);

    if(dpy == NULL){
        std::cout<<"\n\t cannot connect to X server\n\n"<<std::endl;
        throw;
    }

    // The root window is the window in which our window is bounded by
    root = DefaultRootWindow(dpy);

    // Configure OpenGL capabilities
    vi = glXChooseVisual(dpy, 0, att);

    if(vi == NULL){
        std::cout<<"\n no appropriate visual found\n" << std::endl;
        throw;
    }
    else{
        printf("\n\t visual %p selected \n", (void *)vi->visualid);
    }
    // Create a colormap for the window
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

    // This tells the X that the colormap we created before shall be used for the window
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    // Here we create the window
    win = XCreateWindow(dpy, root, 0, 0, 640, 360, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XMapWindow(dpy, win);
    // render the opengl pipeline on the x window
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    glEnable(GL_DEPTH_TEST);
}

void window::json_parser(char *socket_data){
    data.clear();

    parsed_json = json_tokener_parse(socket_data);

    json_object_object_get_ex(parsed_json, "x", &x);
    json_object_object_get_ex(parsed_json, "y", &y);

    data_points = json_object_array_length(x);
    std::cout<<"Number of data points : "<<data_points<<std::endl;

    for(int i=0;i<data_points;i++){
        Data render_data;
        x_id = json_object_array_get_idx(x, i);
        y_id = json_object_array_get_idx(y, i);
#ifdef DEBUG
        printf("x : %lu. %s\n", i+1, json_object_get_string(x_id));
        printf("y : %lu. %s\n", i+1, json_object_get_string(y_id));
#endif
        std::string x_coord = json_object_get_string(x_id);
        std::string y_coord = json_object_get_string(y_id);
        std::string::size_type sz;

        render_data.x = std::stof (x_coord,&sz);
        render_data.y = std::stof (y_coord,&sz);
        std::cout<<"x = "<<render_data.x<<std::endl;
        std::cout<<"y = "<<render_data.x<<std::endl;
        render_data.r = array_color[0];
        render_data.g = array_color[1];
        render_data.b = array_color[2];
        data.push_back(render_data);
    }
    std::cout<<"------------------"<<std::endl;
}

void window::run(void){
    // XNextEvent(dpy, &xev);

    XGetWindowAttributes(dpy, win, &gwa);
    glViewport(0, 0, gwa.width, gwa.height);
    drawArray();
    glXSwapBuffers(dpy, win);
    /*
    if(xev.type == KeyPress){
        glXMakeCurrent(dpy, None, NULL);
        glXDestroyContext(dpy, glc);
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
        exit(0);
    }
    */
}