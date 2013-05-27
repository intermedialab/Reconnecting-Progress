#pragma once

#include "ofMain.h"
#include "ofxStreamer.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

struct nodePoint {
    string hostname;
    ofColor color;
    int sendsToPort;
    int index;
};

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    nodePoint * nodes[3];
    nodePoint * nodeMe;
    
    std::string localHostname;
    
    int portNumber;
    
    ofxXmlSettings XML;
    bool settingsLoaded;

    ofxStreamerSender * senders[2];
  
    ofxStreamerReceiver * receivers[2];

    ofImage inputImage;
    
    ofVideoGrabber grabber;
    
    unsigned char * data ;
    
};
