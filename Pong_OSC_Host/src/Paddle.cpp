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

void Paddle::updateOsc( ci::osc::Message message )
{
    this->pos.y = message.getArgAsFloat(1);
}

void Paddle::keyInput(ci::app::KeyEvent event)
{
    if (event.getCode() == 273) {
        this->pos.y -= this->speed;
    }else if (event.getCode() == 274) {
        this->pos.y += this->speed;
    }
}

void Paddle::draw()
{
    ci::gl::color( 1.0, 1.0, 1.0 );
    ci::gl::drawSolidRect( ci::Rectf( this->pos.x - 5, this->pos.y - 50, this->pos.x + 5, this->pos.y + 50 ) );
}

glm::vec2 Paddle::getPos()
{
    return this->pos;
}
