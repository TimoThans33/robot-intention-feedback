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
        /*assign some value to memory and render the first frame */
        // buffer_pointer = socket.run();
        // scene.json_parser(buffer_pointer);
        int success = scene.compile_shader();
        // scene.draw();
        while(success==1){
            buffer_pointer = socket.run();
            scene.draw(buffer_pointer);
        }
        socket.cleanup();
    }
    catch(...){
        std::cout<<"Error occured..."<<std::endl;
    }

    return 0;
}