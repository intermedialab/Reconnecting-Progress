#include "testApp.h"

int width = 800;
int height = 600;

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
    
    portNumber = XML.getValue("global:network:port", 1234);
    
    data = (unsigned char*) malloc(sizeof(char)* width * height * 3*10);
    
    grabber.initGrabber(width, height);
    
    int receiverNumber = 0;
    
    for(int i = 0; i < 3; i++){
        nodes[i] = new nodePoint;
        
        nodes[i]->hostname = XML.getValue("nodes:node" + ofToString(i+1) + ":hostname", ofToString(i+1)+".local");
        nodes[i]->color.r = XML.getValue("nodes:node" + ofToString(i+1) + ":color:r", 255);
        nodes[i]->color.g = XML.getValue("nodes:node" + ofToString(i+1) + ":color:g", 255);
        nodes[i]->color.b = XML.getValue("nodes:node" + ofToString(i+1) + ":color:b", 255);
        nodes[i]->index = i;
        
        if(ofToString(nodes[i]->hostname).compare(ofToString(localHostname)) == 0){
            nodeMe = nodes[i];
            cout << "I am " << nodeMe->hostname << endl;
        }
    }
    
    sender.setup(width, height,"localhost", portNumber);

    for(int i = 0; i < 3; i++){

        if(ofToString(nodes[i]->hostname).compare(ofToString(localHostname)) != 0){
        
            receivers[receiverNumber] = new ofxStreamerReceiver();
            receivers[receiverNumber]->setup(portNumber,"udp://" + nodes[i]->hostname); // remember udp prefix on receiver!!!

//            senders[receiverNumber] = new ofxStreamerSender();
//            senders[receiverNumber]->setup(width, height,nodeMe->hostname, portNumber); // no prefix here

            receiverNumber++;
                        
        }
    }    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    grabber.update();
    
    if(grabber.isFrameNew()){
        
        ofBuffer buffer;
        buffer.set((char*)data, width*height * 3);
        inputImage.setFromPixels(data, width, height, OF_IMAGE_COLOR);
        
/*        for (int j = 0; j < 2 ; j++) {
            senders[j]->encodeFrame(grabber.getPixels(), width * height * 3);
            senders[j]->sendFrame();
        }
*/
        sender.encodeFrame(grabber.getPixels(), width * height * 3);
        sender.sendFrame();
    }
    
    for (int i = 0; i < 2 ; i++) {
        if(receivers[i]){
            if(!receivers[i]->isConnected() && ofGetElapsedTimeMillis() % 1000 < 20 ){
                receivers[i]->setup(receivers[i]->port, receivers[i]->host);
            }
            receivers[i]->update();
        }
    }
        
}

//--------------------------------------------------------------
void testApp::draw(){
    ofClear(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    int y = 15;
    int x = width + 20;

    for (int i = 0; i < 3; i++){
        nodePoint * currentNode = nodes[i];
        
        ofFill();
        ofSetColor(currentNode->color.r, currentNode->color.g, currentNode->color.b);
        
        if(currentNode == nodeMe){
            grabber.draw(0, 0, width, height);
            
            /*
            for (int j = 0; j < 2 ; j++) {
            ofDrawBitmapString("Streamer Sender Example", x, y+=35);
            ofDrawBitmapString("Frame Num: \t\t"+ofToString(senders[j]->frameNum), x, y+=20);
            ofDrawBitmapString("Frame Rate: "+ofToString(senders[j]->frameRate,1)+" fps", x, y+=15);
            ofDrawBitmapString("bitrate: "+ofToString(senders[j]->bitrate)+" kbits/s", x, y+=15);
            ofDrawBitmapString("URL: "+senders[j]->url, x, y+=35);
            ofDrawBitmapString("Preset: "+senders[j]->preset, x, y+=35);
            }
             */
            ofDrawBitmapString("Streamer Sender Example", x, y+=35);
            ofDrawBitmapString("Frame Num: \t\t"+ofToString(sender.frameNum), x, y+=20);
            ofDrawBitmapString("Frame Rate: "+ofToString(sender.frameRate,1)+" fps", x, y+=15);
            ofDrawBitmapString("bitrate: "+ofToString(sender.bitrate)+" kbits/s", x, y+=15);
            ofDrawBitmapString("URL: "+sender.url, x, y+=35);
            ofDrawBitmapString("Preset: "+sender.preset, x, y+=35);

            
        } else {
            for (int j = 0; j < 2 ; j++) {
                if (receivers[j]) {
                    if((receivers[j]->host).find(currentNode->hostname) != std::string::npos){
                        if (receivers[j]->isConnected()) {
                            receivers[j]->draw(0, 0, width, height);
                            cout << receivers[j]->host << " is connected"
                            << endl;
                        } else {
                            ofDrawBitmapString("connecting to " + ofToString(receivers[j]->host),  x, y+=35);
                        }
                        
                        ofDrawBitmapString("Streamer Receiver Example",  x, y+=35);
                        ofDrawBitmapString("Frame Num: \t\t"+ofToString(receivers[j]->frameNum), x, y+=20);
                        ofDrawBitmapString("Frame Rate: "+ofToString(receivers[j]->frameRate,1)+" fps", x, y+=15);
                        ofDrawBitmapString("bitrate: "+ofToString(receivers[j]->bitrate)+" kbits/s", x, y+=15);
                        ofDrawBitmapString("URL: "+receivers[j]->url, x, y+=35);
                    }

                } else {
                    ofDrawBitmapString("initialising " + ofToString(receivers[j]->host),  x, y+=35);
                }
            }
        }
    }
    
    if(ofGetWidth() > ofGetHeight()){
        // this screen is landscape
        
    } else {
        // this screen is portrait
        
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
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