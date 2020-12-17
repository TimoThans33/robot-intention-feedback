#include "scene.h"

/* VBO data structure */
struct Data
{
    /* data */
    GLfloat x, y;
    GLfloat r, g, b;
};

std::vector <Data> data;

struct Velocity
{
    GLfloat v;
};

// #define DEBUG

float x_values = 1.0;
float y_values = 1.0;
float v_values = 0.5;

/* Function Prototypes */
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
/* End of Function Prototypes */

/* read the text from the basic.frag and basic.frag files, return the contents of
the defined file as a char array (string) */
std::string Scene::read_shader(char direction[])
{
    /* use fstream to read data from file into array */
    std::ifstream in(direction);
    std::string contents((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());
    std::string shader_text = contents.c_str();
    return shader_text;
}

/* check for compile errors from the glsl compiler, pass 0 upon completion and 1 when error occured */
int Scene::get_compile_data(GLuint shader)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        /* ERROR handling */
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        /* make sure we don't leak the shader */
        glDeleteShader(shader);
        for(int i=0; i<maxLength; i++){
            std::cout<<errorLog[i];
        }
        /* return 1 on error */
        return 1;
    }
    /* return 0 when succesfull */
    return 0;
}

/* initiate GLFW */
void Scene::init_glfw(void)
{
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    window = glfwCreateWindow(640, 360, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);
}

/* render the data recieved from the socket */
int Scene::draw(char *socket_data)
{
    float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4_identity(m);
    /* This will rotate the projection */
    mat4x4_rotate_Y(m, m, 0.0);// mat4x4_rotate_Z(m, m, (float) glfwGetTime());
    mat4x4_ortho(p, 0.f, 0.7, -.3f, .3f, 1.f, -1.f);
    // mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    // mat4x4_scale(m, m, 6 );
    mat4x4_mul(mvp, p, m);
    
    glUseProgram(program);
    /* Draw using the vertices in our vertex buffer object */
    /* create the vertex array object */
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    /* The MVP matrix */
    mvp_location = glGetUniformLocation(program, "MVP");
    vcol_location = glGetAttribLocation(program, "velocity_color");
    /* Fill the array with json values */
    size_t data_points;
    
    parsed_json = json_tokener_parse(socket_data);
    if(socket_data == nullptr) return 0;
    json_object_object_get_ex(parsed_json, "x", &x);
    if(x == nullptr) return 0;
    json_object_object_get_ex(parsed_json, "y", &y);
    if(y == nullptr) return 0;
    json_object_object_get_ex(parsed_json, "v", &v);
    if(v == nullptr) return 0;

    data_points = json_object_array_length(x);
    std::cout<<"Number of data points : "<<data_points<<std::endl;

    // struct Data vertices[data_points];

    for(int i=0;i<data_points;i++){
        x_id = json_object_array_get_idx(x, i);
        y_id = json_object_array_get_idx(y, i);
        v_id = json_object_array_get_idx(v, i);

        std::string x_coord = json_object_get_string(x_id);
        std::string y_coord = json_object_get_string(y_id);
        std::string v_coord = json_object_get_string(v_id);
        std::string::size_type sz;

        data.push_back({std::stof (x_coord, &sz), std::stof (y_coord, &sz),
                        std::stof (v_coord,&sz), 1-std::stof (v_coord, &sz), 0.0});

#ifdef DEBUG
        std::cout<<"x = "<<data[i].x<<std::endl;
        std::cout<<"y = "<<data[i].y<<std::endl;
        std::cout<<"r = "<<data[i].r<<std::endl;
        std::cout<<"g = "<<data[i].g<<std::endl;
        std::cout<<"b = "<<data[i].b<<std::endl;
#endif
        /*
        vertices[i].x = x_values;
        vertices[i].y = y_values;

        vertices[i].r = v_values / 1.0;
        vertices[i].g = 1 - v_values / 1.0;
        vertices[i].b = 0.0;

        std::cout<<"x = "<<vertices[i].x<<std::endl;
        std::cout<<"y = "<<vertices[i].y<<std::endl;
        std::cout<<"v = "<<v_values<<std::endl;
        
        vertices[i].x = std::stof (x_coord,&sz);
        vertices[i].y = std::stof (y_coord,&sz);
        data[i].v = std::stof (v_coord,&sz);

        vertices[i].r = data[i].v / 1.0;
        vertices[i].g = 1 - data[i].v / 1.0;
        vertices[i].b = 0.0;

        std::cout<<"x = "<<vertices[i].x<<std::endl;
        std::cout<<"y = "<<vertices[i].y<<std::endl;
        std::cout<<"v = "<<data[i].v<<std::endl;
        */
    }
    std::cout<<"------------------"<<std::endl;
    /* copy the array to the buffer object */
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(data), &data[0], GL_STREAM_DRAW);

    glEnableVertexAttribArray(vertex_buffer);
    glVertexAttribPointer(vertex_buffer, 2, GL_FLOAT, GL_FALSE, sizeof(data[0]), 0);

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(data[0]), (void*) (sizeof(float) * 2));

    /* changing the linewidth like this is depreceated in modern openGL... it is better to construct a geometry shader */
    glLineWidth( 5.0 );
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);

    glDrawArrays(GL_LINE_STRIP, 0, data_points);

    data.clear();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window)){
        glfw_cleanup();
        return 0;
    }
}

void Scene::glfw_cleanup(void){
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/* Setting an error callback for glfw */
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

/* link shader with VBO >> DEPRECIATED!*/
int Scene::link_shader(char vs_direction[], char fs_direction[])
{
    vertex_shader_text = read_shader(vs_direction);
    vs_text = vertex_shader_text.c_str();
    std::cout<<"using vertex shader : \n"<<vs_text<<std::endl;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_text, NULL);
    glCompileShader(vertex_shader);

    vertex_flag = get_compile_data(vertex_shader);
    if (vertex_flag==1){
        std::cout<<"Error when compiling"<<std::endl;
        throw;
    }

    std::cout<<"using vertex shader : \n"<<vs_text<<std::endl;

    fragment_shader_text = read_shader(fs_direction);
    fs_text = fragment_shader_text.c_str();

    std::cout<<"using the fragment shader : \n"<<fs_text<<std::endl;

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_text, NULL);
    glCompileShader(fragment_shader);

    fragment_flag = get_compile_data(fragment_shader);
    if (fragment_flag==1){
        std::cout<<"Error when compile the fragment shader"<<std::endl;
        throw;
    }

    std::cout<<"using the fragment shader : \n"<<fs_text<<std::endl;

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    return 0;
}
