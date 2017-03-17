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

class Pong_OSC_GuestApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown(ci::app::KeyEvent event) override;
	void update() override;
	void draw() override;
    
    osc::Listener listener;
    osc::Sender sender;
    
    PaddleRef myPaddle;
    PaddleRef theirPaddle;
    shared_ptr<Puck> _Puck;
};

void Pong_OSC_GuestApp::setup()
{
    setWindowSize( 500, 500 );
    _Puck = make_shared<Puck>();
    
    listener.setup( 7777 ); //change later
    
    theirPaddle = Paddle::create( glm::vec2( 50, getWindowHeight()/2), 10 );
    myPaddle = Paddle::create( glm::vec2( getWindowWidth()- 50, getWindowHeight()/2 ), 10 );
}

void Pong_OSC_GuestApp::mouseDown( MouseEvent event )
{
}

void Pong_OSC_GuestApp::keyDown(ci::app::KeyEvent event)
{
    myPaddle->keyInput( event );
}

void Pong_OSC_GuestApp::update()
{
    while ( listener.hasWaitingMessages() )
    {
        osc::Message message;
        listener.getNextMessage( &message );
        
        if (message.getArgAsString(0) == "/paddlePos") {
            //set other paddle position
            theirPaddle->updateOsc(message);
            cout << "New Paddle Message!" << endl;
        }
        
        if (message.getArgAsString(0) == "/puckPos") {
            //set puck posititon
            _Puck->mLoc.x = message.getArgAsFloat(1);
            _Puck->mLoc.y = message.getArgAsFloat(2);
        }
    }
}

void Pong_OSC_GuestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    _Puck->draw();
    myPaddle->draw();
    theirPaddle->draw();
}

CINDER_APP( Pong_OSC_GuestApp, RendererGl )
