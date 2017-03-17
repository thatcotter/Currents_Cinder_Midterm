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

class Pong_OSC_HostApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown(KeyEvent event ) override;
    void mouseMove( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    osc::Listener listener;
    osc::Sender sender;
    
    PaddleRef myPaddle;
    PaddleRef theirPaddle;
    shared_ptr<Puck> _Puck;
};

void Pong_OSC_HostApp::setup()
{
    setWindowSize( 500, 500 );
    _Puck = make_shared<Puck>();
    
    listener.setup( 8888 );
    
    std::string host = ci::System::getIpAddress();
    // assume the broadcast address is this machine's IP address but with 255 as the final value
    // so to multicast from IP 192.168.1.100, the host should be 192.168.1.255
    if( host.rfind( '.' ) != string::npos )
        host.replace( host.rfind( '.' ) + 1, 3, "255" );
    console() << host << endl;
    sender.setup( host, 7777, true );
    
    myPaddle = Paddle::create( glm::vec2( 50, getWindowHeight()/2), 10 );
    theirPaddle = Paddle::create( glm::vec2( getWindowWidth()- 50, getWindowHeight()/2 ), 10 );
}

void Pong_OSC_HostApp::mouseDown( MouseEvent event )
{
}

void Pong_OSC_HostApp::mouseMove(cinder::app::MouseEvent event)
{
    myPaddle->mouseMove(event);
}

void Pong_OSC_HostApp::keyDown(cinder::app::KeyEvent event)
{
    myPaddle->keyInput( event );
}

void Pong_OSC_HostApp::update()
{
    
    
    glm::vec2 dist1 = (*_Puck).mLoc - myPaddle->getPos();
    float dist1F = glm::length(dist1);
//    console() << dist1F << endl;
    if(dist1F <= (*_Puck).r + 5){
        (*_Puck).collision(1);
    }
    glm::vec2 dist2 = theirPaddle->getPos() - (*_Puck).mLoc;
    float dist2F = glm::length(dist2);
    if(dist2F <= (*_Puck).r + 5){
        (*_Puck).collision(2);
    }
    _Puck->update();
    
    //Update their paddle via osc
    while ( listener.hasWaitingMessages() )
    {
        osc::Message message;
        listener.getNextMessage( &message );
        
        
        
        
        if (message.getArgAsString(0) == "/paddlePos") {
            theirPaddle->updateOsc(message);
        }
    }
    
    osc::Message message1;
//    message1.setAddress("/paddlePos");
    message1.addStringArg("/paddlePos");
    message1.addFloatArg(myPaddle->getPos().y);
    sender.sendMessage(message1);
//    console() << message1.getArgAsString(0) << endl;
//    console() << message1.getArgAsFloat(1) << endl;
    
    osc::Message message2;
//    message2.setAddress("/puckPos");
    message2.addStringArg("/puckPos");
    message2.addFloatArg(_Puck->mLoc.x);
    message2.addFloatArg(_Puck->mLoc.y);
    sender.sendMessage(message2);
//    console() << message2.getArgAsString(0) << endl;
//    console() << message2.getArgAsFloat(1) << endl;
//    console() << message2.getArgAsFloat(2) << endl;
    
}

void Pong_OSC_HostApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    _Puck->draw();
    myPaddle->draw();
    theirPaddle->draw();
}

CINDER_APP( Pong_OSC_HostApp, RendererGl )
