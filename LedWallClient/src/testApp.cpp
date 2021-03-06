#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	led = NULL;
	ofxXmlSettings xml;
	int port = 2838;
	if(xml.loadFile("config.xml"))
	{
		xml.pushTag("DATA");
		port = xml.getValue("PORT",2838);
		xml.popTag();
		
	}
	receiver.setup(port);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofBackground(0);
    ofSetFrameRate(60);
	ofEnableAlphaBlending();
	
	if(	spi.connect())
	{
		ofLogNotice()<<"connected to SPI";
	}
	current = 0;
	next = 1;
	framerate = 12;
	lastFrameTime = 0;
}
void testApp::exit()
{
	if(led!=NULL)
	{
		led->clear(ofColor(0));
		
		spi.send(led->txBuffer);
	}
}
//--------------------------------------------------------------
void testApp::update(){
	float  dt = 1.0f / ofGetFrameRate();
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;

		receiver.getNextMessage(&m);
		
		// check for mouse moved message
		if(m.getAddress() == "/settings/length"){
			if(led==NULL)
			{
			numLED = m.getArgAsInt32(0);
			led = new ofxLEDsLPD8806(numLED);
			ofLogNotice("OSC") << " Set LED length as " << numLED;
			}
		}
		else if(m.getAddress() == "/settings/lastFrameTime"){
			sequenceTime[current] = m.getArgAsFloat(0);
			sequenceTime[next] = m.getArgAsFloat(1);			
		}
		else if(m.getAddress() == "/settings/framerate"){

			framerate = m.getArgAsInt32(0);
			ofLogNotice()<<"framerate " << framerate;
		}
		else if(m.getAddress() == "/led/cue/"){

			//arg 0: folder name
			//arg 1: transition duration
			//arg 2: curve
			//arg 3: animation duration
			//arg 4: loop
			
			int prev = current;
			current = next;
			next = prev;
			
			string folder = m.getArgAsString(0);

			sequence[next].unloadSequence();
			sequence[next].loadSequence(folder);
			sequence[next].setFrameRate(framerate);
				
			animation.animateFromTo( 0, 255 );
			animation.setDuration(m.getArgAsInt32(1));
			animation.setRepeatType( PLAY_ONCE );

			animation.setCurve((AnimCurve) (EASE_IN_EASE_OUT +  m.getArgAsInt32(2) ));
			
//			sequenceTime[next].animateFromTo( 0, 1 );
			int animationDur = m.getArgAsInt32(3);
			ofLogVerbose("Animation") << animationDur;
//			sequenceTime[next].setDuration(animationDur);
			isLoop = m.getArgAsInt32(4);
			ofLogVerbose("isLoop") << isLoop;
//			sequenceTime[next].setRepeatType( (isLoop)?LOOP:PLAY_ONCE );
//			sequenceTime[next].setCurve(LINEAR);
		}
		
		else if(m.getAddress() == "/settings/debug"){

			bool isDebug = m.getArgAsInt32(0);
			ofLogWarning("isLoop") << isDebug;
			ofSetLogLevel((isDebug)?OF_LOG_VERBOSE:OF_LOG_NOTICE);
		}
		else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			ofLogWarning("OSC") << " Unknown osc message " << msg_string;
		}
	}
	
	
	animation.update(dt);
//	sequenceTime[0].update(dt);
//	sequenceTime[1].update(dt);
//	ofLogVerbose() <<"sequenceTime :"<< sequenceTime[next].getCurrentValue();

	if(led!=NULL)
	{
		led->renderBuffer.begin();
		//drawing stuff
		ofPushStyle();
		ofSetColor(255,255);
		int width = led->renderBuffer.getWidth();
		int height = led->renderBuffer.getHeight();
		if(sequence[current].isLoaded())sequence[current].getFrameAtPercent(sequenceTime[current])->draw(0,0,width,height);
		ofPopStyle();
		ofPushStyle();
		
		ofSetColor(255,animation.getCurrentValue());
		if(sequence[next].isLoaded())sequence[next].getFrameAtPercent(sequenceTime[next])->draw(0,0,width,height);
		ofPopStyle();
		led->renderBuffer.end();
		led->encode();
//#define DEBUG
#ifdef DEBUG
		cout << "------------------------------------------" << endl;
		for(int i = 0 ; i< led->txBuffer.size() ;i++)
		{
			if(i>3 && i <led->txBuffer.size()-1)
			{
				cout << int(led->txBuffer[i]) << " | ";
				if(((i+4)%12)==0)
					cout << endl;
			}
		}
		cout << "------------------------------------------" << endl;
#endif
		//	sending stuff
		
		spi.send(led->txBuffer);
		
	}
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	if(ofGetLogLevel()==OF_LOG_VERBOSE)
	{
		if(led!=NULL)
		{
			ofPushMatrix();

			led->renderBuffer.draw(0,0,ofGetWidth(),5);
			led->encodedBuffer.draw(0,5,ofGetWidth(),5);
			ofPopMatrix();
		}
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}