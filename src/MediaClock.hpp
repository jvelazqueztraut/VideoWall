//
//  MediaClock.h
//  VideoWall
//
//  Created by Javier Velazquez Traut on 11/7/17.
//

#pragma once

#include "ofMain.h"
#include "Media.hpp"
#include "ofxParagraph.h"

#define CLOCK_SIZE (height*0.22)
#define CLOCK_BOTTOM_SIZE (height*0.05)

class MediaClock : public Media {
    
public:
    void load(float w, float h){
        width=w;
        height=h;
        
        clock.setText("--:--:--");
        clock.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
        clock.setWidth(width);
        clock.setFont("font_status.ttf",CLOCK_SIZE,"font_clock");
        clock.setSpacing(CLOCK_SIZE*.5);
        
        bottom.setText("");
        bottom.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
        bottom.setWidth(width*0.9);
        bottom.setFont("font_user.ttf",CLOCK_BOTTOM_SIZE,"font_bottom");
        bottom.setSpacing(CLOCK_BOTTOM_SIZE*.5);
    }
    
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void setTextColor(ofColor c){
        clock.setColor(c);
        bottom.setColor(c);
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);
        
        clock.setText(ofToString(ofGetHours(),2,'0')+":"+ofToString(ofGetMinutes(),2,'0')+":"+ofToString(ofGetSeconds(),2,'0'));
        bottom.setText(ofToString(ofGetDay(),2,'0')+"/"+ofToString(ofGetMonth(),2,'0')+"/"+ofToString(ofGetYear()));
    }
    
    void play(){
        Media::startPlaytime();
    }
    
    void stop(){
        Media::stopPlaytime();
    }
    
    void draw(){
        clock.draw((width-clock.getWidth())*0.5,height*0.5);
        bottom.draw((width-bottom.getWidth())*0.5,height*0.5+height*0.3);
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
    
    float getWidth(){return width;}
    float getHeight(){return height;}
    
    ofxParagraph clock,bottom;
    
    float width,height;
};
