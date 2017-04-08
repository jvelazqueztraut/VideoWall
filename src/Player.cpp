//
//  Player.cpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/7/17.
//
//

#include "Player.hpp"

void Player::setup(int w, int h){
    width=w;height=h;
    fbo.allocate(width,height);
    current = -1;
}

void Player::setPos(int x, int y){
    pos.set(x,y);
}

void Player::setBackground(ofColor color){
    back.set(color);
}

void Player::update(float dt){
    if(current>=0){
        contents[current]->update(dt);
        if(contents[current]->isDone()){
            contents[current]->stop();
            current++;
            if(current>=contents.size())
                current=0;
            contents[current]->play();
        }
    }
    
    fbo.begin();
    ofClear(back);
    ofSetColor(255);
    if(current>=0){
        ofPushMatrix();
        float scale = 1.0f;
        float x=0.0f;
        float y=0.0f;
        if(contents[current]->getWidth()!=width || contents[current]->getHeight()!=height){
            if(contents[current]->getWidth()/contents[current]->getHeight() > width/height){
                scale = width/contents[current]->getWidth();
                y = (height-scale*contents[current]->getHeight())*0.5f;
            }
            else{
                scale = height/contents[current]->getHeight();
                x = (width-scale*contents[current]->getWidth())*0.5f;
            }
        }
        ofTranslate(x,y);
        ofScale(scale,scale);
        contents[current]->draw();
        ofPopMatrix();
    }
    fbo.end();
}

void Player::play(){
    stop();
    if(current>=0){
        contents[current]->play();
    }
}

void Player::stop(){
    for(int i=0; i<contents.size(); i++)
        contents[i]->stop();
}

void Player::draw(){
    fbo.draw(pos);
}
