#include "opengl.h"

/* Use an array-of-structure form of data */
/* From OpenGL superbible:
Now we have two inputs to our vertex shader (position and color) interleaved together
in a single structure. Clearly, if we make an array of these structures, we have an AoS
layout for our data. To represent this with calls to glVertexArrayVertexBuffer(),
we have to use its stride parameter. The stride parameter tells OpenGL, how far apart
in bytes the beginning of each vertex's data is. If we leave it as 0, OpenGL
will use the same data for every vertex, however, to use the vertex structure declared above,
we can simply use sizeof(vertex) for the stride parameter and everything will work
out.
*/
struct Data
{
    /* data */
    float x, y;
    float r, g, b;
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

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, 0.0); // mat4x4_rotate_Z(m, m, (float) glfwGetTime());
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    glDrawArrays(GL_LINE_STRIP, 0, sizeof(Data) * data.size());

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
    glfwSwapInterval(1);        glfwDestroyWindow(window);
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
    const char* fragment_shader_text = fs_contents.c_str();
    std::ifstream vs("src/shader/basic.vert");
    std::string vs_contents((std::istreambuf_iterator<char>(vs)),
        std::istreambuf_iterator<char>());
    const char* vertex_shader_text = vs_contents.c_str();
    /* give some feedback to the user */
    std::cout<<"using vertex shader : \n"<<vertex_shader_text<<std::endl;
    std::cout<<"using fragment shader : \n"<<fragment_shader_text<<std::endl;
    std::cout<<"addres of the struct : "<<&data[0].x<<std::endl;
    /* allocate and initialize a buffer object */
    /*
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Data), &data[0], GL_DYNAMIC_DRAW);
    */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    
    GLint vs_isCompiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vs_isCompiled);
    if(vs_isCompiled == GL_FALSE)
    {
        GLint vs_maxLength = 0;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &vs_maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> vs_errorLog(vs_maxLength);
        glGetShaderInfoLog(vertex_shader, vs_maxLength, &vs_maxLength, &vs_errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vertex_shader); // Don't leak the shader.
        std::cout<<"Error in compiling the vertex shader" << std::endl;
        for(int i=0; i<vs_maxLength; i++){
            std::cout<<vs_errorLog[i];
        }
        throw;
    }
    
    std::cout<<"succesfully compile the vertex shader"<<std::endl;
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    
    GLint fs_isCompiled = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fs_isCompiled);
    if(fs_isCompiled == GL_FALSE)
    {
        GLint fs_maxLength = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &fs_maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> fs_errorLog(fs_maxLength);
        glGetShaderInfoLog(fragment_shader, fs_maxLength, &fs_maxLength, &fs_errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragment_shader); // Don't leak the shader.
        std::cout<<"Error in compiling the fragment shader :"<<std::endl;
        for(int i=0; i<fs_maxLength; i++){
            std::cout<<fs_errorLog[i];
        }
        throw;
    }

    std::cout<<"succesfully compiled the fragment shader"<<std::endl;

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    /*
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    std::cout<<"sizeof(Data): data.size()\n"<<sizeof(GLfloat)<<":"<< data.size() << std::endl;
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(data[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(data[0]), (void*) (sizeof(float) * 2));
    */
    /* create the vertex array object */
    glCreateVertexArrays(1, &vao);
    std::cout<<"succes vao"<<std::endl;
    /* Allocate and initialize a buffer object */
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferStorage(vertex_buffer, sizeof(Data)*data.size(), &data[0].x, 0);
    std::cout<<"succes initialize buffer object"<<std::endl;
    /* Set p two vertex attributes - first positions */
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Data, x));
    glEnableVertexAttribArray(0);
    std::cout<<"set two vertex attributes"<<std::endl;
    /* now the colors */
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Data, r));
    glEnableVertexAttribArray(1);
    std::cout<<"succesfully initiatilize color"<<std::endl;
    /* mvp matrix */
    mvp_location = glGetUniformLocation(program, "MVP");
    /* Finally, bind our one and only buffer to the vertex array object */
    glVertexArrayVertexBuffer(vao, 0, vertex_buffer, offsetof(Data, x), sizeof(data[0].x));
    std::cout<<"succesfully binded"<<std::endl;
}
