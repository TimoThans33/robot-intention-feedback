#include "scene.h"

static const struct Data
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
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
void Scene::compile_shader(void)
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
    // std::cout<<"addres of the struct : "<<&data[0].x<<std::endl;
    /* allocate and initialize a buffer object */
    /*
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    */
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
    /*
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));
    */
    /* create the vertex array object */
    glCreateVertexArrays(1, &vao);
    /* Allocate and initialize a buffer object */
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, 0);
    /* Set p two vertex attributes - first positions */
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Data, x));
    glEnableVertexAttribArray(1);
    /* now the colors */
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Data, r));
    glEnableVertexAttribArray(0);
    /* The MVP matrix */
    mvp_location = glGetUniformLocation(program, "MVP");
    /* Finally, bind our one and only buffer to the vertex array object */
    glVertexArrayVertexBuffer(vao, 0, vertex_buffer, offsetof(Data, x), sizeof(vertices[0]));
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
void Scene::draw()
{
    float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, (float) glfwGetTime());
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window))
        throw;
}