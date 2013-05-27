#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetWindowTitle("Receiver");
    ofSetWindowPosition(0, 540);

    ofLogLevel(OF_LOG_WARNING);
    receiverR.setup(1234);
    receiverG.setup(5000,"233.99.61.106");
    receiverB.setup(1236);
    
    oscSender.setup("localhost.local", 9999);
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    receiverR.update();
    receiverG.update();
    receiverB.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    //ofBackgroundGradient(ofColor(28,78,28), ofColor(14,25,14),OF_GRADIENT_LINEAR);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofClear(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
   // if(receiver.isFrameNew()) {
    ofSetColor(255,0,0);
    receiverR.draw(0, 0);
    ofSetColor(0, 255,0);
    receiverG.draw(0, 0);
    ofSetColor(0, 0, 255);
    receiverB.draw(0, 0);
    
      /*  if(receiver.getPixels()[0] == 255){
            ofxOscMessage msg;
            msg.setAddress("/streamer/hello");
            oscSender.sendMessage(msg);

        }*/
   // }
    
    int y = 15;
    int x = 650;
    
    /*
    ofDrawBitmapString("Streamer Receiver Example", 650, y);
    ofDrawBitmapString("Frame Num: \t\t"+ofToString(receiver.frameNum), 650, y+=20);
    ofDrawBitmapString("Frame Rate: "+ofToString(receiver.frameRate,1)+" fps", 650, y+=15);
    ofDrawBitmapString("bitrate: "+ofToString(receiver.bitrate)+" kbits/s", 650, y+=15);
    ofDrawBitmapString("URL: "+receiver.url, 650, y+=35);
    */
}

void testApp::exit(){
    receiverR.close();
    receiverG.close();
    receiverB.close();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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