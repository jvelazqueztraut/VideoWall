//
//  MediaImage.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/7/17.
//
//

#pragma once

#include "ofMain.h"
#include "Media.hpp"

class MediaImage : public Media, public ofImage {
    
public:
    
    void draw(){
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
};
