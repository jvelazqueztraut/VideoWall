//
//  MediaInstagram.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/24/17.
//
//

#pragma once

#include "ofMain.h"
#include "Media.hpp"
#include "ofxInstagram.h"
#include "ofxThreadedImageLoader.h"

#define INSTAGRAM_TRANSITION_TIMER 5

class MediaInstagram : public Media, public ofImage {
    
public:
    ~MediaInstagram(){
        loader.stopThread();
    }
    
    void load(string load="liked",int qty=20){
        ofJson credentials;

        ofFile file("credentials_instagram.json");
        file >> credentials;
        
        instagram.setup(credentials["auth_token"],credentials["clientID"]);
        instagram.setCertFileLocation(ofToDataPath("ca-bundle.crt",false));
        
        //if(load=="liked")
            instagram.getUserLikedMedia(qty);
        //else
        //    instagram.getUserFeed(qty);
        
        timer=0;
        current=-1;
    }
    
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);
        
        if(timer<=0){
            current++;
            if(current>=instagram.getImageURL().size())
                current=0;
            if(!instagram.getImageURL().empty()){
                loader.loadFromURL((ofImage&)*this, instagram.getImageURL()[current]);
            }
            timer = INSTAGRAM_TRANSITION_TIMER;
        }
        else{
            timer-=dt;
        }
    }
    
    void play(){
        Media::startPlaytime();
    }
    
    void stop(){
        Media::stopPlaytime();
    }
    
    void draw(){
#ifdef OF_DEBUG
        instagram.drawJSON(0);
#endif
        ofImage::draw(0,0);
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
    
    float getWidth(){return ofImage::getWidth();}
    float getHeight(){return ofImage::getHeight();}
    
    ofxInstagram instagram;
    ofxThreadedImageLoader  loader;
    int current;
    float timer;
};
