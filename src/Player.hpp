//
//  Player.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/7/17.
//
//

#pragma once

#include "ofMain.h"
#include "MediaImage.hpp"

class Player {
    
public:
    ~Player(){
        for(int i=0; i<contents.size(); i++)
            delete contents[i];
    }
    
    void setup(int w, int h);
    void setPos(int x, int y);
    void setBackground(ofColor color);
    
    template<class M>
    M* addContent(LoopType loop, float parameter){
        current=0;
        contents.push_back(new M());
        contents.back()->setup(loop,parameter);
        return (M*) contents.back();
    }
    
    void update(float dt);
    void draw();
    
    void play();
    void stop();
    
    vector< Media * > contents;
    int current;
    
    ofPoint pos;
    float width,height;
    ofColor back;
    
    ofFbo fbo;
};
