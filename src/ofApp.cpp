#include "ofApp.h"

ofColor complementaryColor(ofColor back){
    int d = 0;
    
    // Counting the perceptive luminance - human eye favors green color...
    double a = 1 - ( 0.299 * back.r + 0.587 * back.g + 0.114 * back.b)/255;
    
    if (a < 0.5)
        d = 0; // bright colors - black font
    else
        d = 255; // dark colors - white font
    
    return  ofColor(d);
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    if(ofIsGLProgrammableRenderer()){
        ofLogNotice("ofApp::setup") << "Programmable Renderer available";
    }
    shader.load("shadersGL2/shader");
    
    fullscreen=false;
    background.set(0);

    applyShader=true;
    rows=3;
    cols=3;
    rowDisplacement=10;
    colDisplacement=10;
    screen.allocate(ofGetWidth() + colDisplacement*(cols-1),ofGetHeight() + rowDisplacement*(rows-1));
    
    ofx::HTTP::SimplePostServerSettings settings;
    // Many other settings are available.
    settings.setPort(7890);
    // Apply the settings.
    server.setup(settings);
    // The client can listen for POST form and multi-part upload events.
    // User be aware, these methods are called from other threads.
    // The user is responsible for protecting shared resources (e.g. ofMutex).
    server.postRoute().registerPostEvents(this);
    
    configurationFile = ofFile("configuration.json");
    if (configurationFile.exists()){
        configurationFile >> configuration;
        applyConfiguration();
    }
    else{
        ofLogError("ofApp::setup")  << "Failed to parse configuration file" << endl;
    }
    newConfiguration = false;
    
    // Start the server.
    server.start();
    
#ifndef OF_DEBUG
    ofHideCursor();
#endif
    
    time=ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::applyConfiguration(bool save){
    
    if(!configuration["fullscreen"].is_null()){
        fullscreen=configuration["fullscreen"];
        ofSetFullscreen(fullscreen);
    }
    ofSleepMillis(500);
    
    if(!fullscreen && !configuration["width"].is_null() && !configuration["height"].is_null()){
        ofSetWindowShape(ofToInt(configuration["width"]),ofToInt(configuration["height"]));
    }
    
    if(!configuration["background"].is_null()){
        background.set(ofToInt(configuration["background"]["r"]),ofToInt(configuration["background"]["g"]),ofToInt(configuration["background"]["b"]));
    }
    
    if(!configuration["rows"].is_null() && !configuration["cols"].is_null()){
        rows = ofToInt(configuration["rows"]);
        cols = ofToInt(configuration["cols"]);
    }
    
    if(!configuration["rowDisplacement"].is_null() && !configuration["colDisplacement"].is_null()){
        rowDisplacement = ofToFloat(configuration["rowDisplacement"]);
        colDisplacement = ofToFloat(configuration["colDisplacement"]);
    }
    
    screen.allocate(ofGetWidth() + colDisplacement*(cols-1),ofGetHeight() + rowDisplacement*(rows-1));
    
    if(!configuration["players"].is_null()){
        
        float width = screen.getWidth();
        float height = screen.getHeight();
        
        ///////////////////////////////////////////////////
        int rows = ofToInt(configuration["rows"]);
        int cols = ofToInt(configuration["cols"]);
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
                    int z = configuration["zones"][r][c];
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
                            if(!taken[rr][cc] && (z==(int)configuration["zones"][rr][cc])){
                                if(((rr-1)>=0 && z==(int)configuration["zones"][rr-1][cc]) || ((cc-1)>=0 && z==(int)configuration["zones"][rr][cc-1]) || ((rr+1)<rows && z==(int)configuration["zones"][rr+1][cc]) || ((cc+1)<cols && z==(int)configuration["zones"][rr][cc+1])){
                                    taken[rr][cc] = true;
                                    newPlayers.back().width = max(cc-c+1,newPlayers.back().width);
                                    newPlayers.back().height = max(rr-r+1,newPlayers.back().height);
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
                if(newPlayers[p].playerId == (int)configuration["players"][i]["id"]){
                    ofLogNotice("ofApp::applyConfiguration") << "Pos:"<< player.pos.x << "," << player.pos.y << " Size:"<< player.width << "," << player.height;
                    player.setBackground(ofColor(ofToInt(configuration["players"][i]["background"]["r"]),ofToInt(configuration["players"][i]["background"]["g"]),ofToInt(configuration["players"][i]["background"]["b"])));
                    for(int m=0; m<configuration["players"][i]["contents"].size(); m++){
                        Media * media;
                        if(configuration["players"][i]["contents"][m]["type"] == "image"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding image content.";
                            MediaImage * image = player.addContent<MediaImage>();
                            string file = configuration["players"][i]["contents"][m]["load"];
                            image->ofImage::load(file);
                            media = image;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"] == "video"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding video content.";
                            MediaVideo * video = player.addContent<MediaVideo>();
                            video->ofVideoPlayer::load(configuration["players"][i]["contents"][m]["load"]);
                            media = video;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"] == "web"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding web content.";
                            MediaWeb * web = player.addContent<MediaWeb>();
                            web->ofxAwesomiumPlus::setup(player.width,player.height,"VideoWall");
                            web->load(configuration["players"][i]["contents"][m]["load"],configuration["players"][i]["contents"][m]["reload"]);
                            WebPlayer webPlayer; webPlayer.web = web; webPlayer.player = &player;
                            webs.push_back(webPlayer);
                            media = web;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"] == "youtube"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding youtube content";
                            MediaWeb * web = player.addContent<MediaWeb>();
                            web->ofxAwesomiumPlus::setup(player.width,player.height,"VideoWall");
                            string videoId = configuration["players"][i]["contents"][m]["load"];
                            if(configuration["players"][i]["contents"][m]["reload"] || videoId.substr(0,2)=="PL") //Youtube playlist
                                web->load("https://www.youtube.com/embed?listType=playlist&list="+ videoId + "&version=3&autoplay=1&loop=1&controls=0&modestbranding=1&showinfo=0");
                            else //Youtube video in loop mode
                                web->load("https://www.youtube.com/embed/"+ videoId + "?version=3&playlist=" + videoId + "&autoplay=1&loop=1&controls=0&modestbranding=1&showinfo=0");
                            WebPlayer webPlayer; webPlayer.web = web; webPlayer.player = &player;
                            webs.push_back(webPlayer);
                            media = web;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"] == "twitter"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding twitter content";
                            MediaTwitter * twitter = player.addContent<MediaTwitter>();
                            twitter->load(configuration["players"][i]["contents"][m]["load"],player.width,player.height);
                            twitter->setTextColor(complementaryColor(player.back));
                            media = twitter;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"] == "instagram"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding instagram content";
                            MediaInstagram * instagram = player.addContent<MediaInstagram>();
                            instagram->load(configuration["players"][i]["contents"][m]["load"],ofToInt(configuration["players"][i]["contents"][m]["qty"]));
                            media = instagram;
                        }
                        else if(configuration["players"][i]["contents"][m]["type"] == "countdown"){
                            ofLogNotice("ofApp::applyConfiguration") << "Adding countdown";
                            MediaCountdown * countdown = player.addContent<MediaCountdown>();
                            countdown->load(2017,5,6,10,50,40,player.width,player.height);
                            countdown->setTopText("FALTAN");
                            countdown->setBottomText("para el evento de #A3");
                            countdown->setTextColor(complementaryColor(player.back));
                            media = countdown;
                        }
                        else{
                            ofLogNotice("ofApp::applyConfiguration") << "Adding NULL player.";
                            MediaImage * image = player.addContent<MediaImage>();
                            image->load("logo.png");
                            media = image;
                        }
                        if(configuration["players"][i]["contents"][m]["loop"] == "time"){
                            media->setup(LOOP_BY_TIME,ofToFloat(configuration["players"][i]["contents"][m]["time"]));
                        }
                        else if(configuration["players"][i]["contents"][m]["loop"] == "repetitions"){
                            media->setup(LOOP_BY_REP,ofToInt(configuration["players"][i]["contents"][m]["repetitions"]));
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
        
        if(save)
            ofSavePrettyJson(configurationFile,configuration);
        
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
    ofEnableAlphaBlending();
    if(applyShader){
        shader.begin();
        shader.setUniform1f("colInterval", ofGetWidth()/(float)cols);
        shader.setUniform1f("rowInterval", ofGetHeight()/(float)rows);
        shader.setUniform1f("colDisplacement", colDisplacement);
        shader.setUniform1f("rowDisplacement", rowDisplacement);
    }

    screen.draw(0,0);
    
    if(applyShader){
        shader.end();
    }
    
#ifdef OF_DEBUG
    ofSetColor(255,0,255);
    for(int c=1; c<cols; c++)
        ofDrawLine(c*ofGetWidth()/cols, 0, c*ofGetWidth()/cols, ofGetHeight());
    for(int r=1; r<rows; r++)
        ofDrawLine(0, r*ofGetHeight()/rows, ofGetWidth(), r*ofGetHeight()/rows);
    
    ofDrawBitmapStringHighlight("See " + server.url(), 10, 16);
    ofDrawBitmapStringHighlight("colDisplacement " + ofToString(colDisplacement), 10, 32);
    ofDrawBitmapStringHighlight("rowDisplacement " + ofToString(rowDisplacement), 10, 48);
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='f'){
        ofToggleFullscreen();
        ofShowCursor();
    }
    for(int i=0; i<webs.size(); i++){
        webs[i].web->ofxAwesomiumPlus::keyPressed(key);
    }
    
    if(key==OF_KEY_UP){
        rowDisplacement+=1;
    }
    else if(key==OF_KEY_DOWN){
        rowDisplacement-=1;
    }
    if(key==OF_KEY_RIGHT){
        colDisplacement+=1;
    }
    else if(key==OF_KEY_LEFT){
        colDisplacement-=1;
    }
    if(key=='s'){
        applyShader=!applyShader;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    for(int i=0; i<webs.size(); i++){
        webs[i].web->ofxAwesomiumPlus::keyReleased(key);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for(int i=0; i<webs.size(); i++){
        ofPoint webPos(webs[i].player->pos);
        webPos.x-=colDisplacement*floor(webPos.x/(ofGetWidth()/cols));
        webPos.y-=rowDisplacement*floor(webPos.y/(ofGetHeight()/rows));
        webs[i].web->ofxAwesomiumPlus::mouseMoved(x-webPos.x,y-webPos.y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    for(int i=0; i<webs.size(); i++){
        ofPoint webPos(webs[i].player->pos);
        webPos.x-=colDisplacement*floor(webPos.x/(ofGetWidth()/cols));
        webPos.y-=rowDisplacement*floor(webPos.y/(ofGetHeight()/rows));
        webs[i].web->ofxAwesomiumPlus::mouseDragged(x-webPos.x,y-webPos.y,button);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for(int i=0; i<webs.size(); i++){
        ofPoint webPos(webs[i].player->pos);
        webPos.x-=colDisplacement*floor(webPos.x/(ofGetWidth()/cols));
        webPos.y-=rowDisplacement*floor(webPos.y/(ofGetHeight()/rows));
        webs[i].web->ofxAwesomiumPlus::mousePressed(x-webPos.x,y-webPos.y,button);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    for(int i=0; i<webs.size(); i++){
        ofPoint webPos(webs[i].player->pos);
        webPos.x-=colDisplacement*floor(webPos.x/(ofGetWidth()/cols));
        webPos.y-=rowDisplacement*floor(webPos.y/(ofGetHeight()/rows));
        webs[i].web->ofxAwesomiumPlus::mouseReleased(x-webPos.x,y-webPos.y,button);
    }
}

//--------------------------------------------------------------
void ofApp::onHTTPPostEvent(ofx::HTTP::PostEventArgs& args){
    ofLogNotice("ofApp::onHTTPPostEvent") << "Data: " << args.getBuffer().getText();
    // Now try to parse the configuration
    if(!args.getBuffer().getText().empty()){
        configuration = nlohmann::json::parse(args.getBuffer().getText());
        newConfiguration = true;
    }
    Poco::Net::HTTPServerResponse& response(args.response());
    response.HTTPResponse::HTTPMessage::MessageHeader::NameValueCollection::add("Access-Control-Allow-Origin","*");
    response.setReason(ofToString(ofGetFrameRate(),2));
    response.send();
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
