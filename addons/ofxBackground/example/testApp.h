#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxBackground.h"


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
private:

	ofVideoGrabber 		vidGrabber;			//to get a live video image to analyse
	ofxCvColorImage		colorImg;			//a place to save the live video frame
	ofxBackground		backgroundAddon;	//the addon that allows you to use a variety of methods for background/foreground segmentation
	bool				bLearnBackground;	//a boolean to indicate whether to instruct the addon to learn the background again, defaults to true
											// on startup
};
#endif
