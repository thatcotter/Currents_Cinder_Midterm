#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/System.h"

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
    void mouseMove( ci::app::MouseEvent event) override;
	void update() override;
	void draw() override;
    int score1 = 0;
    int score2 = 0;
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
    
    listener.setup( 7777 );
    std::string host = ci::System::getIpAddress();
    // assume the broadcast address is this machine's IP address but with 255 as the final value
    // so to multicast from IP 192.168.1.100, the host should be 192.168.1.255
    if( host.rfind( '.' ) != string::npos )
        host.replace( host.rfind( '.' ) + 1, 3, "255" );
    console() << host << endl;
    
    //override
//    host = "149.31.131.125";
    
    
    sender.setup( host, 8888, true );
    
    theirPaddle = Paddle::create( glm::vec2( 50, getWindowHeight()/2), 10 );
    myPaddle = Paddle::create( glm::vec2( getWindowWidth()- 50, getWindowHeight()/2 ), 10 );
}

void Pong_OSC_GuestApp::mouseDown( MouseEvent event )
{
}

void Pong_OSC_GuestApp::mouseMove(ci::app::MouseEvent event)
{
    myPaddle->mouseMove( event );
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
        cout << "New Message!" << endl;
        
        
        if (message.getArgAsString(0) == "/player/position") {
            //set other paddle position
            theirPaddle->updateOsc(message);
//            cout << "New Paddle Message!" << endl;
        }
        
        if (message.getArgAsString(0) == "/ball/position") {
            //set puck posititon
            _Puck->mLoc.x = message.getArgAsFloat(1);
            _Puck->mLoc.y = message.getArgAsFloat(2);
        }
        
        if (message.getArgAsString(0) == "/score") {
            score1 = message.getArgAsFloat(1);
            score2 = message.getArgAsFloat(2);
        }
    }
    
    osc::Message message1;
    //    message1.setAddress("/paddlePos");
    message1.addStringArg("/player/position");
    message1.addFloatArg(myPaddle->getPos().y);
    sender.sendMessage(message1);
//    console() << message1.getArgAsString(0) << endl;
//    console() << message1.getArgAsFloat(1) << endl;
}

void Pong_OSC_GuestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    _Puck->draw();
    myPaddle->draw();
    theirPaddle->draw();
    
    ci::gl::drawString(std::to_string(int(score2)), glm::vec2(20.f, 50.f));
    ci::gl::drawString(std::to_string(int(score1)), glm::vec2(ci::app::getWindowWidth()-20.f, 50.f));
    
    
}

CINDER_APP( Pong_OSC_GuestApp, RendererGl )
