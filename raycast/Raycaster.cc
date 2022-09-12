#include "Raycaster.h"

Raycaster::Raycaster() {
    //run();
}

void Raycaster::setPosX(double n) {
    if (n > 0 && n < WIDTH) {
        posX = n;
    } else {
        posX = WIDTH / 2;
    }
}

void Raycaster::setPosY(double n) {
    if (n > 0 && n < HEIGHT) {
        posX = n;
    } else {
        posX = HEIGHT / 2;
    }
}

void Raycaster::setRot(double n) {
    rot = normalize(n);
}

void Raycaster::setMap(int x, int y, int a) {
    if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT) {
        map[MAP_HEIGHT - 1 - y][x] = a;
    }
}


int Raycaster::getMap(int x, int y) {
    if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT) {
        return map[MAP_HEIGHT - 1 - y][x];
    }
    
    return -1;
}

void Raycaster::redraw() {
    // all of the code from ncurses version

    // draw background
    /*
    glBegin(GL_QUADS);

    glColor3f(0.0, 0.0, 0.5); // sky
    glVertex2i(0, HEIGHT);
    glVertex2i(0, HEIGHT / 2);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(WIDTH, HEIGHT / 2);
    
    glColor3f(0.0, 0.5, 0.0); // ground
    glVertex2i(0, HEIGHT / 2); 
    glVertex2i(0, 0);
    glVertex2i(WIDTH, HEIGHT / 2);
    glVertex2i(WIDTH, 0);

    glEnd();
    */  

    // setup variables for our raycasting
    double currentOffset = degToRad(FOV/2);
    double currentAngle = normalize(rot + currentOffset);
    double rayX, rayY, offsetX, offsetY;
    int depthOfField, mapX, mapY;
    int mapTileH, mapTileV, mapTile;

    // get the heights of our bars
    for (int i = 0; i < WIDTH; i++) {
        double aTan = -1/tan(currentAngle);
        double nTan = -tan(currentAngle);

        depthOfField = 0;
        /// horizontal lines
        if (currentAngle < M_PI) { /// looking up
            rayY = int(posY + 1);
            rayX = (rayY - posY) * -aTan + posX;
            offsetY = 1;
            offsetX = -offsetY * aTan;
        } else if (currentAngle > M_PI) { /// looking down
            rayY = int(posY);
            rayX = (posY - rayY) * aTan + posX;
            offsetY = -1;
            offsetX = -offsetY * aTan;
        } else if (fabs(currentAngle - 0) < 0.001 || fabs(currentAngle - M_PI) < 0.001) {
            rayX = posX;
            rayY = posY;
            depthOfField = MAP_HEIGHT;
        }

        while (depthOfField < MAP_HEIGHT) {
            mapX = int(rayX);
            mapY = int(rayY + (currentAngle < M_PI ? 0.1 : -0.1));
            if (getMap(mapX, mapY) != 0) {
                depthOfField = MAP_HEIGHT;
            } else {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField++;
            }
        }

        mapTileH = getMap(mapX, mapY);

        double distanceToH = distance(posX, rayX, posY, rayY);

        /// vertical lines
        depthOfField = 0;
        if (currentAngle < M_PI / 2 || currentAngle > 3 * M_PI / 2) { /// looking right
            rayX = int(posX + 1);
            rayY = (rayX - posX) * -nTan + posY;
            offsetX = 1;
            offsetY = -offsetX * nTan;
        } else if (currentAngle > M_PI / 2 && currentAngle < 3 * M_PI / 2) { /// looking left
            rayX = int(posX);
            rayY = (posX - rayX) * nTan + posY;
            offsetX = -1;
            offsetY = -offsetX * nTan;
        } else if (fabs(currentAngle - M_PI / 2) < 0.001 || fabs(currentAngle - 3 * M_PI / 2) < 0.001) {
            rayX = posX;
            rayY = posY;
            depthOfField = MAP_HEIGHT;
        }

        while (depthOfField < MAP_HEIGHT) {
            mapX = int(rayX + (currentAngle < M_PI / 2 || currentAngle > 3 * M_PI / 2 ? 0.1 : -0.1));
            mapY = int(rayY);
            if (getMap(mapX, mapY) != 0) {
                depthOfField = MAP_HEIGHT;
            } else {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField++;
            }
        }

        int mapTileV = getMap(mapX, mapY);
        
        double distanceToV = distance(posX, rayX, posY, rayY);
        
        // pick the shortest distance and normalize it
        double distance;
        int mapTile;
        double color = 0;
        if (distanceToV < distanceToH) {
            distance = distanceToV;
            mapTile = mapTileV;
            color = 0.8;
        } else {
            distance = distanceToH;
            mapTile = mapTileH;
            color = 0.5;
        }
        distance = distance * cos(currentOffset);

        int height = int(HEIGHT / distance + 0.5);
        //heights[i] = int(NUM_ROWS / distance + 0.5);
        //heights[i] = roundDownToEven(heights[i]); // rounds numbers down to the next even number

        // draw the column onto the screen
        glBegin(GL_LINES);
        glColor3d(color, 0, 0);
        glVertex2i(i, (HEIGHT - height) / 2);
        glVertex2i(i, HEIGHT - (HEIGHT - height) / 2);
        glEnd();

        // update your offset/angle
        currentOffset -= degToRad(FOV/WIDTH);
        currentAngle = normalize(rot + currentOffset);
    }
}

bool Raycaster::input(unsigned char key) {
    if (key == 'w') {
        // checking to make sure this coordinate works
        double tempX = posX + SPEED * cos(rot);
        double tempY = posX + SPEED * sin(rot);

        // check up and down
        if (tempY - int(tempY) > 0.8 && getMap(int(tempX), int(tempY + 1)) != 0) { // up
            posY = (tempY) + 0.8;
        } else if (tempY - int(tempY) < 0.2 && getMap(int(tempX), int(tempY - 1)) != 0) { // down
            posY = int(tempY) + 0.2;
        } else {
            posY = tempY;
        }

        // check left and right
        if (tempX - int(tempX) > 0.8 && getMap(int(tempX + 1), int(tempY)) != 0) { // right
            posX = int(tempX) + 0.8;
        } else if (tempX - int(tempX) < 0.2 && getMap(int(tempX - 1), int(tempY)) != 0) { // left
            posX = int(tempX) + 0.2;
        } else {
            posX = tempX;
        }

    } else if (key == 's') {
        // checking to make sure this coordinate works
        double tempX = posX - SPEED * cos(rot);
        double tempY = posY - SPEED * sin(rot);

        // check up and down
        if (tempY - int(tempY) > 0.8 && getMap(int(tempX), int(tempY + 1)) != 0) { // up
            posY = int(tempY) + 0.8;
        } else if (tempY - int(tempY) < 0.2 && getMap(int(tempX), int(tempY - 1)) != 0) { // down
            posY = int(tempY) + 0.2;
        } else {
            posY = tempY;
        }

        // check left and right
        if (tempX - int(tempX) > 0.8 && getMap(int(tempX + 1), int(tempY)) != 0) { // right
            posX = int(tempX) + 0.8;
        } else if (tempX - int(tempX) < 0.2 && getMap(int(tempX - 1), int(tempY)) != 0) { // left
            posX = int(tempX) + 0.2;
        } else {
            posX = tempX;
        }

    } else if (key == 'a') {
        rot += ANG_SPEED;
        if (rot >= 2 * M_PI) {
            rot -= 2 * M_PI;
        }
    } else if (key == 'd') {
        rot -= ANG_SPEED;
        if (rot < 0) {
            rot += 2 * M_PI;
        }
    } else if  (key == 'e') {
        for (int i = 0; i < MAP_WIDTH; i++) {
            for (int j = 0; j < MAP_HEIGHT; j++) {
                if (getMap(i, j) == 2) {
                    if (distance(posX, i + 0.5, posY, j + 0.5) <= 1.5) {
                        setMap(i, j, 0);
                    }
                }
            }
        }
    } else if (key == 'd' /* todo replace with delete key */) {
        return false;
    }
    return true;
}

double Raycaster::degToRad(double i) {
    return i * (M_PI / 180);
}

double Raycaster::normalize(double angle) {
    if (angle > 2 * M_PI) {
        return normalize(angle - 2 * M_PI);
    } else if (angle < 0) {
        return normalize(angle + 2 * M_PI);
    } else {
        return angle;
    }
}

double Raycaster::distance(double x1, double x2, double y1, double y2) {
    return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}