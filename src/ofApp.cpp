#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    fullscreen=false;
    screen.allocate(ofGetWidth(),ofGetHeight());
    background.set(0);
    
    ofx::HTTP::SimplePostServerSettings settings;
    // Many other settings are available.
    settings.setPort(7890);
    // Apply the settings.
    server.setup(settings);
    // The client can listen for POST form and multi-part upload events.
    // User be aware, these methods are called from other threads.
    // The user is responsible for protecting shared resources (e.g. ofMutex).
    server.getPostRoute().registerPostEvents(this);
    
    std::string file = "configuration.json";
    if (configuration.open(file)){
        applyConfiguration();
    }
    else{
        ofLogError("ofApp::setup")  << "Failed to parse configuration file" << endl;
    }
    newConfiguration = false;
    
    // Start the server.
    server.start();
    
    ofHideCursor();
    
    time=ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::applyConfiguration(bool save){
    
    if(configuration.isMember("fullscreen")){
        fullscreen=configuration["fullscreen"].asBool();
        ofSetFullscreen(fullscreen);
    }
    ofSleepMillis(500);
    
    if(!fullscreen && configuration.isMember("width") && configuration.isMember("height")){
        ofSetWindowShape(ofToInt(configuration["width"].asString()),ofToInt(configuration["height"].asString()));
    }
    
    if(configuration.isMember("background")){
        background.set(ofToInt(configuration["background"]["r"].asString()),ofToInt(configuration["background"]["g"].asString()),ofToInt(configuration["background"]["b"].asString()));
    }
    
    screen.allocate(ofGetWidth(),ofGetHeight());
    
    if(configuration.isMember("players")){
        
        float width = ofGetWidth();
        float height = ofGetHeight();
        
        ///////////////////////////////////////////////////
        int rows = ofToInt(configuration["rows"].asString());
        int cols = ofToInt(configuration["cols"].asString());
        bool taken[rows][cols];
        for(int r=0; r<rows; r++){
            for(int c=0; c<cols; c++){
                taken[r][c]=false;
            }
        }
        vector<PlayerSize> newPlayers;
        for(int r=0; r<rows; r++){
            for(int c=0; c<cols; c++){
                if(!taken[r][c]){
                    int z = configuration["zones"][r][c].asInt();
                    taken[r][c] = true;
                    newPlayers.push_back(PlayerSize());
                    newPlayers.back().playerId = z;
                    newPlayers.back().x = c;
                    newPlayers.back().y = r;
                    newPlayers.back().width = 1;
                    newPlayers.back().height = 1;
                    newPlayers.back().added = false;
                    for(int rr=r; rr<rows; rr++){
                        for(int cc=c; cc<cols; cc++){
                            if(!taken[rr][cc] && (z==configuration["zones"][rr][cc].asInt())){
                                if(((rr-1)>=0 && z==configuration["zones"][rr-1][cc].asInt()) || ((cc-1)>=0 && z==configuration["zones"][rr][cc-1].asInt())){
                                    taken[rr][cc] = true;
                                    newPlayers.back().width = cc-c+1;
                                    newPlayers.back().height = rr-r+1;
                                }
                            }
                        }
                    }
                }
            }
        }
        ofLogNotice("ofApp::applyConfiguration") << "Adding players: " << newPlayers.size();
        ///////////////////////////////////////////////////
        
        webs.clear();
        players.clear();
        players.assign(newPlayers.size(), Player());
        
        for(int p=0; p<newPlayers.size(); p ++){
            Player& player(players[p]);
            player.setup(newPlayers[p].width*(width/cols),newPlayers[p].height*(height/rows));
            player.setPos(newPlayers[p].x*(width/cols),newPlayers[p].y*(height/rows));
            for(int i=0; !newPlayers[p].added && i<configuration["players"].size();i++){
                if(newPlayers[p].playerId == configuration["players"][i]["id"].asInt()){
                    ofLogNotice("ofApp::applyConfiguration") << "Pos:"<< player.pos.x << "," << player.pos.y << " Size:"<< player.width << "," << player.height;
                    player.setBackground(ofColor(ofToInt(configuration["players"][i]["background"]["r"].asString()),ofToInt(configuration["players"][i]["background"]["g"].asString()),ofToInt(configuration["players"][i]["background"]["b"].asString())));
                    for(int m=0; m<configuration["players"][i]["contents"].size(); m++){
                        Media * media;
                        if(configuration["players"][i]["contents"][m]["type"].asString() == "image"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding image content.";
                            MediaImage * image = player.addContent<MediaImage>();
                            image->load(configuration["players"][i]["contents"][m]["load"].asString());
                            media = image;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"].asString() == "video"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding video content.";
                            MediaVideo * video = player.addContent<MediaVideo>();
                            video->load(configuration["players"][i]["contents"][m]["load"].asString());
                            media = video;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"].asString() == "web"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding web content.";
                            MediaWeb * web = player.addContent<MediaWeb>();
                            web->ofxAwesomium::setup(player.width,player.height);
                            web->load(configuration["players"][i]["contents"][m]["load"].asString(),configuration["players"][i]["contents"][m]["reload"].asBool());
                            WebPlayer webPlayer; webPlayer.web = web; webPlayer.player = &player;
                            webs.push_back(webPlayer);
                            media = web;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"].asString() == "youtube"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding youtube content";
                            MediaWeb * web = player.addContent<MediaWeb>();
                            web->ofxAwesomium::setup(player.width,player.height);
                            string videoId = configuration["players"][i]["contents"][m]["load"].asString();
                            if(videoId.substr(0,2) == "PL") //Youtube playlist
                                web->load("https://www.youtube.com/embed?listType=playlist&list="+ videoId + "&version=3&autoplay=1&loop=1&controls=0&modestbranding=1&showinfo=0");
                            else //Youtube video in loop mode
                                web->load("https://www.youtube.com/embed/"+ videoId + "?version=3&playlist=" + videoId + "&autoplay=1&loop=1&controls=0&modestbranding=1&showinfo=0");
                            WebPlayer webPlayer; webPlayer.web = web; webPlayer.player = &player;
                            webs.push_back(webPlayer);
                            media = web;
                        }
                        else{
                            ofLogNotice("ofApp::applyConfiguration") << "Adding NULL player.";
                            MediaImage * image = player.addContent<MediaImage>();
                            image->load("logo.png");
                            media = image;
                        }
                        if(configuration["players"][i]["contents"][m]["loop"].asString() == "time"){
                            media->setup(LOOP_BY_TIME,ofToFloat(configuration["players"][i]["contents"][m]["time"].asString()));
                        }
                        else if(configuration["players"][i]["contents"][m]["loop"].asString() == "repetitions"){
                            media->setup(LOOP_BY_REP,ofToInt(configuration["players"][i]["contents"][m]["repetitions"].asString()));
                        }
                        else{
                            media->setup(PLAY_ONCE,0);
                        }
                    }
                    
                    player.play();
                    newPlayers[p].added = true;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if(newConfiguration){
        applyConfiguration(true);
        newConfiguration = false;
    }
    
    float t = ofGetElapsedTimef();
    float dt = t - time;
    time = t;
    
    for(int i=0; i<players.size(); i++){
        players[i].update(dt);
    }
    
    screen.begin();
    ofClear(background);
    ofSetColor(255);
    for(int i=0; i<players.size(); i++){
        players[i].draw();
    }
    screen.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    screen.draw(0,0);
    ofDrawBitmapStringHighlight("See " + server.getURL(), 10, 16);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='f'){
        ofToggleFullscreen();
        ofShowCursor();
    }
    for(int i=0; i<webs.size(); i++){
        webs[i].web->ofxAwesomium::keyPressed(key);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    for(int i=0; i<webs.size(); i++){
        webs[i].web->ofxAwesomium::keyReleased(key);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for(int i=0; i<webs.size(); i++){
        ofPoint webPos(webs[i].player->pos);
        webs[i].web->ofxAwesomium::mouseMoved(x-webPos.x,y-webPos.y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    for(int i=0; i<webs.size(); i++){
        ofPoint webPos(webs[i].player->pos);
        webs[i].web->ofxAwesomium::mouseDragged(x-webPos.x,y-webPos.y,button);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for(int i=0; i<webs.size(); i++){
        ofPoint webPos(webs[i].player->pos);
        webs[i].web->ofxAwesomium::mousePressed(x-webPos.x,y-webPos.y,button);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    for(int i=0; i<webs.size(); i++){
        ofPoint webPos(webs[i].player->pos);
        webs[i].web->ofxAwesomium::mouseReleased(x-webPos.x,y-webPos.y,button);
    }
}

//--------------------------------------------------------------
void ofApp::onHTTPPostEvent(ofx::HTTP::PostEventArgs& args){
    ofLogNotice("ofApp::onHTTPPostEvent") << "Data: " << args.getBuffer().getText();
    // Now try to parse the configuration
    if (configuration.parse(args.getBuffer().getText())){
        newConfiguration = true;
    }
    else{
        ofLogError("ofApp::onHTTPPostEvent")  << "Failed to parse configuration" << endl;
    }
}


//--------------------------------------------------------------
void ofApp::onHTTPFormEvent(ofx::HTTP::PostFormEventArgs& args){
    ofLogNotice("ofApp::onHTTPFormEvent") << "";
    ofx::HTTP::HTTPUtils::dumpNameValueCollection(args.getForm(), ofGetLogLevel());
}

//--------------------------------------------------------------
void ofApp::onHTTPUploadEvent(ofx::HTTP::PostUploadEventArgs& args){
    std::string stateString = "";
    
    switch (args.getState())
    {
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_STARTING:
            stateString = "STARTING";
            break;
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_PROGRESS:
            stateString = "PROGRESS";
            break;
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_FINISHED:
            stateString = "FINISHED";
            break;
    }
    
    ofLogNotice("ofApp::onHTTPUploadEvent") << "";
    ofLogNotice("ofApp::onHTTPUploadEvent") << "         state: " << stateString;
    ofLogNotice("ofApp::onHTTPUploadEvent") << " formFieldName: " << args.getFormFieldName();
    ofLogNotice("ofApp::onHTTPUploadEvent") << "orig. filename: " << args.getOriginalFilename();
    ofLogNotice("ofApp::onHTTPUploadEvent") <<  "     filename: " << args.getFilename();
    ofLogNotice("ofApp::onHTTPUploadEvent") <<  "     fileType: " << args.getFileType().toString();
    ofLogNotice("ofApp::onHTTPUploadEvent") << "# bytes xfer'd: " << args.getNumBytesTransferred();
}
