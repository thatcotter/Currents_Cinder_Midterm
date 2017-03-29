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
    
    float score1;
    float score2;
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
    
    //override ip
//    host = "149.31.131.125";
    
    sender.setup( host, 7777, true );
    
    myPaddle = Paddle::create( glm::vec2( 50, getWindowHeight()/2), 10 );
    theirPaddle = Paddle::create( glm::vec2( getWindowWidth()- 50, getWindowHeight()/2 ), 10 );
    
    score1 = 0.f;
    score2 = 0.f;
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
        cout << "New Message!" << endl;
        
        if (message.getArgAsString(0) == "/player/position") {
            theirPaddle->updateOsc(message);
        }
    }
    
    osc::Message message1;
//    message1.setAddress("/paddlePos");
    message1.addStringArg("/player/position");
//    message1.addFloatArg(myPaddle->getPos().x);
    message1.addFloatArg(myPaddle->getPos().y);
    sender.sendMessage(message1);
//    console() << message1.getArgAsString(0) << endl;
//    console() << message1.getArgAsFloat(1) << endl;
    
    osc::Message message2;
//    message2.setAddress("/puckPos");
    message2.addStringArg("/ball/position");
    message2.addFloatArg(_Puck->mLoc.x);
    message2.addFloatArg(_Puck->mLoc.y);
    sender.sendMessage(message2);
    
    osc::Message message3;
    message3.addStringArg("/score");
    message3.addFloatArg(score1);
    message3.addFloatArg(score2);
    sender.sendMessage(message3);
//    cout << message3.getArgAsFloat(1) << endl;
//    cout << message3.getArgAsFloat(2) << endl;
    
    
    
//    osc::Message message4;
//    message3.addStringArg("/score2");
//    message3.addFloatArg(score2);
//    sender.sendMessage(message4);
//    cout << message4.getArgAsFloat(1) << endl;
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
    
    if (_Puck->mLoc.x < 0-_Puck->r){
        score1 ++;
        
    } else if (_Puck->mLoc.x > cinder::app::getWindowWidth()+_Puck->r){
        score2 ++;
    }
    ci::gl::drawString(std::to_string(int(score2)), glm::vec2(20.f, 50.f));
    ci::gl::drawString(std::to_string(int(score1)), glm::vec2(ci::app::getWindowWidth()-20.f, 50.f));
    
}

CINDER_APP( Pong_OSC_HostApp, RendererGl )
