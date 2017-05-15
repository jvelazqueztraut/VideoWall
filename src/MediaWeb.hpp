//
//  MediaWeb.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/9/17.
//
//

#pragma once

#include "ofMain.h"
#include "Media.hpp"
#include "ofxAwesomium.h"

class MediaWeb : public Media, public ofxAwesomium {
    
public:
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);
		ofxAwesomium::updateCore(); 
		ofxAwesomium::update();
    }
    
    void load(string u,bool r=false){
        URL=u;
        ofxAwesomium::loadURL(URL);
        reload=r;
    }
    
    void play(){
        Media::startPlaytime();
        if(reload)
            ofxAwesomium::loadURL(URL);
    }
    
    void stop(){
        Media::stopPlaytime();
    }
    
    void draw(){
        ofxAwesomium::draw(0,0);
    }
    
    bool isDone(){
        bool done=false;
        switch(loop){
            case LOOP_BY_TIME:
                if(playtime>loopParameter)
                    done=true;
                break;
            default:
                done=true;
                break;
        }
        return done;
    }
    
    float getWidth(){return ofxAwesomium::getWidth();}
    float getHeight(){return ofxAwesomium::getHeight();}
    
    bool reload;
    string URL;
};
