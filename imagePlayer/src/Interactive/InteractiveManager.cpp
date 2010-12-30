/*
 *  InteractiveManager.cpp
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/27/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#include "InteractiveManager.h"

// ---------------------------------------
InteractiveManager::InteractiveManager() {
//	printf("--- Interactive Manager Created ---\n");
}

InteractiveManager::~InteractiveManager() {
//	printf("--- Interactive Manager Destroyed ---\n");
}

// ---------------------------------------
void InteractiveManager::allocate(int aSlideShowDuration, int aSlideshowMillisBetweenSlides) {
	
	width = ofGetWidth(); height = ofGetHeight();
	
	plasmaPath = "plasmaPath";
	leftProjectorPath = "projectorLeftPath";
	rightProjectorPath = "projectorRightPath"; //set them up to be meaningless strings to start
	
	setupVectorOfSlideshows();
	
	plasmaPath = getUniquePath(plasmaPath);
	leftProjectorPath = getUniquePath(leftProjectorPath);
	rightProjectorPath = getUniquePath(rightProjectorPath);
	
	slideShowDuration = (float)aSlideShowDuration;
	slideShowDuration *= 1000.f; //milli vanilli
	cout << "slideShowDuration: " << slideshowTimeBetweenSlides << ", recieved from settings, aSlideShowDuration:" << aSlideShowDuration << endl;

	slideshowTimeBetweenSlides = (float)aSlideshowMillisBetweenSlides; //assuming 30fps (-;
	cout << "slideshowTimeBetweenSlides: " << slideshowTimeBetweenSlides << ", recieved from settings, aSlideshowMillisBetweenSlides:" << aSlideshowMillisBetweenSlides << endl;
	
	cout << "slideshowTimeBetweenSlides: " << slideshowTimeBetweenSlides << ", recieved from settings, aSlideshowMillisBetweenSlides:" << aSlideshowMillisBetweenSlides << endl;
	
	plasmaLoader.init(1); //1 image per frame?
	plasmaLoader.loadFromDirectory(plasmaPath);
	plasmaTimerStart	= ofGetElapsedTimef();
	plasmaMaxTime		= slideShowDuration; // every slideShowDuration seconds
	plasmaTime			= 0.f;
	plasmaFrameTimerStart = ofGetElapsedTimef();
	plasmaFrameMaxTime	= slideshowTimeBetweenSlides;
	plasmaFrameTime		= 0.f;
	plasmaFrameIndex	= 0;
	
	leftProjectorLoader.init(1);
	leftProjectorLoader.loadFromDirectory(leftProjectorPath);
		//leftProjectorTimerStart	= ofGetElapsedTimef();// no thanks, makes the other pause + ((float)aSlideShowDuration/2.f); //left is a bit ahead of right, nice hack
	leftProjectorTimerStart	= ofGetElapsedTimef() + (((float)aSlideShowDuration)/2.f);
	leftProjectorMaxTime		= slideShowDuration; // every slideShowDuration seconds
	leftProjectorTime			= 0.f;
	leftProjectorFrameTimerStart = ofGetElapsedTimef(); 
	leftProjectorFrameMaxTime	= slideshowTimeBetweenSlides;
	leftProjectorFrameTime		= 0.f;
	
	rightProjectorLoader.init(1);
	rightProjectorLoader.loadFromDirectory(rightProjectorPath);
	rightProjectorTimerStart	= ofGetElapsedTimef();
	rightProjectorMaxTime		= slideShowDuration; // every slideShowDuration seconds
	rightProjectorTime			= 0.f;
	rightProjectorFrameTimerStart = ofGetElapsedTimef();
	rightProjectorFrameMaxTime	= slideshowTimeBetweenSlides;
	rightProjectorFrameTime		= 0.f;
	
	// gui
	panel.setup("Interactive Manager", 0, 0, 420, ofGetHeight()-20);

	// ------------------------------------ Interactive Manager
	panel.addPanel("Interactive", 1, false);
	panel.setWhichPanel("Interactive");
	vector <string> modeNames;
	modeNames.push_back("Plasma");
	modeNames.push_back("Projector");
	panel.addMultiToggle("Interactive Mode", "INTERACTIVE_MODE", 0, modeNames);
	panel.addToggle("Center App", "CENTER_APP", false);
	panel.addSlider("Z Scale", "Z_SCALE", 0.0, -2000, 2000.0, false);
	panel.addToggle("Reset Z", "RESET_Z", false);	

	// ------------------------------------ Plasma Screen - just get this working first
	panel.addPanel("Plasma Setup", 1, false);
	panel.setWhichPanel("Plasma Setup");
	panel.setWhichColumn(0);
	panel.addSlider("Plasma X", "PLASMA_X", 0.0, -ofGetWidth(), ofGetWidth(), true);
	panel.addSlider("Plasma Y", "PLASMA_Y", 0.0, -ofGetHeight(), ofGetHeight(), true);
	panel.addSlider("Plasma Scale", "PLASMA_SCALE", 1.0, 0.f, 3.f, false);

		// ------------------------------------ Plasma Screen - just get this working first
	panel.addPanel("Projector Setup", 1, false);
	panel.setWhichPanel("Projector Setup");
	panel.setWhichColumn(0);
	panel.addSlider("Projector Left X", "PROJECTOR_LEFT_X", 0.0, -ofGetWidth()*3, ofGetWidth()*3, true);
	panel.addSlider("Projector Left Y", "PROJECTOR_LEFT_Y", 0.0, -ofGetHeight()*3, ofGetHeight()*3, true);
	panel.addSlider("Projector Left Scale", "PROJECTOR_LEFT_SCALE", 1.0, 0.f, 3.f, false);
	panel.addSlider("Projector Right X", "PROJECTOR_RIGHT_X", 0.0, -ofGetWidth()*3, ofGetWidth()*3, true);
	panel.addSlider("Projector Right Y", "PROJECTOR_RIGHT_Y", 0.0, -ofGetHeight()*3, ofGetHeight()*3, true);
	panel.addSlider("Plasma Right Scale", "PROJECTOR_RIGHT_SCALE", 1.0, 0.f, 3.f, false);		
	
	// ------------------------------------ Style
	panel.selectedPanel = 0;
 	panel.loadSettings("feedback_player_presets_startup.xml");
		//panel.setIncrementSave("feedback_player_presets_incremental");
	
	
	// Interactive Elements
	totalModes		= 2;
	interactiveMode = MODE_PLASMA;
	
	bAllocated = true;
	
	panel.toggleView(); //sets it to default to invisible
}

// ---------------------------------------
void InteractiveManager::update() {
	
	// what mode are we running in
	interactiveMode = panel.getValueI("INTERACTIVE_MODE");
	
	// the best gui ever!
	panel.update();
	
	cout << "Interactive mode is" << interactiveMode << endl;
	
	
	if(interactiveMode == MODE_PLASMA) {
		
			// timer every slideShowDuration millis it will clear the old
			// images and load in the new images. 
			// every slideshowTimeBetweenSlides we will try to go to next frame, and keep trying to load if we are waiting..
		plasmaTime = ofGetElapsedTimef() - plasmaTimerStart;
		plasmaTime *= 1000.0;
		plasmaTime += 1000.0;
		if(plasmaTime >= plasmaMaxTime) {
			
			if(plasmaLoader.readyForMoreImages()) {
				plasmaTimerStart = ofGetElapsedTimef();
				plasmaFrameTimerStart = ofGetElapsedTimef();
				plasmaFrameIndex	= 0;					
					//cout << "--- load next set of plasma images ---" << endl;
				
				plasmaPath = getUniquePath(plasmaPath);
				
				plasmaLoader.loadFromDirectory(plasmaPath);
			}
			else {
					//cout << "waiting to load plasma..." << endl;	
			}
		}
		
		plasmaFrameTime = ofGetElapsedTimef() - plasmaFrameTimerStart;
		plasmaFrameTime *= 1000.0;
		plasmaFrameTime += 1000.0;
		if(plasmaFrameTime >= plasmaFrameMaxTime) {	
			
			cout << "plasmaFrameTime: " << plasmaFrameTime << ", plasmaFrameMaxTime: " << plasmaFrameMaxTime << endl;
 		
			int attemptFrame = plasmaFrameIndex+1;
			attemptFrame %= plasmaLoader.totalImages(); //for safety
			
				//cout << "Trying to load frame: " << attemptFrame << endl;
			
			if (plasmaLoader.getImage(attemptFrame)->ready) { //if we can advance, do it, but keep trying every frame to advance before resetting frame
															  //time
				plasmaFrameIndex = attemptFrame;
				plasmaFrameTimerStart = ofGetElapsedTimef();
			}
		}
		
			// update the image loader
		plasmaLoader.update();
	}
	
	if(interactiveMode == MODE_PROJECTOR) {
			//projector only stuff here
			// timer every slideShowDuration millis it will clear the old
			// images and load in the new images. 
			// every slideshowTimeBetweenSlides we will try to go to next frame, and keep trying to load if we are waiting..
		leftProjectorTime = ofGetElapsedTimef() - leftProjectorTimerStart;
		leftProjectorTime *= 1000.0;
		leftProjectorTime += 1000.0;
		if(leftProjectorTime >= leftProjectorMaxTime) {
			
			if(leftProjectorLoader.readyForMoreImages()) {
				leftProjectorTimerStart = ofGetElapsedTimef();
				leftProjectorFrameTimerStart = ofGetElapsedTimef();
				leftProjectorFrameIndex	= 0;					
					//cout << "--- load next set of leftProjector images ---" << endl;
				
				leftProjectorPath = getUniquePath(leftProjectorPath);
				
				leftProjectorLoader.loadFromDirectory(leftProjectorPath);
			}
			else {
					//cout << "waiting to load leftProjector..." << endl;	
			}
		}
		
		leftProjectorFrameTime = ofGetElapsedTimef() - leftProjectorFrameTimerStart;
		leftProjectorFrameTime *= 1000.0;
		leftProjectorFrameTime += 1000.0;
		if(leftProjectorFrameTime >= leftProjectorFrameMaxTime) {	
			
			int attemptFrame = leftProjectorFrameIndex+1;
			attemptFrame %= leftProjectorLoader.totalImages(); //for safety
			
			if (leftProjectorLoader.getImage(attemptFrame)->ready) { //if we can advance, do it, but keep trying every frame to advance before resetting frame
																	 //time
				leftProjectorFrameIndex = attemptFrame;
				leftProjectorFrameTimerStart = ofGetElapsedTimef();				
			}
		}
		
			// update the image loader
		leftProjectorLoader.update();
		
			//now the right
		
			// timer every slideShowDuration millis it will clear the old
			// images and load in the new images. 
			// every slideshowTimeBetweenSlides we will try to go to next frame, and keep trying to load if we are waiting..
		rightProjectorTime = ofGetElapsedTimef() - rightProjectorTimerStart;
		rightProjectorTime *= 1000.0;
		rightProjectorTime += 1000.0;
		if(rightProjectorTime >= rightProjectorMaxTime) {
			
			if(rightProjectorLoader.readyForMoreImages()) {
				rightProjectorTimerStart = ofGetElapsedTimef();
				rightProjectorFrameTimerStart = ofGetElapsedTimef();
				rightProjectorFrameIndex	= 0;					
					//cout << "--- load next set of rightProjector images ---" << endl;
				
				rightProjectorPath = getUniquePath(rightProjectorPath);
				
				rightProjectorLoader.loadFromDirectory(rightProjectorPath);
			}
			else {
					//cout << "waiting to load rightProjector..." << endl;	
			}
		}
		
		rightProjectorFrameTime = ofGetElapsedTimef() - rightProjectorFrameTimerStart;
		rightProjectorFrameTime *= 1000.0;
		rightProjectorFrameTime += 1000.0;
		if(rightProjectorFrameTime >= rightProjectorFrameMaxTime) {	
			
			int attemptFrame = rightProjectorFrameIndex+1;
			attemptFrame %= rightProjectorLoader.totalImages(); //for safety
			
			if (rightProjectorLoader.getImage(attemptFrame)->ready) { //if we can advance, do it, but keep trying every frame to advance before resetting frame
																	  //time
				rightProjectorFrameIndex = attemptFrame;
				rightProjectorFrameTimerStart = ofGetElapsedTimef();					
			}
		}
		
			// update the image loader
		rightProjectorLoader.update();
		
	}	
}

// ---------------------------------------
void InteractiveManager::drawDebug() {
	
	if(interactiveMode == MODE_PLASMA) {
		ofSetColor(0xffffff);
		
		if(plasmaLoader.readyForDisplaying()) {
			cout << "Image debug, image index: " << plasmaFrameIndex << "image width: " << plasmaLoader.getImage(plasmaFrameIndex)->image.width << "image height: " << plasmaLoader.getImage(plasmaFrameIndex)->image.height << endl;
			plasmaLoader.getImage(plasmaFrameIndex)->image.draw(panel.getValueI("PLASMA_X"), panel.getValueI("PLASMA_Y"), plasmaLoader.getImage(plasmaFrameIndex)->image.width*panel.getValueF("PLASMA_SCALE"), plasmaLoader.getImage(plasmaFrameIndex)->image.height*panel.getValueF("PLASMA_SCALE"));
		}else{
//			ofSetColor(0xffffff);
//			ofDrawBitmapString("Plasma Loading...", 20, 20);
		}
		
			// seconds till next plasma image load
//		ofSetColor(0xffffff);
//		ofDrawBitmapString("Plasma Countdown:"+ofToString(plasmaMaxTime/1000.0, 1)+"/"+ofToString(plasmaTime/1000.0, 1)+" seconds", 20, ofGetHeight()-30);
	}
	
	if(interactiveMode == MODE_PROJECTOR) {
		ofSetColor(0xffffff);
		glPushMatrix();
		glRotatef(90, 0, 0, 1); 
		
		ofSetColor(0xffffff);
		
		if(leftProjectorLoader.readyForDisplaying()) {
			leftProjectorLoader.getImage(leftProjectorFrameIndex)->image.draw(panel.getValueI("PROJECTOR_LEFT_X"), panel.getValueI("PROJECTOR_LEFT_Y"), leftProjectorLoader.getImage(leftProjectorFrameIndex)->image.width*panel.getValueF("PROJECTOR_LEFT_SCALE"), leftProjectorLoader.getImage(leftProjectorFrameIndex)->image.height*panel.getValueF("PROJECTOR_LEFT_SCALE"));
		}else{
//			ofSetColor(0xffffff);
//			ofDrawBitmapString("leftProjector Loading...", 20, 20);
		}
		
			// seconds till next leftProjector image load
//		ofSetColor(0xffffff);
//		ofDrawBitmapString("leftProjector Countdown:"+ofToString(leftProjectorMaxTime/1000.0, 1)+"/"+ofToString(	leftProjectorTime/1000.0, 1)+" seconds", 20, ofGetHeight()-30);		
//				
//		ofSetColor(0xffffff);
//		
		ofSetColor(0xffffff);
		
		if(rightProjectorLoader.readyForDisplaying()) {
			rightProjectorLoader.getImage(rightProjectorFrameIndex)->image.draw(panel.getValueI("PROJECTOR_RIGHT_X"), panel.getValueI("PROJECTOR_RIGHT_Y"), rightProjectorLoader.getImage(rightProjectorFrameIndex)->image.width*panel.getValueF("PROJECTOR_RIGHT_SCALE"), rightProjectorLoader.getImage(rightProjectorFrameIndex)->image.height*panel.getValueF("PROJECTOR_RIGHT_SCALE"));
		}else{
			ofSetColor(0xffffff);
//			ofDrawBitmapString("rightProjector Loading...", 20, 20);
		}
		
			// seconds till next rightProjector image load
		ofSetColor(0xffffff);
//		ofDrawBitmapString("rightProjector Countdown:"+ofToString(rightProjectorMaxTime/1000.0, 1)+"/"+ofToString(	rightProjectorTime/1000.0, 1)+" seconds", 20, ofGetHeight()-50);
		
		glPopMatrix();		
	}	
	
	panel.draw();
}

// ---------------------------------------
void InteractiveManager::draw() {
	
		//seems to be not working
	
//	interactiveMode = panel.getValueI("INTERACTIVE_MODE");
//	
//	glPushMatrix();
//	
//	//glTranslatef((ofGetWidth()-width)/2, (ofGetHeight()-height)/2, 0);
//	float ratio = ((float)height/(float)width);
//	float sw = ofGetWidth();
//	float sh = ofGetWidth() * ratio;
//	if(panel.getValueB("CENTER_APP")) {
//		glTranslatef((ofGetWidth()-sw)/2, (ofGetHeight()-sh)/2, panel.getValueF("Z_SCALE"));
//	}
//	glScalef(sw/width, sh/height, 0.0);
//	ofSetColor(0xffffff);
//	if(interactiveMode == MODE_PLASMA) {
//		//plasma only stuff here
//	    if(currPlasmaImg){
//			texPlasma.draw(0,0,currPlasmaImg->width,currPlasmaImg->height);
//				//cout << "Drawing the image!, its size is: " << currImg->width << " by " << currImg->height << endl;
//		}	
//	}
//	if(interactiveMode == MODE_PROJECTOR) {
//		//projector only stuff here
//		
//	}	
//	
//	glPopMatrix();
	
}


// --------------------------------------- GUI Drawing
void InteractiveManager::draw(float x, float y) {
	draw(x, y, width, height);
}
void InteractiveManager::draw(float x, float y, float w, float h) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	// i like this color :)
	//ofSetColor(255, 90, 9);
	//ofFill();
	//ofRect(0, 0, 200, 200);
	
	string info = "Mode: ";
	if(interactiveMode == MODE_PLASMA)	info+="Plasma\n";
	if(interactiveMode == MODE_PROJECTOR) info+="Projector\n";
	ofSetColor(200, 200, 200);
	ofDrawBitmapString(info, 10, 10);
	
	glPopMatrix();
}


// ---------------------------------------
void InteractiveManager::keyPressed(int key) {
	if(key == 'h') panel.toggleView();
	if(key == 'f') ofToggleFullscreen();
	
//	// move through the interactive modes
//	if(panel.getSelectedPanel() == 0) {
//		if(key == OF_KEY_UP)	interactiveMode --;
//		if(key == OF_KEY_DOWN)	interactiveMode ++;
//		interactiveMode %= totalModes;
//		panel.setValueI("INTERACTIVE_MODE", interactiveMode);
//	}
	
	if(key == OF_KEY_RIGHT) {
		if(panel.selectedPanel < panel.panels.size()-1) panel.selectedPanel ++;
	}
	if(key == OF_KEY_LEFT) {
		if(panel.selectedPanel > 0) panel.selectedPanel --;
	}
}
void InteractiveManager::mouseDragged(int x, int y, int button) {
	panel.mouseDragged(x, y, button);
}
void InteractiveManager::mousePressed(int x, int y, int button) {
	panel.mousePressed(x, y, button);
}
void InteractiveManager::mouseReleased(int x, int y, int button) {
	panel.mouseReleased();
}

string InteractiveManager::getUniquePath(string pathToChange){
	int numberOfSlideshows = vectorOfSlideshowPaths.size(); //dirList.listDir("capture/");
	
//	cout << "Number of slideshows is " << vectorOfSlideshowPaths.size() << endl;
	
	string newUniquePath = pathToChange;
	
	while (newUniquePath == plasmaPath || newUniquePath == leftProjectorPath || newUniquePath == rightProjectorPath) {
		int randomFolderIndex = random()%numberOfSlideshows; //danger! possibility of divide by zero
		
		newUniquePath = vectorOfSlideshowPaths[randomFolderIndex];
		
//		cout << "New path is " << newUniquePath << endl;
	}
	
	return newUniquePath;
}

void InteractiveManager::setupVectorOfSlideshows(){
	int numberOfSlideshows = dirList.listDir("capture/");
	
	vectorOfSlideshowPaths.resize(numberOfSlideshows);
	
	for(int i=0; i< numberOfSlideshows; i++){
		vectorOfSlideshowPaths[i] = dirList.getPath(i);
	}
	
		//so you have to rerun the app to get new paths into the system, fair enough, doing the listing with dirlist was causing problems before
		//no time to work out why, think it was because threads were getting locks on the folders?
		//must have at least four folders?
}





