#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

class Raycaster {
    public:
    Raycaster();
    
    // setters
    void setPosX(double n);
    void setPosY(double n);
    void setRot(double n);
    void setMap(int x, int y, int a);

    // getters
    double getPosX() {return posX;}
    double getPosY() {return posY;}
    double getRot() {return rot;}
    int getMap(int x, int y);

    // helpers
    void redraw();
    bool input(unsigned char key);
    static double degToRad(double i);
    static double normalize(double angle);
    static double distance(double x1, double x2, double y1, double y2);

    static const int WIDTH = 640;
    static const int HEIGHT = 480;

    static const int MAP_WIDTH = 16;
    static const int MAP_HEIGHT = 16;

    static constexpr double SPEED = 0.01;
    static constexpr double ANG_SPEED = 2 * M_PI / 180;
    static constexpr double FOV = 80;

    private:
    int heights[640];

    int map[16][16] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    double posX = 1.5;
    double posY = 1.5;
    double rot = M_PI_2;
    

};

#endif