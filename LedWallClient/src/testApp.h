#pragma once

#include "ofMain.h"

#include "ofxImageSequence.h"
#include "ofxAnimatableFloat.h"
#include "ofxOsc.h"
#include "ofxLEDsLPD8806.h"
#include "ofxLPD8806.h"
#include "ofxXmlSettings.h"
class testApp : public ofBaseApp{

	public:
		void setup();
	void exit();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	ofDirectory dir;
	
	ofxOscReceiver receiver;
	ofxLEDsLPD8806 *led;
	ofxLPD8806 spi;
	ofxAnimatableFloat animation;
//	ofxAnimatableFloat sequenceTime[2];
	float sequenceTime[2];
	ofxImageSequence sequence[2];
	
	bool isLoop;
	
	int current,next;
	int numLED;
	int framerate;
	int lastFrameTime;
//	bool bUpdate;
};
