//
//  MediaCountdown.h
//  VideoWall
//
//  Created by Javier Velazquez Traut on 5/4/17.
//
//

#pragma once

#include "ofMain.h"
#include "Media.hpp"
#include <time.h>
#include "ofxParagraph.h"

#define DAYS_PER_MONTH {31,28,31,30,31,30,31,31,30,31,30,31}
#define DAYS_PER_MONTH_LEAP {31,29,31,30,31,30,31,31,30,31,30,31}

typedef struct{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
}Date;

class MediaCountdown : public Media {
    
public:
    void load(int year, int month, int day, int hour, int minute, int second , float w, float h){
        width=w;
        height=h;
        
        target.year=year;
        target.month=month;
        target.day=day;
        target.hour=hour;
        target.minute=minute;
        target.second=second;
        
        top.setText("Countdown");
        top.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
        top.setWidth(width*0.9);
        top.setFont("font_status.ttf",height*0.075,"font_top");
        top.setSpacing(height*0.05*.5);
        
        bottom.setText("");
        bottom.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
        bottom.setWidth(width*0.9);
        bottom.setFont("font_user.ttf",height*0.075,"font_bottom");
        bottom.setSpacing(height*0.05*.5);
        
        count.setText("");
        count.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
        count.setWidth(width*0.9);
        count.setFont("font_status.ttf",height*0.1,"font_countdown");
        count.setSpacing(height*0.05*0.75*.5);

    }
    
    void setTopText(string text){
        top.setText(text);
    }
    
    void setBottomText(string text){
        bottom.setText(text);
    }
    
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void setTextColor(ofColor c){
        count.setColor(c);
        top.setColor(c);
        bottom.setColor(c);
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);

        int years = target.year - ofGetYear();
        int months = target.month - ofGetMonth();
        months += years*12;
        
        int days = target.day - ofGetDay();
        
        int daysPerMonth[] = DAYS_PER_MONTH;
        int daysPerMonthLeap[] = DAYS_PER_MONTH_LEAP;
        
        for(int m=0;m<months;m++)
            days+=(isLeap(ofGetYear()+(ofGetMonth()+m)/12)?daysPerMonthLeap[(ofGetMonth()+m-1)%12]:daysPerMonth[(ofGetMonth()+m-1)%12]);
        
        int hours = target.hour - ofGetHours();
        int minutes = target.minute - ofGetMinutes();
        int seconds = target.second - ofGetSeconds();
        if(seconds<0){
            minutes--;
            seconds+=60;
        }
        if(minutes<0){
            hours--;
            minutes+=60;
        }
        if(hours<0){
            days--;
            hours+=24;
        }
        
        string text = ofToString(hours,2,'0')+":"+ofToString(minutes,2,'0')+":"+ofToString(seconds,2,'0')+" hs";
        if(days)
            text = ofToString(days) + " días    " + text;
        
        count.setText(text);
    }
    
    void play(){
        Media::startPlaytime();
    }
    
    void stop(){
        Media::stopPlaytime();
    }
    
    void draw(){
        count.draw((width-count.getWidth())*0.5,height*0.5);
        top.draw((width-top.getWidth())*0.5,height*0.5-height*0.2);
        bottom.draw((width-bottom.getWidth())*0.5,height*0.5+height*0.2);
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
    
    Date target;
    
    ofxParagraph top,bottom,count;
    
    float width,height;
    
    static bool isLeap(int year){
        if(year%4)
            return false;
        else if(year%100)
            return true;
        else if(year%400)
            return false;
        else
            return true;
    }
};
