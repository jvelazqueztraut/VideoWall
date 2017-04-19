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
#include "ofxAwesomiumPlus.h"

class MediaWeb : public Media, public ofxAwesomiumPlus {
    
public:
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);
        ofxAwesomiumPlus::update();
    }
    
    void load(string u,bool r=false){
        URL=u;
        ofxAwesomiumPlus::loadURL(URL);
        reload=r;
    }
    
    void play(){
        Media::startPlaytime();
        if(reload)
            ofxAwesomiumPlus::reload(true);
    }
    
    void stop(){
        Media::stopPlaytime();
    }
    
    void draw(){
        ofxAwesomiumPlus::draw(0,0);
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
    
    float getWidth(){return ofxAwesomiumPlus::getWidth();}
    float getHeight(){return ofxAwesomiumPlus::getHeight();}
    
    bool reload;
    string URL;
};
