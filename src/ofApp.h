#pragma once

#include "ofMain.h"
#include "Player.hpp"
#include "ofxJSON.h"

#include "ofxHTTP.h"

typedef struct{
    MediaWeb * web;
    Player * player;
}WebPlayer;

typedef struct{
    int playerId;
    float x,y;
    float width, height;
    bool added;
}PlayerSize;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
        void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
    
    ofxJSONElement configuration;
    void applyConfiguration();
    bool newConfiguration;
    
    vector< Player > players;
    
    vector< WebPlayer > webs;
        
    float time;
    
    void onHTTPPostEvent(ofx::HTTP::PostEventArgs& evt);
    void onHTTPFormEvent(ofx::HTTP::PostFormEventArgs& evt);
    void onHTTPUploadEvent(ofx::HTTP::PostUploadEventArgs& evt);
    
    ofx::HTTP::SimplePostServer server;
};
