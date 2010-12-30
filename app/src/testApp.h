#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "InteractiveManager.h"
#include "RenderManager.h"
#include "ofxXmlSettings.h"
#include "AppSettings.h"
	//#include "ofxBackground.h"
#include "ImageSaver.h"

class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	// simple grabbing
	int					w, h;
	ofVideoGrabber		input;
	ofxCvColorImage		colorImage;
	
	// interactive
	InteractiveManager	interactive;
	
	// image saver for LCD screens
	ImageSaver			saver;
	
	// render
	RenderManager		render;
	
	bool				bShowMouse;
	bool				bSaverStatusLastFrame;
};

#endif
