#include "Puck.h"
#include "cinder/Rand.h"
Puck::Puck()
{
    //start in the middle of the screen
    mLoc = glm::vec2(cinder::app::getWindowWidth()/2, cinder::app::getWindowHeight()/2);
    mVel = glm::vec2(0, 0);
    //Start with a random speed between -5 and 5
    //mAcc = glm::vec2(cinder::Rand::randFloat(10.f)-5.f, cinder::Rand::randFloat(10.f)-5.f);
    mAcc = glm::vec2(-1.f, 2.5f);
    r = 15;
}

Puck::~Puck()
{
    
}

void Puck::update()
{
    restart();
    wallBall();
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

void Puck::restart()
{
    if (mLoc.x < 0-r || mLoc.x > cinder::app::getWindowWidth()+r){
        //start in the middle of the screen
        mLoc = glm::vec2(cinder::app::getWindowWidth()/2, cinder::app::getWindowHeight()/2);
        mVel = glm::vec2(0, 0);
        //Start with a random speed between -5 and 5
        //mAcc = glm::vec2(cinder::Rand::randFloat(10.f)-5.f, cinder::Rand::randFloat(10.f)-5.f);
        mAcc = glm::vec2(-1.f, 2.5f);
    }
}

void Puck::wallBall()
{
    if (mLoc.y <= 0){
        mAcc.y = 1;
    } else if (mLoc.y >= cinder::app::getWindowHeight()){
        mAcc.y = -1;
    }
    
}

void Puck::collision(float side)
{
    //mAcc.x = 0;
    if (side == 1){
        mAcc.x -= -1;
    }
    if (side == 2){
        mAcc.x -= 1;
    }
}