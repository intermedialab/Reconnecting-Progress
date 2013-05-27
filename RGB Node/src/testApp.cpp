#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetWindowTitle("Sender");
    ofSetWindowPosition(0, 0);
    ofSetFrameRate(70);

    ofLogLevel(OF_LOG_WARNING);

    // set own hostname

    for (int i = 0; i < 100; i++) {
        FILE* stream = popen( "hostname", "r" );
        ostringstream output;
        
        while( !feof( stream ) && !ferror( stream ))
        {
            char buf[128];
            int bytesRead = fread( buf, 1, 128, stream );
            output.write( buf, bytesRead );
        }
        localHostname = output.str();
        if(localHostname != ""){
            // get rid of the newline and store in localHostname
            localHostname = localHostname.substr(0, localHostname.length()-1);
            break;
        }
    }
    
    
    cout << localHostname << endl;
    
    // load settings
    
    settingsLoaded = XML.loadFile("globalSettings.xml");
    
    portNumber = XML.getValue("global:network:port", 13000);

    //sender.setup(640, 480, "jive.local", 1234);
    //sender.setup(800, 800.*9./16., localHostname, portNumber);
    
    sender.setup(800, 800.*9./16.);
    
    data = (unsigned char*) malloc(sizeof(char)* sender.width * sender.height * 3*10);
    
    grabber.initGrabber(sender.width, sender.height);
    
    int receiverNumber = 0;
    
    for(int i = 0; i < 3; i++){
        nodes[i] = new nodePoint;
        
        nodes[i]->hostname = XML.getValue("nodes:node" + ofToString(i+1) + ":hostname", ofToString(i+1)+".local");
        nodes[i]->color.r = XML.getValue("nodes:node" + ofToString(i+1) + ":color:r", 255);
        nodes[i]->color.g = XML.getValue("nodes:node" + ofToString(i+1) + ":color:g", 255);
        nodes[i]->color.b = XML.getValue("nodes:node" + ofToString(i+1) + ":color:b", 255);
        
        cout << nodes[i]->hostname << " = " << localHostname << endl;
        
        
        
        if(ofToString(nodes[i]->hostname).compare(ofToString(localHostname)) == 0){
            nodeMe = nodes[i];
            cout << "i am " << nodeMe->hostname << endl;
        } else {
            receivers[receiverNumber] = new ofxStreamerReceiver;
            receivers[receiverNumber]->setup(portNumber,nodes[i]->hostname);
            receiverNumber++;
        
        }
    }
    //oscReceiver.setup(9999);
    
    latency = 0;
    if(nodeMe){
        cout << nodeMe->color << endl;
    }
    
}

//--------------------------------------------------------------
void testApp::update(){
    grabber.update();
    
    if(sendPing){
        sendPing = false;
        sendPingTime = ofGetElapsedTimeMillis();
        
        
        unsigned char bytes[sender.width*sender.height*3];
        for(int i=0;i<sender.width*sender.height*3;i++){
            bytes[i] = 255;
        }
        
        
        sender.encodeFrame(bytes, sender.width*sender.height*3);
        sender.sendFrame();
        
    }
    else if(grabber.isFrameNew()){
        
        ofBuffer buffer;
        buffer.set((char*)data, sender.width*sender.height * 3);
        
        inputImage.setFromPixels(data, sender.width, sender.height, OF_IMAGE_COLOR);
        
        sender.encodeFrame(grabber.getPixels(), sender.width * sender.height * 3);
        //    x264Encoder.encodeFrame(data, 640 * 480 * 3);
        
        sender.sendFrame();
   }

    for (int i = 0; i < 2 ; i++) {
        if(receivers[i]){
            receivers[i]->update();
        }
    }
    
    
   /* if(oscReceiver.hasWaitingMessages()){
        ofxOscMessage msg;
        oscReceiver.getNextMessage(&msg);
        
        cout<<"Got message "<<msg.getAddress()<<endl;
        
        latency = ofGetElapsedTimeMillis() -  sendPingTime;
        
        cout<<"Diff "<<latency<<endl;
    }*/

}

//--------------------------------------------------------------
void testApp::draw(){
    ofClear(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    for (int i = 0; i < 3; i++){
        nodePoint * currentNode = nodes[i];
        
        ofFill();
        ofSetColor(currentNode->color.r, currentNode->color.g, currentNode->color.b);
        
        if(currentNode == nodeMe){
            grabber.draw(0, 0, grabber.width, grabber.height);
        } else {
            for (int j = 0; j < 2 ; j++) {
                if (receivers[j]) {
                if(receivers[j]->host.compare(currentNode->hostname) == 0){
                    receivers[j]->draw(0, 0, receivers[j]->width, receivers[j]->height);
                }
                }
            }
        }
    }
    
    if(ofGetWidth() > ofGetHeight()){
        // this screen is landscape
        
    } else {
        // this screen is portrait
        
    }
    
    
    /*
    int y = 15;
    int x = 650;
    
    ofDrawBitmapString("Streamer Sender Example", 650, y);
    ofDrawBitmapString("Frame Num: \t\t"+ofToString(sender.frameNum), 650, y+=20);
    ofDrawBitmapString("Frame Rate: "+ofToString(sender.frameRate,1)+" fps", 650, y+=15);
    ofDrawBitmapString("bitrate: "+ofToString(sender.bitrate)+" kbits/s", 650, y+=15);
    ofDrawBitmapString("URL: "+sender.url, 650, y+=35);
    ofDrawBitmapString("Preset: "+sender.preset, 650, y+=15);
    
    if(latency){
        ofDrawBitmapString("Latency: "+ofToString(latency)+" ms", 650, y+=15);

    } else {
        ofDrawBitmapString("Latency: Press any key", 650, y+=15);
    }
     */
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    sendPing = true;
    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == 1){
        XML.setValue("nodes:node1:hostname", localHostname);
        XML.saveFile();
    }
    if(key == 2){
        XML.setValue("nodes:node2:hostname", localHostname);
        XML.saveFile();
    }
    if(key == 3){
        XML.setValue("nodes:node3:hostname", localHostname);
        XML.saveFile();
    }
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