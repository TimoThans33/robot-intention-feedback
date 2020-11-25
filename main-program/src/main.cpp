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
        /* read the defined shaders into a char array */
        char vs_direction[] = "src/shader/basic.vert";
        char fs_direction[] = "src/shader/basic.frag";

        /* link the shaders to our program */
        int main_flag = scene.link_shader(vs_direction, fs_direction);

        while(main_flag==0){
            /* get json data from socket */
            buffer_pointer = socket.run();
            /* the draw function will return non-zero upon exit */
            main_flag = scene.draw(buffer_pointer);
        }
        /* do some proper clean-up */
        socket.cleanup();
        scene.glfw_cleanup();
    }
    catch(...)
    {
        std::cout<<"an error occurred"<<std::endl;
    }
    
}