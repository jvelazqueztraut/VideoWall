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
#include "ofxParagraph.h"
#include "MediaCountdown.hpp"


#define INSTAGRAM_TRANSITION_TIMER 5

#define INSTAGRAM_USER_SIZE (height*0.035)
#define INSTAGRAM_DATE_SIZE (height*0.025)
#define INSTAGRAM_CAPTION_SIZE (height*0.03)

#define INSTAGRAM_PROFILE_WIDTH (width*0.11)
#define INSTAGRAM_PROFILE_HEIGHT (width*0.11)
#define INSTAGRAM_PROFILE_MARGIN (width*0.015)
#define INSTAGRAM_PROFILE_X ((width*0.55)+INSTAGRAM_PROFILE_MARGIN)
#define INSTAGRAM_PROFILE_Y (height*0.2)

class MediaInstagram : public Media{
    
public:
    ~MediaInstagram(){
        loader.stopThread();
    }
    
    void load(float w, float h, string load="liked", int qty=20){
        width=w;
        height=h;
        
        ofJson credentials;

        ofFile file("credentials_instagram.json");
        file >> credentials;
        
        instagram.setup(credentials["auth_token"],credentials["clientID"]);
        instagram.setCertFileLocation(ofToDataPath("ca-bundle.crt",false));
        
        if(load=="liked")
            instagram.getUserLikedMedia(qty);
        else
            instagram.getUserFeed(qty);
        
        profileMasked.allocate(INSTAGRAM_PROFILE_WIDTH,INSTAGRAM_PROFILE_HEIGHT);
        ofPixels img;
        ofLoadImage(img,"border.png");
        profileBorder.loadData(img);
        
        
        user.setText("user");
        user.setAlignment(ofxParagraph::Alignment::ALIGN_LEFT);
        user.setWidth(width*0.3);
        user.setFont("font_status.ttf",INSTAGRAM_USER_SIZE,"font_user");
        user.setIndent(0);

        date.setText("date");
        date.setAlignment(ofxParagraph::Alignment::ALIGN_LEFT);
        date.setWidth(width*0.3);
        date.setFont("font_user.ttf",INSTAGRAM_DATE_SIZE,"font_date");
        date.setIndent(0);

        caption.setText("");
        caption.setAlignment(ofxParagraph::Alignment::ALIGN_LEFT);
        caption.setWidth(width*0.4);
        caption.setFont("font_user.ttf",INSTAGRAM_CAPTION_SIZE,"font_caption");
        caption.setIndent(0);

        timer=0;
        current=-1;
    }
    
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void setTextColor(ofColor c){
        user.setColor(c);
        date.setColor(ofColor(c,c.a-50));
        caption.setColor(c);
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);
        
        if(timer<=0){
            current++;
            if(current>=instagram.getImageURL().size())
                current=0;
            if(!instagram.getImageURL().empty()){
                loader.loadFromURL(image, instagram.getImageURL()[current]);
                loader.loadFromURL(profile,instagram.getProfilePicture()[current]);
                basicData data = instagram.getBasicData()[current];
                user.setText(simpleString(data.user));
                
                for(int i=0;i<data.caption.size();i++){
                    if(data.caption.at(i)=='\n') data.caption.at(i)=' ';
                }
				caption.setText(simpleString(data.caption));
                
                
                int days = ofToInt(data.created_at)/60/60/24;
                int year = 1970;
                int dd=0;
                for(int d = 0; d<days; d++){
                    dd++;
                    if(dd>(MediaCountdown::isLeap(year)?DAYS_PER_YEAR_LEAP:DAYS_PER_YEAR)){
                        year++;
                        dd=0;
                    }
                }
                int daysPerMonth[] = DAYS_PER_MONTH;
                int daysPerMonthLeap[] = DAYS_PER_MONTH_LEAP;
                int month=0;
                days=dd;
                for(int d = 0; d < days; d++){
                    dd++;
                    if(dd>(MediaCountdown::isLeap(year)?daysPerMonthLeap[month]:daysPerMonth[month])){
                        month++;
                        dd=0;
                    }
                }
                days=dd;
                month++;
                date.setText(ofToString(days,2,'0')+"/"+ofToString(month,2,'0')+"/"+ofToString(year));
                cout<<"Basic data:"<<data.imageID<<","<<data.imageURL<<","<<data.user<<","<<data.created_at<<","<<data.caption<<endl;
            }
            timer = INSTAGRAM_TRANSITION_TIMER;
        }
        else{
            timer-=dt;
        }
        
        profileMasked.begin();
        ofClear(0,0);
        ofPushStyle();
        ofSetColor(255);
        ofDrawCircle(INSTAGRAM_PROFILE_WIDTH/2,INSTAGRAM_PROFILE_HEIGHT/2,INSTAGRAM_PROFILE_WIDTH/2);
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        profile.draw(0,0,INSTAGRAM_PROFILE_WIDTH,INSTAGRAM_PROFILE_HEIGHT);
        ofPopStyle();
        profileMasked.end();
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
        float imageResizeWidth = width*0.5;
        float imageResizeHeigth = height*0.8;
        float imageScale = 1.0f;
        float imageX=width*0.05;
        float imageY=height*0.1;
        ofPushMatrix();
        if(image.getHeight()>0 && image.getWidth()/image.getHeight() > imageResizeWidth/imageResizeHeigth){
            imageScale = imageResizeWidth/image.getWidth();
            imageY += (imageResizeHeigth-imageScale*image.getHeight())*0.5f;
        }
        else{
            imageScale = imageResizeHeigth/image.getHeight();
            imageX += (imageResizeWidth-imageScale*image.getWidth())*0.5f;
        }
        ofTranslate(imageX,imageY);
        ofScale(imageScale,imageScale);
        image.draw(0,0);
        ofPopMatrix();
        
        profileMasked.draw(INSTAGRAM_PROFILE_X, INSTAGRAM_PROFILE_Y, INSTAGRAM_PROFILE_WIDTH, INSTAGRAM_PROFILE_HEIGHT);
        profileBorder.draw(INSTAGRAM_PROFILE_X-INSTAGRAM_PROFILE_WIDTH*0.06,INSTAGRAM_PROFILE_Y-INSTAGRAM_PROFILE_HEIGHT*0.06,INSTAGRAM_PROFILE_WIDTH*1.12,INSTAGRAM_PROFILE_HEIGHT*1.12);
        user.draw(INSTAGRAM_PROFILE_X + INSTAGRAM_PROFILE_WIDTH + INSTAGRAM_PROFILE_MARGIN, INSTAGRAM_PROFILE_Y + INSTAGRAM_USER_SIZE + INSTAGRAM_PROFILE_MARGIN);
        date.draw(INSTAGRAM_PROFILE_X+ INSTAGRAM_PROFILE_WIDTH + INSTAGRAM_PROFILE_MARGIN, INSTAGRAM_PROFILE_Y + INSTAGRAM_USER_SIZE + INSTAGRAM_DATE_SIZE + 2*INSTAGRAM_PROFILE_MARGIN);
        caption.draw(INSTAGRAM_PROFILE_X , INSTAGRAM_PROFILE_Y + INSTAGRAM_PROFILE_HEIGHT + INSTAGRAM_CAPTION_SIZE + 2*INSTAGRAM_PROFILE_MARGIN);
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
    
    ofxInstagram instagram;
    ofxThreadedImageLoader  loader;
    ofImage image,profile;
    ofFbo profileMasked;
    ofTexture profileBorder;
    ofxParagraph user,caption,date;
    int current;
    float width,height;
    float timer;
};
