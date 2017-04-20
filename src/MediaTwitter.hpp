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
#ifdef OF_DEBUG
        //client.filter(query);
#else
        client.filter(query);
#endif
        
        status.setText("Tweet show\n"+track);
        status.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
        status.setWidth(width*0.9);
        status.setFont("font.ttf",height*0.05,"font_status");
        status.setSpacing(height*0.05*.5);
        
        user.setText("");
        user.setAlignment(ofxParagraph::Alignment::ALIGN_RIGHT);
        user.setWidth(width*0.7);
        user.setFont("font.ttf",height*0.05,"font_user");
        user.setSpacing(height*0.05*.5);
        
        location.setText("");
        location.setAlignment(ofxParagraph::Alignment::ALIGN_RIGHT);
        location.setWidth(width*0.7);
        location.setFont("font.ttf",height*0.05*0.75,"font_location");
        location.setSpacing(height*0.05*0.75*.5);
    }
    
    void setup(LoopType l, float p){
        Media::setupPlaytime(l,p);
    }
    
    void setTextColor(ofColor c){
        status.setColor(c);
        user.setColor(ofColor(c,c.a-20));
        location.setColor(ofColor(c,c.a-20));
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
        status.draw((width-status.getWidth())*0.5,(height-status.getHeight())*0.5);
        user.draw(width*0.85-user.getWidth(),(height+status.getHeight())*0.5+height*0.03);
        location.draw(width*0.85-user.getWidth(),(height+status.getHeight())*0.5+user.getHeight()+height*0.04);
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
    
    
    void onStatus(const ofxTwitter::Status& tweet){
        count++;
        ofLogNotice("onStatus") << "Text: " << tweet.text();
        ofLogNotice("onStatus") << "Coordinates: " << (tweet.coordinates() ? ofToString(tweet.coordinates()) : "NONE");
        ofLogNotice("onStatus") << "Place: " << (tweet.place() ? ofToString(tweet.place()->fullName()) : "NONE");
        
        status.setText(tweet.text());
        user.setText(tweet.user()->name());
        location.setText((tweet.place() ? ofToString(tweet.place()->fullName()) : ""));
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
    
    ofxParagraph status,user,location;
    
    float width,height;
};
