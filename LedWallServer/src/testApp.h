#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxAnimatableFloat.h"
class Cue{
public:
	Cue()
	{
		current = 0;
		next = 1;
		animatable = new ofxAnimatableFloat[2];
	}
	void update(float dt)
	{
		
		animatable[current].update(dt);
		animatable[next].update(dt);
	}
	void swap()
	{
		int prev = current;
		current = next;
		next = prev;
	}
	int next,current;
ofxAnimatableFloat *animatable;
};

class testApp : public ofBaseApp{

	public:
		void setup();
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
	ofxOscSender *sender;
	Cue *cue;
	
	ofxXmlSettings xml;
	int numDevice;
	void parseCue(int _cue);
	void  initClient();
	bool bDebug;
	ofSerial serial;
	bool bSerialInited;
	int timeConunt ;
	string deviceName;
	int baudrate;
	
};
