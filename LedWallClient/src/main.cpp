#include "ofMain.h"
#include "testApp.h"
#ifdef TARGET_LINUX_ARM
#include "ofGLES2Renderer.h"
#endif
#include "testApp.h"

//========================================================================
int main( ){
#ifdef TARGET_LINUX_ARM
	ofGLES2Renderer * renderer = new ofGLES2Renderer();
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(renderer));
#endif
	ofSetupOpenGL(1024,768,OF_WINDOW);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());
	
}
