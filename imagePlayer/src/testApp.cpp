#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	
	// load the settings
	settings.loadSettings("app_settings.xml");
	
	// Interactive Manager
	interactive.allocate(settings.APP_SLIDESHOW_DURATION, settings.APP_SLIDESHOW_MILLIS_BETWEEN_SLIDES);	
	
	ofHideCursor();
	bShowMouse = false;
}
//--------------------------------------------------------------
void testApp::update(){
	interactive.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	
	interactive.drawDebug();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	interactive.keyPressed(key);	
	
	switch (key){
		case 'm': 		
			if(bShowMouse){
				ofHideCursor();
				bShowMouse = false;
			}else {
				ofShowCursor();
				bShowMouse = true;
			} 
			break;
		default: break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	interactive.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	interactive.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	interactive.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

