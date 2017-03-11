#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Pong_OSC_GuestApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void Pong_OSC_GuestApp::setup()
{
}

void Pong_OSC_GuestApp::mouseDown( MouseEvent event )
{
}

void Pong_OSC_GuestApp::update()
{
}

void Pong_OSC_GuestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( Pong_OSC_GuestApp, RendererGl )
