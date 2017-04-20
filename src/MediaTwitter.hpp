//
//  MediaTwitter.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/18/17.
//
//

//
//  MediaWeb.hpp
//  VideoWall
//
//  Created by Javier Velazquez Traut on 4/9/17.
//
//

#pragma once

#include "ofMain.h"
#include "Media.hpp"
#include "ofxTwitter.h"
#include "ofxParagraph.h"

class MediaTwitter : public Media {
    
public:
    void load(string track, float w, float h){
        width=w;
        height=h;
        
        client.setCredentialsFromFile("credentials.json");

        client.registerStreamingEvents(this);
        
        // Create a bounding box for Buenos Aires
        ofxGeo::CoordinateBounds buenosAiresBounds(ofxGeo::Coordinate(-34.418239163, -58.1485748291),
                                                    ofxGeo::Coordinate(-34.8476210128, -58.843460083));
        
        // Create a filter query.
        ofxTwitter::FilterQuery query;
        
        query.setLocations({ buenosAiresBounds });
        
        // Track two emoji.
        // Read more about query terms here:
        // https://dev.twitter.com/streaming/overview/request-parameters
        vector<string> tracks = ofSplitString(track,",");
        query.setTracks(tracks);
        
        // Start filter query.
        //client.filter(query);
        
        text.setText("Tweet show\n"+track);
        text.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
        text.setWidth(width*0.9);
        text.setFont("font.ttf",height*0.1,"default");
    }
    
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void setTextColor(ofColor c){
        text.setColor(c);
    }
    
    void update(float dt){
        Media::updatePlaytime(dt);
    }
    
    void play(){
        Media::startPlaytime();
    }
    
    void stop(){
        Media::stopPlaytime();
    }
    
    void draw(){
        text.draw((width-text.getWidth())*0.5,(height-text.getHeight())*0.5);
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
    
    void onConnect(){
        ofLogNotice("onConnect") << "Connected.";
    }
    
    
    void onDisconnect(){
        ofLogNotice("onDisconnect") << "Disconnected.";
    }
    
    
    void onStatus(const ofxTwitter::Status& status){
        count++;
        ofLogNotice("onStatus") << "Text: " << status.text();
        ofLogNotice("onStatus") << "Coordinates: " << (status.coordinates() ? ofToString(status.coordinates()) : "NONE");
        ofLogNotice("onStatus") << "Place: " << (status.place() ? ofToString(status.place()->fullName()) : "NONE");
    }
    
    
    void onStatusDeletedNotice(const ofxTwitter::StatusDeletedNotice& notice){
        ofLogNotice("onStatusDeletedNotice") << "Status deleted.";
    }
    
    
    void onLocationDeletedNotice(const ofxTwitter::LocationDeletedNotice& notice){
        ofLogNotice("onLocationDeletedNotices") << "Location scrubbed.";
    }
    
    
    void onLimitNotice(const ofxTwitter::LimitNotice& notice){
        countMissed += notice.track();
        ofLogNotice("onLimitNotice") << "Limited: " << notice.track();
    }
    
    
    void onStatusWithheldNotice(const ofxTwitter::StatusWithheldNotice& notice){
        ofLogNotice("onLimitNotice") << "Status witheld.";
    }
    
    
    void onUserWitheldNotice(const ofxTwitter::UserWithheldNotice& notice){
        ofLogNotice("onUserWitheldNotice") << "User witheld.";
    }
    
    
    void onDisconnectNotice(const ofxTwitter::DisconnectNotice& notice){
        ofLogNotice("onDisconnectNotice") << "Disconnect notice: " << notice.reason();
    }
    
    
    void onStallWarning(const ofxTwitter::StallWarning& notice){
        ofLogNotice("onStallWarning") << "Stall warning: " << notice.message();
    }
    
    
    void onException(const std::exception& notice){
        ofLogError("onException") << "Exception: " << notice.what();
    }
    
    
    void onMessage(const ofJson& json){
        // Ignoring the raw message.
    }
    
    ofxTwitter::StreamingClient client;
    uint64_t count = 0;
    uint64_t countMissed = 0;
    
    ofxParagraph text;
    
    float width,height;
};
