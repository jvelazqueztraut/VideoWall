#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    std::string file = "settings.json";
    
    // Now parse the settings
    if (settings.open(file)){
        ofLogNotice("ofApp::setup") << settings.getRawString();
        
        if(settings.isMember("width") && settings.isMember("height"))
            ofSetWindowShape(settings["width"].asInt(),settings["height"].asInt());
        if(settings.isMember("fullscreen"))
            ofSetFullscreen(settings["fullscreen"].asBool());
        if(settings.isMember("framerate"))
            ofSetFrameRate(settings["framerate"].asInt());
        if(settings.isMember("background"))
            ofBackground(settings["background"][0].asInt(),settings["background"][1].asInt(),settings["background"][2].asInt());
        
        if(settings.isMember("players")){
            players.assign(settings["players"].size(), Player());
            
            for(int i=0; i<settings["players"].size();i++){
                Player& player(players[i]);
                player.setup(settings["players"][i]["width"].asInt(),settings["players"][i]["height"].asInt());
                player.setPos(settings["players"][i]["x"].asInt(),settings["players"][i]["y"].asInt());
                player.setBackground(ofColor(settings["players"][i]["background"][0].asInt(),settings["players"][i]["background"][1].asInt(),settings["players"][i]["background"][2].asInt()));
                
                for(int m=0; m<settings["players"][i]["contents"].size(); m++){
                    Media * media;
                    if(settings["players"][i]["contents"][m]["type"].asString() == "image"){
                        MediaImage * image = player.addContent<MediaImage>();
                        image->load(settings["players"][i]["contents"][m]["load"].asString());
                        media = image;
                    }
                    else if(settings["players"][i]["contents"][m]["type"].asString() == "video"){
                        MediaVideo * video = player.addContent<MediaVideo>();
                        video->load(settings["players"][i]["contents"][m]["load"].asString());
                        media = video;
                    }
                    else if(settings["players"][i]["contents"][m]["type"].asString() == "web"){
                        MediaWeb * web = player.addContent<MediaWeb>();
                        web->ofxAwesomium::setup(player.width,player.height);
                        web->loadURL(settings["players"][i]["contents"][m]["load"].asString());
                        media = web;
                    }
                    else if(settings["players"][i]["contents"][m]["type"].asString() == "youtube"){
                        MediaWeb * web = player.addContent<MediaWeb>();
                        web->ofxAwesomium::setup(player.width,player.height);
                        string videoId = settings["players"][i]["contents"][m]["load"].asString();
                        if(videoId.substr(0,2) == "PL") //Youtube playlist
                            web->loadURL("https://www.youtube.com/embed?listType=playlist&list="+ videoId + "&version=3&autoplay=1&loop=1&controls=0&modestbranding=1&showinfo=0");
                        else //Youtube video in loop mode
                            web->loadURL("https://www.youtube.com/embed/"+ videoId + "?version=3&playlist=" + videoId + "&autoplay=1&loop=1&controls=0&modestbranding=1&showinfo=0");
                        media = web;
                    }
                    else{
                        MediaImage * image = player.addContent<MediaImage>();
                        image->load("logo.png");
                        media = image;
                    }
                    if(settings["players"][i]["contents"][m]["loop"].asString() == "time"){
                        media->setup(LOOP_BY_TIME,settings["players"][i]["contents"][m]["loopParameter"].asFloat());
                    }
                    else if(settings["players"][i]["contents"][m]["loop"].asString() == "repetitions"){
                        media->setup(LOOP_BY_REP,settings["players"][i]["contents"][m]["loopParameter"].asInt());
                    }
                    else{
                        media->setup(PLAY_ONCE,0);
                    }
                }
                
                player.play();
            }
        }
    }
    else{
        ofLogError("ofApp::setup")  << "Failed to parse settings" << endl;
    }

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
