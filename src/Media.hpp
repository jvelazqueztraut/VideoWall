//
//  Media.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/7/17.
//
//

#pragma once

typedef enum{
    PLAY_ONCE=0,
    LOOP_BY_REP,
    LOOP_BY_TIME
}LoopType;

class Media {
    
public:
    virtual ~Media(){}
    
    void setup(LoopType l,float p){
        playtime=0.0f;
        repetitions=0;
        loop=l;
        loopParameter=p;
    }
    
    void update(float dt){
        playtime+=dt;
    }
    
    virtual void draw(){}
    
    void play(){
        playtime=0.0f;
        repetitions=0;
    }
    
    void stop(){
        playtime=0.0f;
        repetitions=0;
    }
    
    virtual float getWidth(){}
    virtual float getHeight(){}
    
    virtual bool isDone(){}
    
    LoopType loop;
    float loopParameter;
    float playtime;
    unsigned int repetitions;
};
