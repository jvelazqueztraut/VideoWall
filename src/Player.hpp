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
#include "MediaVideo.hpp"
#include "MediaTwitter.hpp"
#include "MediaInstagram.hpp"
#include "MediaCountdown.hpp"
#include "MediaWeather.hpp"
#include "MediaClock.hpp"

//Leave this one last
#include "MediaWeb.hpp"

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
    M* addContent(){
        current=0;
        contents.push_back(new M());
        return (M*) contents.back();
    }
    
    void update(float dt);
    
    void play();
    void stop();
    
    void draw();
    
    vector< Media * > contents;
    int current;
    
    ofPoint pos;
    float width,height;
    ofColor back;
};
