#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//	xml.addTag("DATA");
	//	if(xml.pushTag("DATA"))
	//	{
	//
	//
	//		for(int i = 0 ; i < 5 ; i++)
	//		{
	//			xml.addTag("DEVICE");
	//			if(xml.pushTag("DEVICE",i))
	//			{
	//				xml.addValue("HOST", "127.0.0.1");
	//				xml.addValue("PORT", 2838);
	//				xml.popTag();
	//			}
	//		}
	//		xml.popTag();
	//	}
	//	xml.saveFile("config.xml");
	ofSetLogLevel(OF_LOG_VERBOSE);
	deviceName = "/dev/ttyUSB0" ;
	baudrate = 9600;

	if(xml.loadFile("config.xml"))
	{
		xml.pushTag("DATA");
		numDevice = xml.getNumTags("DEVICE");
		ofLogVerbose("numDevice") << numDevice;
		sender = new ofxOscSender[numDevice];
		cue = new Cue[numDevice];
		for(int i = 0 ; i < numDevice ; i++)
		{
			
			if(xml.pushTag("DEVICE",i))
			{
				sender[i].setup(xml.getValue("HOST", "127.0.0.1"), xml.getValue("PORT", 2838));
				
				xml.popTag();
			}
			if(xml.pushTag("SERIAL"))
			{
				deviceName = xml.getValue("NAME", "/dev/ttyUSB0");
				baudrate = xml.getValue("BAUDRATE", 9600);
				xml.popTag();
			}
		}
		//			xml.popTag();
		
	}
	bSerialInited = serial.setup(deviceName, baudrate);
	if(!bSerialInited)
	{
		ofLogError("Serial") << "Device Name may not corret "<< endl << "Use \"ls /dev/tty*\" to checkyour available rs232 device";
	}
	
	timeConunt = 0;
	keyPressed(OF_KEY_RETURN);
	ofSetFrameRate(25);
}

//--------------------------------------------------------------
void testApp::update(){
	
	float dt = 1.0f/ofGetFrameRate();
	for(int i = 0 ; i < numDevice ; i++)
	{
		cue[i].update(dt);
		ofxOscBundle b;
		
		ofxOscMessage m;
		m.setAddress("/settings/lastFrameTime");
		//this is tricky you end to invert the next cue point and curent cue point here
		m.addFloatArg(cue[i].animatable[cue[i].next].getCurrentValue());		
		m.addFloatArg(cue[i].animatable[cue[i].current].getCurrentValue());

		b.addMessage(m);
			sender[i].sendBundle(b);
	}
	if(bSerialInited)
	{
		if(serial.available())
		{
			unsigned char bytesReturned[1];
			
			
			while( serial.readBytes( bytesReturned, 1) > 0){
				ofLogVerbose("Serial") << "bytesReturned : " <<bytesReturned[0];
				parseCue(int(bytesReturned[0]));
			};
		}
	}
	else
	{
		int diff = ofGetElapsedTimeMillis() - timeConunt;
		if(diff>10000)
		{
			bSerialInited = serial.setup(deviceName, baudrate);
			if(!bSerialInited)
			{
				ofLogError("Serial") << "Device Name may not corret "<< endl << "\t Use \"ls /dev/tty*\" to check your available rs232 device";
			}
			timeConunt = ofGetElapsedTimeMillis();
			
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
}
void testApp::initClient()
{
	for(int i = 0 ; i < numDevice ; i++)
	{
		if(xml.pushTag("DEVICE",i))
		{
			ofxOscBundle b;
			
			ofxOscMessage m;
			m.setAddress("/settings/length");
			
			m.addIntArg(xml.getValue("LENGTH", 32));
			b.addMessage(m);
			
			
			sender[i].sendBundle(b);
		}
		xml.popTag();
	}
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key)
	{
		case OF_KEY_RETURN:
		{
			initClient();
		}
			break;
		case '1':
			parseCue(0);
			break;
		case '2':
			parseCue(1);
			break;
		case '3':
			parseCue(2);
			break;
		case 'd':
		{
			bDebug = !bDebug;
			ofxOscBundle b;
			
			ofxOscMessage m;
			m.setAddress("/settings/debug");
			
			m.addIntArg(bDebug);
			b.addMessage(m);
			
			for(int i = 0 ; i < numDevice ; i++)
			{
				
				sender[i].sendBundle(b);
			}
		}
			break;
			
	}
}
void testApp::parseCue(int _cue)
{
	initClient();
	//	keyPressed(OF_KEY_RETURN);
	int framerate = xml.getValue("FRAMERATE", 12);
	if(xml.pushTag("CUES"))
	{
		if(xml.pushTag("CUE",_cue))
		{
			
			ofxOscBundle b;
			
			ofxOscMessage m;
			m.setAddress("/led/cue/");
			//arg 0: folder name
			//arg 1: transition duration
			//arg 2: curve
			//arg 3: animation duration
			m.addStringArg(xml.getValue("FILE", "sequence"));
			m.addIntArg(xml.getValue("TRANSITION_DURATION", 1));
			m.addIntArg(xml.getValue("EASING", 0));
			float animationDur = xml.getValue("DURATION", 0);
			m.addIntArg(animationDur);
			bool bLoop = xml.getValue("LOOP", 0);
			m.addIntArg(bLoop);
			b.addMessage(m);
			
			ofxOscMessage m2;
			m2.setAddress("/settings/framerate");
			m2.addIntArg(framerate);
			b.addMessage(m2);
			
			if(xml.pushTag("DEVICES"))
			{
				int numTag = xml.getNumTags("ID");
				for(int i = 0 ; i < numTag ; i++)
				{
					int id = xml.getValue("ID",0,i);
					if(id<numDevice && id>-1)
					{
						cue[id].swap();
						cue[id].animatable[cue[id].current].animateFromTo( 0, 1 );
						cue[id].animatable[cue[id].current].setDuration(animationDur);
						cue[id].animatable[cue[id].current].setRepeatType( (bLoop)?LOOP:PLAY_ONCE );
						cue[id].animatable[cue[id].current].setCurve(LINEAR);

						sender[id].sendBundle(b);
						ofLogVerbose("Device "+ofToString(id)) << "sent";
					}
				}
				xml.popTag();
			}
			
			xml.popTag();
		}
		xml.popTag();
	}
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