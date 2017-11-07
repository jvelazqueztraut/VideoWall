//
//  MediaWeather.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 11/6/17.
//

#pragma once

#include "ofMain.h"
#include "Media.hpp"
#include "ofxParagraph.h"
#include "ofxSVG.h"

#define ZERO_KELVIN (-273.15f)
#define WEATHER_UPDATE_TIMER 60
#define WEATHER_TEMPERATURE_SIZE (height*0.24)
#define WEATHER_BOTTOM_SIZE (height*0.05)

class MediaWeather : public Media {
    
public:
    ~MediaWeather(){
        ofLogWarning()<< "Closing MediaWeather" << endl;
        ofUnregisterURLNotification(this);
    }
    
    void load(float w, float h){
        width=w;
        height=h;
        
        temperature.setText("--.-°");
        temperature.setAlignment(ofxParagraph::Alignment::ALIGN_RIGHT);
        temperature.setWidth(width*0.4);
        temperature.setFont("fonts/Raleway-Bold.ttf",WEATHER_TEMPERATURE_SIZE,"font_bold");
        temperature.setSpacing(WEATHER_TEMPERATURE_SIZE*.5);
        
        bottom.setText("");
        bottom.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
        bottom.setWidth(width*0.9);
        bottom.setFont("fonts/Raleway-Medium.ttf",WEATHER_BOTTOM_SIZE,"font_medium");
        bottom.setSpacing(WEATHER_BOTTOM_SIZE*.5);
        
        svg.load(ofToDataPath("weather/na.svg"));
        
        ofRegisterURLNotification(this);
        timer=0;
    }
    
    void urlResponse(ofHttpResponse & response){
        if(response.request.name == "weather"){
            if(response.status == 200){
                ofJson data = ofJson::parse(response.data);
                float temp = data["main"]["temp"];
                temp += ZERO_KELVIN;
                temperature.setText(ofToString(temp,1) + "°");
                
                int pressure = data["main"]["pressure"];
                int humidity = data["main"]["humidity"];
                float temp_min = data["main"]["temp_min"];
                temp_min += ZERO_KELVIN;
                float temp_max = data["main"]["temp_max"];
                temp_max += ZERO_KELVIN;
                bottom.setText("Min:"+ofToString(temp_min,1)+"°  Max:"+ofToString(temp_max,1)+"°  "+ofToString(pressure)+" HPa  "+ofToString(humidity)+"%");
                
                string weather_icon = data["weather"][0]["icon"];
                if(ofFile::doesFileExist(ofToDataPath("weather/"+weather_icon+".svg"))){
                    svg.load(ofToDataPath("weather/"+weather_icon+".svg"));
                }
                if(svg.getNumPath() && (svg.getPathAt(0).getFillColor()!=color || svg.getPathAt(0).getStrokeColor()!=color)){
                    for(int p=0;p<svg.getNumPath();p++)
                        svg.getPathAt(p).setColor(color);
                }
            }else{
                cout << response.status << " " << response.error << " for request " << response.request.name << endl;
            }
        }
    }
    
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void setTextColor(ofColor c){
        temperature.setColor(c);
        bottom.setColor(c);
        color=c;
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);
        
        if(timer<=0){
            ofLoadURLAsync("http://api.openweathermap.org/data/2.5/weather?id=3430234&appid=6831b02667112590928df0e450bb1a7a","weather");
            timer=WEATHER_UPDATE_TIMER;
        }
        else{
            timer-=dt;
        }
    }
    
    void play(){
        Media::startPlaytime();
    }
    
    void stop(){
        Media::stopPlaytime();
    }
    
    void draw(){
        temperature.draw(width*0.18,height*0.55);
        bottom.draw((width-bottom.getWidth())*0.5,height*0.5+height*0.3);
        
        ofPushMatrix();
        //ofTranslate(ofGetMouseX(),ofGetMouseY());
        ofTranslate(width*0.55,height*0.1);
        float scale = ofMap(height, 0, 1000, 0, 1);
        ofScale(scale, scale, scale);
        svg.draw();
        ofPopMatrix();
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
    
    ofxParagraph temperature,bottom;
    ofxSVG svg;
    ofColor color;

    float width,height;
    float timer;
};
