#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	//ofSetWindowPosition(-1500, 90);
	
	// load the settings
	settings.loadSettings("app_settings.xml");
	
	// Camera
	float pct = 2;
	w = 640/pct;
	h = 480/pct;
	input.initGrabber(w, h);
	colorImage.allocate(w, h);
	
	// Render
	render.setupScreen(settings.APP_W, settings.APP_H);
	
	// Interactive Manager
	interactive.allocate(w, h, settings.APP_NUMBER_FRAMES_TO_LOOK, settings.APP_AVG_NUMBER_OF_FACES_NEEDED_TO_TRIGGER, settings.APP_ENABLE_GRABBING);	
	
	int numFrames = settings.APP_NUMBER_OF_FRAMES_TO_GRAB;
	int grabEveryFrame = settings.APP_GRAB_EVERY_NTH_FRAME;
	saver.init(numFrames, grabEveryFrame);	
	
	ofHideCursor();
	bShowMouse = false;
	bSaverStatusLastFrame = false;
}
//--------------------------------------------------------------
void testApp::update(){
	
	input.update();
	if(input.isFrameNew()) {
		
		colorImage.setFromPixels(input.getPixels(), w, h);
		colorImage.mirror(false, true);
		
		bool shouldGrab = interactive.updateImage(colorImage);
		
		if (shouldGrab) {
			saver.grabAndSaveImages();
		}
	}
	
	interactive.update();
	
	render.clear();
	render.begin();
	interactive.drawInputFullScreen();
	interactive.draw();
	
	render.end();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	
	// interactive.draw();
	ofSetColor(0xffffff);
	render.draw();
	
	bool wasSavingThisFrame = saver.updateInDraw();
	
//	if(wasSavingThisFrame) {	
//		//here for now
//		ofSetColor(202, 202, 202);
//		ofDrawBitmapString("SAVING", 20, 20);	
//	}
	
	interactive.drawDebug();
	
	if (!wasSavingThisFrame && bSaverStatusLastFrame) {
			//then it switched on the last frame, so trigger a new preset on the screen
		interactive.loadARandomPreset();
	}
	
	bSaverStatusLastFrame = wasSavingThisFrame;
	
	//ofSetColor(202, 202, 202);
	//ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	interactive.keyPressed(key);	
	
	switch (key){
		case 's': input.videoSettings(); break;
				//case 'g': saver.grabAndSaveImages(); break;
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

