#pragma once

#include "ofMain.h"
#include "ofxLEDsLPD8806.h"
#include "ofxLibwebsockets.h"
#include "ofxTrueTypeFontUC.h"
#define NUM_MESSAGES 30 // how many past messages we want to keep

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	ofxLEDsLPD8806 *led;

	ofVideoPlayer player;
	ofVideoGrabber grabber;
	ofSerial serial;
	void drawGraphic();
	int mode;
	ofxTrueTypeFontUC font ;
	
	//websocket
	ofxLibwebsockets::Server server;
    
	bool bConnected;
    
	//queue of rec'd messages
	vector<string> messages;
    
	//string to send to clients
	string toSend;
    
	// websocket methods
	void onConnect( ofxLibwebsockets::Event& args );
	void onOpen( ofxLibwebsockets::Event& args );
	void onClose( ofxLibwebsockets::Event& args );
	void onIdle( ofxLibwebsockets::Event& args );
	void onMessage( ofxLibwebsockets::Event& args );
	void onBroadcast( ofxLibwebsockets::Event& args );
	
	float elapsedTime;
	ofFbo customFbo;
	string greeting;
	ofImage image;
};
