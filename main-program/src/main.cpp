#include "opengl.h"
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
        GL gl;
        /* initiate the communication with the simulation */
        socket.init_socket();
        /* initiate the window */
        gl.init_window();
        /* read the user defined shader program */
        gl.compile_shader();
        /* socket connections */
        socket.print_addr();
        socket.create();
        socket.make_connection();
        while(1){
            buffer_pointer = socket.run();
            gl.json_parser(buffer_pointer);
            gl.draw();
        }
        socket.cleanup();
    }
    catch(...){
        std::cout<<"Error occured..."<<std::endl;
    }

    return 0;
}