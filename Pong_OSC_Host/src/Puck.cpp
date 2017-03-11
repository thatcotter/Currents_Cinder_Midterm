#include "Puck.h"
#include "cinder/Rand.h"
Puck::Puck()
{
    //start in the middle of the screen
    mLoc = glm::vec2(cinder::app::getWindowWidth()/2, cinder::app::getWindowHeight()/2);
    mVel = glm::vec2(0, 0);
    //Start with a random speed between -5 and 5
    mAcc = glm::vec2(cinder::Rand::randFloat(10.f)-5.f, cinder::Rand::randFloat(10.f)-5.f);
    
}

Puck::~Puck()
{
    
}

void Puck::update()
{
    mVel += mAcc;
    mLoc += mVel;
    
}

void Puck::draw()
{
    ci::gl::color(1.f, 0.f, 0.f, 1.f);
    ci::gl::drawSolidEllipse(mLoc, 15, 15);
    //set Acceleration back to 0 so it doesnt build up.
    mAcc *= 0;
}

void Puck::collision()
{
    
}