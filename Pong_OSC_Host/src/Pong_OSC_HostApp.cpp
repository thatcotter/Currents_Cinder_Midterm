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
    
    PaddleRef myPaddle;
    PaddleRef theirPaddle;
    shared_ptr<Puck> _Puck;
};

void Pong_OSC_HostApp::setup()
{
    setWindowSize( 500, 500 );
    _Puck = make_shared<Puck>();
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
    _Puck->update();
}

void Pong_OSC_HostApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    _Puck->draw();
    myPaddle->draw();
    theirPaddle->draw();
}

CINDER_APP( Pong_OSC_HostApp, RendererGl )
