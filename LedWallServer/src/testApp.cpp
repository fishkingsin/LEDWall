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
	if(xml.loadFile("config.xml"))
	{
		xml.pushTag("DATA");
			numDevice = xml.getNumTags("DEVICE");
			sender = new ofxOscSender[numDevice];
			for(int i = 0 ; i < numDevice ; i++)
			{
				if(xml.pushTag("DEVICE",i))
				{
					sender[i].setup(xml.getValue("HOST", "127.0.0.1"), xml.getValue("PORT", 2838));
					xml.popTag();
				}
			}
//			xml.popTag();
		
	}
	keyPressed(OF_KEY_RETURN);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key)
	{
		case OF_KEY_RETURN:
		{
			ofxOscBundle b;
			
			ofxOscMessage m;
			m.setAddress("/settings/length");
			
			m.addIntArg(160);
			b.addMessage(m);
			
			for(int i = 0 ; i < numDevice ; i++)
			{
				
				sender[i].sendBundle(b);
			}
		}
			break;
		case '1':
			parseCue(0);
			break;
		case '2':
			parseCue(1);
			break;
		case 'd':
		{
			ofxOscBundle b;
			
			ofxOscMessage m;
			m.setAddress("/settings/debug");

			m.addIntArg(1);
			b.addMessage(m);

			for(int i = 0 ; i < numDevice ; i++)
			{
				
					sender[i].sendBundle(b);
			}
		}
			break;
				
	}
}
void testApp::parseCue(int cue)
{
	if(xml.pushTag("CUES"))
	{
		if(xml.pushTag("CUE",cue))
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
			m.addIntArg(xml.getValue("DURATION", 0));
			b.addMessage(m);
;
			if(xml.pushTag("DEVICES"))
			{
				int numTag = xml.getNumTags("ID");
				for(int i = 0 ; i < numTag ; i++)
				{
					int id = xml.getValue("ID",-1);
					if(id<numDevice && id-1)
					{
						sender[id].sendBundle(b);
						ofLogVerbose("Device"+ofToString(id)) << "sent";
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