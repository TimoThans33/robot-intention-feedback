#include "scene.h"
#include "socketdev.h"

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

//#define DEBUG

int main(int argc, char* argv[]){
    try {
        char *buffer_pointer;
        std::string vertex_shader_text;
        std::string fragment_shader_text;
        const char* vs_text;
        const char* fs_text;
        int vertex_flag, fragment_flag;
        GLuint vertex_shader, fragment_shader, program;

        socket_programming socket(argc, argv);
        Scene scene;

        /* initiate the communication with the simulation */
        socket.init_socket();
        /* initiate the window */
        scene.init_glfw();
        /* socket connections */
        socket.print_addr();
        socket.create();
        socket.make_connection();
        /* read the defined shaders */
        char vs_direction[] = "src/shader/basic.vert";
        char fs_direction[] = "src/shader/basic.frag";

        vertex_shader_text = scene.read_shader(vs_direction);
        vs_text = vertex_shader_text.c_str();

        fragment_shader_text = scene.read_shader(fs_direction);
        fs_text = fragment_shader_text.c_str();

        /* compile the defined shaders */
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vs_text, NULL);
        glCompileShader(vertex_shader);

        /* check for succesfull compilation */
        vertex_flag = scene.get_compile_data(vertex_shader);
        if (vertex_flag==1){
            std::cout<<"Error when compiling the vertex shader"<<std::endl;
            throw;
        }
        std::cout<<"using vertex shader : \n"<<vs_text<<std::endl;

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fs_text, NULL);
        glCompileShader(fragment_shader);

        fragment_flag = scene.get_compile_data(fragment_shader);
        if (fragment_flag==1){
            std::cout<<"Error when compiling the fragment shader"<<std::endl;
            throw;
        }
        std::cout<<"using fragment shader : \n"<<fs_text<<std::endl;

        /* link the shaders to our program */
        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        int main_flag = 0;

        while(main_flag==0){
            buffer_pointer = socket.run();
            /* the draw function will return non-zero upon exit */
            main_flag = scene.draw(buffer_pointer);
        }
        /* do some proper clean-up */
        socket.cleanup();
        scene.glfw_cleanup();
        return 0;
    }
    catch(...)
    {
        std::cout<<"an error occurred"<<std::endl;
    }
}