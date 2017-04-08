#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    players.assign(2, Player());
    
    Player& player0(players[0]);
    player0.setup(300,200);
    player0.setBackground(ofColor(255));
    
    MediaImage * media0 = player0.addContent<MediaImage>();
    media0->load("test.png");
    media0->setup(LOOP_BY_TIME,2);
    
    MediaVideo * media1 = player0.addContent<MediaVideo>();
    media1->load("fingers.mov");
    media1->setup(LOOP_BY_REP,2);
    
    player0.play();
    
    Player& player1(players[1]);
    player1.setup(200,300);
    player1.setBackground(ofColor(255));
    
    MediaImage * media2 = player1.addContent<MediaImage>();
    media2->load("logo.png");
    media2->setup(LOOP_BY_TIME,2);
    
    MediaVideo * media3 = player1.addContent<MediaVideo>();
    media3->load("fingers.mov");
    media3->setup(LOOP_BY_TIME,3);
    
    player1.play();

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
