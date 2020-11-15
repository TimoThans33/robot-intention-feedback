#include "opengl.h"

/* construct the VBO: vertex buffer object */
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

/* Setting an error callback */
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void GL::draw(void)
{
    float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    /* much nicer function for rescaling compared to glOrtho */
    glViewport(0, 0, width, height);
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    /* in the future we will want GL_DEPTH_BUFFER_BIT (delete geometry behind user) */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* some basic matrix multiplications from linmath.h (currently zero angle) */
    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, 0.0);
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    /* link to the shader */
    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    glDrawArrays(GL_LINE_STRIP, 0, 3);

    /* convenient glfw functions for updating window and getting window input events */
    glfwSwapBuffers(window);
    glfwPollEvents();
    /* check whethert user wants to close the window */
    if (glfwWindowShouldClose(window))
        throw;
}

/* some basic window creation, glfw makes this very easy */
void GL::init_window(void){
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        throw;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* change here the width & height accordingly */
    window = glfwCreateWindow(640, 320, "robot-intention-feedback", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);
}

/* json parser for incoming message via the network */
void GL::json_parser(char *socket_data){
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

/* compile the defined shaders in basic.vert and basic.frag */
void GL::compile_shader(void)
{
    /* use fstream to read data from file into array */
    std::ifstream fs("src/shader/basic.frag");
    std::string fs_contents((std::istreambuf_iterator<char>(fs)),
        std::istreambuf_iterator<char>());
    static const char* fragment_shader_text = fs_contents.c_str();
    std::ifstream vs("src/shader/basic.vert");
    std::string vs_contents((std::istreambuf_iterator<char>(vs)),
        std::istreambuf_iterator<char>());
    static const char* vertex_shader_text = vs_contents.c_str();
    /* give some feedback to the user */
    std::cout<<"using vertex shader : \n"<<vertex_shader_text<<std::endl;
    std::cout<<"using fragment shader : \n"<<fragment_shader_text<<std::endl;
    /* creates and initializes a buffer object's data store */
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Data), &data[0].x, GL_STATIC_DRAW );
    /* creating the vertex shader */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(fragment_shader);
    /* creating the fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(fragment_shader);
    /* link the shaders to the program object (in this context a program is an executable file on the GPU) */
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void*) 0);

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void*) (sizeof(float) * 2));
}