//
//  Paddle.cpp
//  Pong_OSC_Host
//
//  Created by Andrew Cotter on 3/11/17.
//
//

#include "Paddle.hpp"


PaddleRef Paddle::create(glm::vec2 position, float speed)
{
    PaddleRef ref = std::shared_ptr<Paddle>( new Paddle() );
    ref->setup( position, speed );
    return ref;
}

Paddle::Paddle()
{
    
}

void Paddle::setup(glm::vec2 position, float speed)
{
    this->pos = position;
    this->speed = speed;
}

void Paddle::keyInput(ci::app::KeyEvent event)
{
    if (event.KEY_DOWN) {
        this->pos.y -= this->speed;
    }else if (event.KEY_UP) {
        this->pos.y += this->speed;
    }
}

void Paddle::draw()
{
    ci::gl::drawSolidRect( ci::Rectf( this->pos.x - 5, this->pos.y - 50, this->pos.x + 5, this->pos.y + 50 ) );
}
