#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <math.h>
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <chrono>
#include <thread>


int main(void)
{
    double acc_array[10] = { 0,2,0.5,0,2,0,0.5,2,0,2 }; //set of made up accaleration variables

    for (int i = 0; i < 10; i++) { // loop through accaleration values
        double acc = acc_array[i];

        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "window", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            
        
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            //addressing different colours to different acceleration values
            if (acc > 0.7 || acc < -0.7) { 
                glColor3f(1, 0, 0); //red
            }
            else if (acc <= 0.7 && acc >= 0.2 || acc >= -0.7 && acc <= -0.2) {
                glColor3f(1, 1, 0); //yellow
            }
            else if (acc < 0.2 && acc > -0.2) {
                glColor3f(0, 1, 0); //green
            }

            glLineWidth(10);
            glBegin(GL_LINES); //simple line, will be computed line using coordinates later

            glVertex2f(-0.9f, 0.0f);
            glVertex2f(0.0f, 0.9f);

            glEnd();




            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            
            
            std::this_thread::sleep_for(std::chrono::seconds(1)); //wait 1 second before continuing 
            glfwDestroyWindow(window); //close window and from here it will loop again with the next acceleration value
        }
    }   
        
        glfwTerminate();

        return 0;

        
    
}
        
       
   
