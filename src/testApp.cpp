#include "testApp.h"
float pos_x = 0;
int row = 10;
int col = 32;
int numLED = col*row;
int SQAURE_ROOT =int(sqrt((float)col));
float ptSize = 5;

#define GREETING "yU+co[lab]hk Happy New year新年快樂";
//--------------------------------------------------------------
void testApp::setup(){
	greeting = GREETING;
	player.loadMovie("fingers.mov");
	player.play();
	led = new ofxLEDsLPD8806(numLED);
	for(int i = 0 ; i< numLED ; i++)
	{
		float x = 50+20+(i%col)*ptSize;
		float y = 100+20+(i/col)*ptSize;
		led->addLED(i,ofVec2f(x,y));
	}
	ofSetFrameRate(12);
	
	serial.listDevices();
	serial.setup(serial.getDeviceList()[5].getDeviceName(), 230400);
	
	ofSetLogLevel(OF_LOG_NOTICE);
	mode = 4;
	grabber.initGrabber(320,240);
	font.loadFont("ArialUnicode.ttf", 10);
	customFbo.allocate(col,row);
	
	bConnected = server.setup( 9093 );
    server.addListener(this);
	image.loadImage("label.gif");
}
void testApp::exit()
{
	
}
//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	pos_x = int(ofGetElapsedTimef()*32);//sin(ofGetElapsedTimef())*32+32;
	player.update();
	
	led->clear(0);
	led->clear(ofColor::black);
	led->renderBuffer.begin();
	
	//draw video as 8px width,height
	//draw line by line horizontally
	//[8 px first row ][8 px second row ]
	drawGraphic();
	led->renderBuffer.end();
	
	led->encode();
	
	serial.writeBytes((unsigned char*)led->txBuffer.data(), led->txBuffer.size());
	
}
void testApp::drawGraphic()
{
	switch(mode)
	{
		case 0:
		{
			ofPushStyle();
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			
			for(int i = 0 ; i < 1; i++)
			{
				ofSetColor(ofColor::fromHsb(i%360, 255, 255),200);
				ofCircle(int(ofGetFrameNum()+i*(SQAURE_ROOT)-16)%numLED, 0, SQAURE_ROOT*0.5);
			}
			ofDisableBlendMode();
			ofPopStyle();
		}break;
		case 1:
			for(int i = 0 ; i < col ; i++)
			{
				//for(int j = 0 ; j < row ; j++)
				{
					player.draw(i*col,-i, col, row);
				}
			}
			break;
		case 2:
		{
			ofPushStyle();
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			int step = 360.0/10;
			for(int i = 0 ; i < numLED; i++)
			{
				ofSetColor(ofColor::fromHsb((i*step)%360, 255, 255),200);
				ofCircle(int(pos_x+i*(16*0.4))%numLED, 0, 16*0.25);
			}
			ofDisableBlendMode();
			ofPopStyle();
		}
			break;
		case 3 :
			for(int i = 0 ; i < col ; i++)
			{
				//for(int j = 0 ; j < row ; j++)
				{
					grabber.draw(i*col,-i, col, row);
				}
			}
			
			break;
		case 4:
			
		{
			customFbo.begin();
			ofClear(0);
			ofPushStyle();
			ofSetColor(ofColor::white);
			ofRectangle rect=font.getStringBoundingBox(greeting, 0, 0);
			//				for(int i = 0 ; i < col ; i++)
			//				{
			//					ofPushStyle();
			//					ofSetColor(ofColor::white);
			//					ofRect(i*col,-i, col, row);
			//					ofPopStyle();
			float width = rect.width-customFbo.getWidth();
			font.drawString(greeting, (sinf(ofGetElapsedTimef())*(width*0.5))-width*0.5,10);
			//				}
			
			ofPopStyle();
			customFbo.end();
			for(int i = 0 ; i < col ; i++)
			{
				ofPushStyle();
				ofSetColor(ofColor::white);
				ofRect(i*col,-i, col, row);
				ofPopStyle();
				customFbo.draw( i*col,-i);
			}
			if(ofGetElapsedTimef() - elapsedTime >5)
			{
				greeting = GREETING;
			}
		}
			
			break;
		case 5:
			
		{
			customFbo.begin();
			ofClear(0);
			ofPushStyle();
			ofSetColor(ofColor::white);
			float scale = (row/image.height*1.);
			float width = image.width*scale;
			float height = image.height*scale;
			float x = sinf(ofGetElapsedTimef()*(width*0.5))-width*0.5;
			image.draw(x,0);

			ofPopStyle();
			customFbo.end();
			for(int i = 0 ; i < col ; i++)
			{
				ofPushStyle();
				ofSetColor(ofColor::white);
				ofRect(i*col,-i, col, row);
				ofPopStyle();
				customFbo.draw( i*col,-i);
			}
		}
			
			break;
	}
	
}
//--------------------------------------------------------------
void testApp::draw(){
	image.draw(0,0);
	ofPushMatrix();
	ofScale(10, 10);
	//draw again to preview
	drawGraphic();
	//	led->encodedBuffer.draw(0,0);
	ofPopMatrix();
	
	ofPushStyle();
	ofNoFill();
	ofSetColor(0, 255, 0);
	ofRect(0,0,numLED*10,10);
	ofPopStyle();
	
	led->draw(ptSize);
	
	//led->draw();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==27)
	{
		memset(led->txBuffer.data(), 0, led->txBuffer.size());
		serial.writeBytes((unsigned char*)led->txBuffer.data(), led->txBuffer.size());
	}
	mode = key-'0';
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}

//--------------------------------------------------------------
void testApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void testApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    messages.push_back("New connection from " + args.conn.getClientIP() + ", " + args.conn.getClientName() );
}

//--------------------------------------------------------------
void testApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    messages.push_back("Connection closed");
}

//--------------------------------------------------------------
void testApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void testApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    greeting = args.message;
	elapsedTime = ofGetElapsedTimef();
	char cmd[256];
	sprintf(cmd,"%s %s",ofToDataPath("./myScript.sh").c_str(),greeting.c_str());
	system(cmd);
	image.loadImage("lable.png");
    // trace out string messages or JSON messages!
    if ( args.json != NULL){
        messages.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName() );
    } else {
        messages.push_back("New message: " + args.message + " from " + args.conn.getClientName() );
    }
	
    // echo server = send message right back!
    args.conn.send( args.message );
}

//--------------------------------------------------------------
void testApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}