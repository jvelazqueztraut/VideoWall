#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    players.push_back(Player());
    players.back().setup(300,200);
    players.back().setBackground(ofColor(255));
    
    MediaImage * media0 = players.back().addContent<MediaImage>(LOOP_BY_TIME,2);
    media0->load("test.png");
    
    MediaImage * media1 = players.back().addContent<MediaImage>(LOOP_BY_TIME,2);
    media1->load("logo.png");

    time=ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){
    float t = ofGetElapsedTimef();
    float dt = t - time;
    time = t;
    
    for(int i=0; i<players.size(); i++){
        players[i].update(dt);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    for(int i=0; i<players.size(); i++){
        players[i].draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    players.front().setPos(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
