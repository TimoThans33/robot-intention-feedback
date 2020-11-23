#include "scene.h"

/* Test data structure */
/*
static const struct Test
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};
*/
/* VBO data structure */
struct Data
{
    /* data */
    GLfloat x, y;
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

/* link shader with VBO */
int Scene::compile_shader(void)
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

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    GLint vs_isCompiled = 0;
    std::cout<<"address of error flag : "<<&vs_isCompiled<<std::endl;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vs_isCompiled);
    if(vs_isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        std::cout<<"address of maxLength : "<<&maxLength<<" | value : "<< maxLength <<std::endl;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::cout<<"value of maxLength : "<< maxLength << std::endl;

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        std::cout<<"address of errorlog : "<<&errorLog[0]<<std::endl;
        glGetShaderInfoLog(vertex_shader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vertex_shader); // Don't leak the shader.
        std::cout<<"Error in compiling the vertex shader" << errorLog[0] << std::endl;
        for(int i=0; i<maxLength; i++){
            std::cout<<errorLog[i];
        }
        throw;
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
 
    GLint fs_isCompiled = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fs_isCompiled);
    if(fs_isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragment_shader); // Don't leak the shader.
        std::cout<<"Error in compiling the fragment shader :"<<std::endl;
        for(int i=0; i<maxLength; i++){
            std::cout<<errorLog[i];
        }
        throw;
    }
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    return 1;
}

/* initiate GLFW */
void Scene::init_glfw(void)
{
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
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

/* render */
void Scene::draw(char * socket_data)
{
    float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    // glOrtho(0.0, 0.4, -0.2, 0.2, 0.0f, 100.);
    glViewport(-1200, 0, width*3.8, height);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4_identity(m);
    mat4x4_rotate_Y(m, m, 0.0);// mat4x4_rotate_Z(m, m, (float) glfwGetTime());
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    glUseProgram(program);
    /* Draw using the vertices in our vertex buffer object */
    /* create the vertex array object */
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    /* The MVP matrix */
    mvp_location = glGetUniformLocation(program, "MVP");
    /* Fill the array with json values */
    size_t data_points;

    parsed_json = json_tokener_parse(socket_data);

    json_object_object_get_ex(parsed_json, "x", &x);
    json_object_object_get_ex(parsed_json, "y", &y);

    data_points = json_object_array_length(x);
    std::cout<<"Number of data points : "<<data_points<<std::endl;

    Data vertices[data_points];

    for(int i=0;i<data_points;i++){
        x_id = json_object_array_get_idx(x, i);
        y_id = json_object_array_get_idx(y, i);

        std::string x_coord = json_object_get_string(x_id);
        std::string y_coord = json_object_get_string(y_id);
        std::string::size_type sz;

        vertices[i].x = std::stof (x_coord,&sz);
        vertices[i].y = std::stof (y_coord,&sz);
        std::cout<<"x = "<<vertices[i].x<<std::endl;
        std::cout<<"y = "<<vertices[i].x<<std::endl;
    }
    std::cout<<"------------------"<<std::endl;
    /* copy the array to the buffer object */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertex_buffer);
    glVertexAttribPointer(vertex_buffer, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    glDrawArrays(GL_LINE_STRIP, 0, data_points);

    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window)){
        glfw_cleanup();
        throw;
    }
}
/*
void Scene::json_parser(char *socket_data){
    size_t data_points;
    
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
*/
void Scene::glfw_cleanup(void){
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
