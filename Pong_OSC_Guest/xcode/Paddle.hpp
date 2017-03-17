//
//  Paddle.hpp
//  Pong_OSC_Host
//
//  Created by Andrew Cotter on 3/11/17.
//
//
#include "OscListener.h"

#ifndef Paddle_hpp
#define Paddle_hpp

class Paddle;

typedef std::shared_ptr<Paddle> PaddleRef;

class Paddle
{
public:
    static PaddleRef create ( glm::vec2 position, float speed );
    
    void update();
    void updateOsc( ci::osc::Message message );
    void draw();
    void keyInput( ci::app::KeyEvent event );
    void mouseMove( ci::app::MouseEvent event );

    
    glm::vec2 getPos();
    
private:
    Paddle();
    void setup( glm::vec2 position, float speed );
    glm::vec2 pos;
    float speed;
    
    
};

#endif /* Paddle_hpp */
