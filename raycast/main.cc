#include "Raycaster.h"
#include <math.h>

void input(unsigned char key, int a, int b);
void redraw();

Raycaster ray = Raycaster();
int main(int argc, char* argv[]) {
    // set up opengl settings
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // set up the window
    glutInitWindowSize(Raycaster::WIDTH, Raycaster::HEIGHT);
    glutCreateWindow("Hello World!");
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, Raycaster::WIDTH, Raycaster::HEIGHT, 0);
    glutDisplayFunc(redraw);
    glutKeyboardFunc(input);
    glutMainLoop();

    return 0;
}

void input(unsigned char key, int a, int b) {
    if(!ray.input(key)) {
        // end program
    }

    glutPostRedisplay();
}

void redraw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ray.redraw();
    
    glutSwapBuffers();
}