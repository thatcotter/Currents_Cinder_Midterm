#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "OscSender.h"
#include "OscListener.h"

#include "Paddle.hpp"
#include "Puck.h"
#include <memory>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

class Pong_OSC_HostApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown(KeyEvent event ) override;
	void update() override;
	void draw() override;
    
    osc::Listener listener;
    
    PaddleRef myPaddle;
    PaddleRef theirPaddle;
    shared_ptr<Puck> _Puck;
};

void Pong_OSC_HostApp::setup()
{
    setWindowSize( 500, 500 );
    _Puck = make_shared<Puck>();
    
    listener.setup( 8888 );
    myPaddle = Paddle::create( glm::vec2( 50, getWindowHeight()/2), 10 );
    theirPaddle = Paddle::create( glm::vec2( getWindowWidth()- 50, getWindowHeight()/2 ), 10 );
}

void Pong_OSC_HostApp::mouseDown( MouseEvent event )
{
}

void Pong_OSC_HostApp::keyDown(cinder::app::KeyEvent event)
{
    myPaddle->keyInput( event );
}

void Pong_OSC_HostApp::update()
{
    glm::vec2 dist1 = (*_Puck).mLoc - myPaddle->getPos();
    float dist1F = glm::length(dist1);
    console() << dist1F << endl;
    if(dist1F <= (*_Puck).r + 5){
        (*_Puck).collision(1);
    }
    glm::vec2 dist2 = theirPaddle->getPos() - (*_Puck).mLoc;
    float dist2F = glm::length(dist2);
    if(dist2F <= (*_Puck).r + 5){
        (*_Puck).collision(2);
    }
    
    (*_Puck).update();
    
    while ( listener.hasWaitingMessages() )
    {
        osc::Message message;
        listener.getNextMessage( &message );
        for (int i = 0; i < message.getNumArgs(); i++)
        {
            //
        }
    }
    
}

void Pong_OSC_HostApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    (*_Puck).draw();
    myPaddle->draw();
    theirPaddle->draw();
}

CINDER_APP( Pong_OSC_HostApp, RendererGl )
