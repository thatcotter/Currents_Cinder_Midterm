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
    
    //methods
    void update();
    void draw();
    void collision();
    
    
private:
    
    
};


