#pragma once
#include <stdio.h>

class Puck;

class Puck
{
public:
    //constructor
    Puck();
    ~Puck();
    
    //variables
    glm::vec2 mAcc;
    glm::vec2 mVel;
    glm::vec2 mLoc;
    int r;
    
    //methods
    void update();
    void draw();
    void wallBall();
    void collision(float side);
    
    
private:
    
    
};


