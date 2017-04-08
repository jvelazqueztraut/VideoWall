//
//  MediaVideo.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/8/17.
//
//

#pragma once

#include "ofMain.h"
#include "Media.hpp"

class MediaVideo : public ofVideoPlayer, public Media {
    
public:
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
        ofVideoPlayer::setLoopState(ofLoopType::OF_LOOP_NONE);
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);
        ofVideoPlayer::update();
        if(ofVideoPlayer::getIsMovieDone()){
            if(loop!=PLAY_ONCE){
                ofVideoPlayer::stop();
                repetitions++;
                ofVideoPlayer::play();
            }
        }
    }
    
    void play(){
        Media::startPlaytime();
        ofVideoPlayer::play();
    }
    
    void stop(){
        Media::stopPlaytime();
        ofVideoPlayer::stop();
    }
    
    void draw(){
        ofVideoPlayer::draw(0,0);
    }
    
    bool isDone(){
        bool done=false;
        switch(loop){
            case LOOP_BY_REP:
                if(repetitions>=loopParameter)
                    done=true;
                break;
            case LOOP_BY_TIME:
                if(playtime>loopParameter)
                    done=true;
                break;
            default:
                done=ofVideoPlayer::getIsMovieDone();
                break;
        }
        return done;
    }
    
    float getWidth(){return ofVideoPlayer::getWidth();}
    float getHeight(){return ofVideoPlayer::getHeight();}
};
