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
    
    virtual void setup(LoopType l, float p){}
    
    void setupPlaytime(LoopType l,float p){
        playtime=0.0f;
        repetitions=0;
        loop=l;
        loopParameter=p;
    }
    
    virtual void update(float dt){}
    
    void updatePlaytime(float dt){
        playtime+=dt;
    }
    
    virtual void draw(){}
    
    virtual void play(){}
    virtual void stop(){}
    
    void startPlaytime(){
        playtime=0.0f;
        repetitions=0;
    }
    
    void stopPlaytime(){
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
