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
	printf("--- Interactive Manager Created ---\n");
}

InteractiveManager::~InteractiveManager() {
	printf("--- Interactive Manager Destroyed ---\n");
}

// ---------------------------------------
void InteractiveManager::allocate(int w, int h, int aNumberOfFramesToLook, float anAverageNumberOfFacesPerFrame, bool anEnableSaving) {
	
	// opencv
	width = w; height = h;
	
	colorImage.allocate(w, h);
	grayImage.allocate(w, h);
	prevImage.allocate(w, h);
	tracker.setSize(w, h);
	
	// history images
	absDiffImage.allocate(w, h);
	grayFloatImage.allocate(w, h);
	adaptiveFloatImage.allocate(w, h);
	prevGrayFloat.allocate(w, h);
	
		//background jgl
	backgroundAddon.allocate(w,h);
	
	colorImageJustForeground.allocate(w, h);
	imageForBlanking.allocate(w, h);
	binaryForeground.allocate(w, h);
	foregroundMaskAffected.allocate(w, h);
	
	grayImageForHaar.allocate(w, h);
	finder.setup("haar/haarcascade_frontalface_default.xml");
	positionInFaceVec = 0;
	numberOfFramesToLook = aNumberOfFramesToLook;
	averageNumberOfFacesPerFrame = anAverageNumberOfFacesPerFrame;
	
	allTheRoundhouseColours.resize(4);
	
	allTheRoundhouseColours[0].r = 246;
	allTheRoundhouseColours[0].g = 161;
	allTheRoundhouseColours[0].b = 13;
	allTheRoundhouseColours[1].r = 13;
	allTheRoundhouseColours[1].g = 51;
	allTheRoundhouseColours[1].b = 246;
	allTheRoundhouseColours[2].r = 29;
	allTheRoundhouseColours[2].g = 246;
	allTheRoundhouseColours[2].b = 13;
	allTheRoundhouseColours[3].r = 246;
	allTheRoundhouseColours[3].g = 13;
	allTheRoundhouseColours[3].b = 227;
	
	enableSaving = anEnableSaving;
	
	// gui
	panel.setup("Interactive Manager", ofGetWidth()-420, 0, 420, ofGetHeight()-20);

	// ------------------------------------ Interactive Manager
	panel.addPanel("Interactive", 1, false);
	panel.setWhichPanel("Interactive");
	//panel.addDrawableRect("Interactive", this, width/2, height/2);
	vector <string> modeNames;
	modeNames.push_back("Pixel Sprayer");
	modeNames.push_back("Physics Sprayer");
	modeNames.push_back("Slit Sprayer");
	panel.addMultiToggle("Interactive Mode", "INTERACTIVE_MODE", 0, modeNames);
	panel.addToggle("Center App", "CENTER_APP", false);
	panel.addSlider("Z Scale", "Z_SCALE", 0.0, -2000, 2000.0, false);
	panel.addToggle("Reset Z", "RESET_Z", false);

		// ------------------------------------ Pixel Sprayer
	panel.addPanel("Pixel Spray", 2, false);
	panel.setWhichPanel("Pixel Spray");
	panel.setWhichColumn(0);
	panel.addToggle("Enable fade in", "ENABLE_FADE_IN", false);
	panel.addSlider("Fade in speed", "FADE_IN_SPEED", 0.3, 0.0, 1.0, false);
	panel.addSlider("Fade out speed", "FADE_OUT_SPEED", 0.1, 0.0, 1.0, false);
	panel.addSlider("Size Scale Factor", "SIZE_SCL_FCT", 3.0, 0.5, 200.0, false);
	panel.setWhichColumn(1);
	panel.addSlider("Damping", "DAMPING", 0.1, 0.0, 1.0, false);
	panel.addSlider("Max Speed", "MAX_SPEED", 3.0, 1.0, 20.0, false);
	panel.addSlider("Release Magnitude", "RELEASE_MAG", 1.0, 0.0, 20.0, false);
	panel.addToggle("Draw Motion Trail", "DRAW_MOTION_TRAIL", false);
	panel.addToggle("Enable Vector Field Force", "VF_FORCE", false);
	panel.addToggle("Draw Pixel Spray", "DRAW_PIXEL_SPRAY", false);
	
		// ------------------------------------ Vector Field
	panel.addPanel("Vector Field", 1, false);
	panel.setWhichPanel("Vector Field");
	panel.addSlider("Vector Fade Rate", "VECTOR_FADE_RATE", 0.4, 0.0, 1.0, false);
	panel.addSlider("Vector Limit", "VECTOR_LIMIT", 3.0, 0.0, 10.0, false);
	panel.addToggle("Draw Vector Field", "DRAW_VF", false);
	
	
		// ------------------------------------ Physics Sprayer
	panel.addPanel("Physics Spray", 2, false);
	panel.setWhichPanel("Physics Spray");
	panel.setWhichColumn(0);
	panel.addDrawableRect("Tracker", &tracker, width/2.5, height/2.5);
	
	panel.addDrawableRect("Adaptive Image", &adaptiveFloatImage, width/2.5, height/2.5);
	panel.setWhichColumn(1);
	panel.addDrawableRect("Prev Image", &prevGrayFloat, width/2.5, height/2.5);
	panel.addDrawableRect("Diff Image", &absDiffImage, width/2.5, height/2.5);
	panel.setWhichColumn(0);
	
	panel.addSlider("Threshold", "IMAGE_THRESH_DIFF_AMT", 20, 0, 255, true);
	panel.addSlider("Adaptive Amount", "IMAGE_ADAPTIVE_AMOUNT", 0.4, 0.0, 1.0, false);
	panel.addSlider("Decay Amount", "IMAGE_DECAY_AMOUNT", 0.4, 0.0, 1.0, false);
	panel.addSlider("Blob Frame Age", "FRAME_AGE", 3, 0, 20, true);
	panel.addSlider("Num Resample Points", "RESAMPLE_NUM", 13, 4, 200, true);
	panel.addSlider("Max Shape Frame Age", "SHAPE_FRAME_AGE", 200, 10, 600, true);
	panel.addSlider("Shape Fade Rate", "SHAPE_FADE_RATE", 0.5, 0.0, 1.0, false);
	
	panel.setWhichColumn(1);
	panel.addSlider("X Gravity", "GRAVITY_X", 0, -40.0, 40.0, false);
	panel.addSlider("Y Gravity", "GRAVITY_Y", 0, -40.0, 40.0, false);
	panel.addSlider("Floor Y Pos", "FLOOR_Y_POS", height, 0.0, 640.0, false);
	panel.addToggle("Draw box2d floor", "DRAW_B2D_FLOOR", false);
	panel.addToggle("Draw Tracker", "DRAW_TRACKER", false);
	
		// ------------------------------------ Image Processing
	panel.addPanel("Image Processing", 1, false);
	panel.setWhichPanel("Image Processing");
	panel.addDrawableRect("Gray Image", &grayImage, width/2, height/2);	
	panel.addToggle("Draw Motion Vectors", "DRAW_MOTION_VECTORS", false);
	panel.addToggle("Draw Input", "DRAW_INPUT", false);
	panel.addToggle("Blur", "BLUR_INPUT", false);
	
		// ------------------------------------ Slit Spraying
	panel.addPanel("Slit Sprayer", 1, false);
	panel.setWhichPanel("Slit Sprayer");
	panel.addToggle("Reverse", "SLIT_REVERSE", false);
	panel.addSlider("nPortions", "SLIT_PORTIONS", 10, 1, h, true);//taken from slitscan demo default values
	panel.addSlider("nFramesToRecord", "SLIT_FRAMES_TO_RECORD", 40, 1, 1000, true);//these two values portions and frames to record are linked, see below
	panel.addSlider("zoom", "SLIT_ZOOM", 1, 1, 5, true);//we can't have more portions than vertical pixels
	panel.addToggle("Draw Vertical", "SLIT_DRAW_VERT", false);	
	
//	gui.addToggle("reverse", reverse);
//	gui.addSlider("nportions", nPortions, 1, maxPortions); //the logic of this 
//	gui.addSlider("nFramesToRecord", nFramesToRecord, 1, maxPortions); //and this are linked so this may be fucked
//	gui.addSlider("zoom", zoom, 1,5);
//	gui.addToggle("drawVert", bDrawVert);	
	
	// ------------------------------------ ofxBackground
	panel.addPanel("ofxBackground", 2, false); //second int is the number of columns
	panel.setWhichPanel("ofxBackground");
	panel.setWhichColumn(0);
	panel.addToggle("Enable?", "OFX_BACKGROUND_ENABLE", false);	
	panel.setWhichColumn(1);
	panel.addDrawableRect("Foreground Image", &colorImageJustForeground, width/2, height/2);
	vector <string> backgroundMethodNames;	
	backgroundMethodNames.push_back("Avg");
	backgroundMethodNames.push_back("Avg CC");
	backgroundMethodNames.push_back("Codebook");
	backgroundMethodNames.push_back("Codebook CC");	
	panel.addMultiToggle("Background Mode", "OFX_BACKGROUND_METHOD", 0, backgroundMethodNames);
	panel.addToggle("Reset Memory", "OFX_BACKGROUND_RESET", false); //TODO:complete reset of memory
	panel.addToggle("Update Memory TODO", "OFX_BACKGROUND_UPDATE_MEMORY", false); //TODO:complete update of memory
	panel.addToggle("Smooth?", "OFX_BACKGROUND_SMOOTH", false);
	panel.addSlider("Smooth Amount", "OFX_BACKGROUND_SMOOTH_AMOUNT", 4,0,15, true); //true indicates its an int
	panel.setWhichColumn(0);	
	panel.addToggle("Highpass?", "OFX_BACKGROUND_HIGHPASS", false);
	panel.addSlider("Highpass Blur Amount", "OFX_BACKGROUND_HIGHPASS_BLUR_AMOUNT", 4,1,200, true);
	panel.addSlider("Highpass Noise Amount", "OFX_BACKGROUND_HIGHPASS_NOISE_AMOUNT", 4,1,30, true);
	panel.addToggle("Amplify?", "OFX_BACKGROUND_AMPLIFY", false);
	panel.addSlider("Amplify Amount", "OFX_BACKGROUND_AMPLIFY_AMOUNT", 4,1,200, true); //true indicates its an int
	panel.addSlider("Threshold Amount", "OFX_BACKGROUND_THRESHOLD_AMOUNT", 0,0,255, true); //true indicates its an int

		// ------------------------------------ haarDetectionAndTriggering
	panel.addPanel("Face Triggering and Recording", 1, false); //second int is the number of columns
	panel.setWhichPanel("Face Triggering and Recording");
	panel.setWhichColumn(0);
	panel.addToggle("Enable?", "OFX_HAAR_ENABLE", true);
		//through app_settings.xml now
		//panel.addSlider("Number of frames to check for faces", "OFX_HAAR_NUMBER_OF_FRAMES_TO_CHECK", 24, 1, 100, true); //basically vector size
	panel.addSlider("Angle to rotate image, 90 degree increments", "OFX_HAAR_ROTATE_ANGLE", 0, -4, 4, true);
	panel.addDrawableRect("Image being checked for faces", &grayImageForHaar, width/2, height/2);
	
	// ------------------------------------ settings saving 
	panel.addPanel("Presets", 2, false);
	panel.setWhichPanel("Presets");
	panel.setWhichColumn(0);
	lister.listDir("presets/");
	panel.addFileLister("Preset Lister", &lister, 200, 300);
	panel.addToggle("Load random preset", "OFX_FEEDBACK_RANDOM_PRESET_ENABLE", false);
	
//	// ------------------------------------ Style
//	panel.selectedPanel = 2;
// 	panel.loadSettings("feedback_presets_startup.xml");
//	panel.setIncrementSave("feedback_presets_incremental");
	
		// ------------------------------------ Style
	panel.selectedPanel = 3;
 	panel.loadSettings("feedback_presets_startup.xml");
	panel.setIncrementSave("feedback_presets_incremental");	
	panel.setupEvents();
	panel.enableEvents();
	ofAddListener(panel.guiEvent, this, &InteractiveManager::guiEvent);	
	
	// optical flow
	stepSize  = 5;
	flowImage.setCalcStep(stepSize, stepSize);
	flowImage.allocate(w, h);	
	field.setupField(60, 40, width, height);
	
	// Interactive Elements
	totalModes		= 3;
	interactiveMode = MODE_PHYSICS_SPRAYER;
	
	pixelSprayer.init(w, h);
	physicsSprayer.setFloorYPos(panel.getValueF("FLOOR_Y_POS"));
	physicsSprayer.init(w, h);
	slitSprayer.init(w,h);
	
	bAllocated = true;
	
	panel.toggleView();//should set it to be invisible
}

	// ---------------------------------------
void InteractiveManager::guiEvent(guiCallbackData &data) {
	if(data.groupName == "FLOOR_Y_POS") {
		physicsSprayer.setFloorYPos(panel.getValueF("FLOOR_Y_POS"));
	}
}

// ---------------------------------------
bool InteractiveManager::updateImage(ofxCvColorImage &img) {
	
	bool shouldGrab;
	
	shouldGrab = false;
	
	if(!bAllocated) return shouldGrab;
	
	colorImage = img;
	grayImageForHaar = img;
	
		// --------------------------------------
		// JGL Background Addon
		// --------------------------------------
	if(panel.getValueB("OFX_BACKGROUND_RESET")){
		backgroundAddon.reset(width, height);
		backgroundAddon.startLearning();
		panel.setValueB("OFX_BACKGROUND_RESET", false);
	}
	
	backgroundAddon.update(colorImage);
	
	int backgroundMethod = panel.getValueI("OFX_BACKGROUND_METHOD");
	
	switch (backgroundMethod){
		case 0: binaryForeground = backgroundAddon.backgroundAverage.getCvImage(); break;
		case 1: binaryForeground = backgroundAddon.backgroundAverageConnectedComponents.getCvImage(); break;
		case 2: binaryForeground = backgroundAddon.backgroundCodebook.getCvImage(); break;
		case 3: binaryForeground = backgroundAddon.backgroundCodeBookConnectedComponents.getCvImage(); break;
		default: binaryForeground = backgroundAddon.backgroundCodeBookConnectedComponents.getCvImage(); break;
	}
	
	foregroundMaskAffected = binaryForeground;
	
		//now some stuff lifted from CYA People vision
	if(panel.getValueB("OFX_BACKGROUND_SMOOTH")){
		foregroundMaskAffected.blur((panel.getValueI("OFX_BACKGROUND_SMOOTH_AMOUNT") * 2) + 1); //needs to be an odd number
	}
	
		//highpass filter (see cpuimagefilter class)	
	if(panel.getValueB("OFX_BACKGROUND_HIGHPASS")){
		foregroundMaskAffected.highpass(panel.getValueI("OFX_BACKGROUND_HIGHPASS_BLUR_AMOUNT"), panel.getValueI("OFX_BACKGROUND_HIGHPASS_NOISE_AMOUNT"));
	}
	
		//amplify (see cpuimagefilter class)
	if(panel.getValueB("OFX_BACKGROUND_AMPLIFY")){
		foregroundMaskAffected.amplify(foregroundMaskAffected, panel.getValueI("OFX_BACKGROUND_AMPLIFY_AMOUNT"));
	}
		//threshold	
	foregroundMaskAffected.threshold(panel.getValueI("OFX_BACKGROUND_THRESHOLD_AMOUNT")); //Pointless, as it's already binary... is it after blur?
	
		//TODO: COULD ADD FANCY FEEDBACK METHOD HERE FOR TRIPPY NESS
	cvCopy(imageForBlanking.getCvImage(), colorImageJustForeground.getCvImage()); //make sure the image is black, now a colour
	cvCopy(colorImage.getCvImage(), colorImageJustForeground.getCvImage(), foregroundMaskAffected.getCvImage()); //then copy in just fg pixels
	
	colorImageJustForeground.flagImageChanged(); //update it!	

	if(panel.getValueB("OFX_BACKGROUND_ENABLE")){
		colorImage = colorImageJustForeground; //use ofx background
	}else{
		colorImage = img; //don't use ofxbackground
	}
	
		//rotate the image if necessary
	int rotationAngle = panel.getValueI("OFX_HAAR_ROTATE_ANGLE");
	
	grayImageForHaar.rotate(rotationAngle * 90, grayImageForHaar.width/2.f, grayImageForHaar.height/2.f);
	
	
	if(panel.getValueB("OFX_HAAR_ENABLE")){
		int numberOfFramesToCheck = numberOfFramesToLook;//now with app settings, used to be panel.getValueI("OFX_HAAR_NUMBER_OF_FRAMES_TO_CHECK");
		
		if(numberOfFramesToCheck != numberOfFacesInLastNFrames.size()){
			numberOfFacesInLastNFrames.clear();
			numberOfFacesInLastNFrames.resize(numberOfFramesToCheck); //sets all to 0
			positionInFaceVec = 0; //start at the beginning again
		}		
			//then do the haar
		int numberOfFaces = finder.findHaarObjects(grayImageForHaar);
			//cout << "Number of faces " << numberOfFaces;
		
		numberOfFacesInLastNFrames[positionInFaceVec] = numberOfFaces;
		
		positionInFaceVec++;
		positionInFaceVec %= numberOfFacesInLastNFrames.size(); //for safety
		
		int numberOfFacesSeen = 0;
		
		for(int i = 0; i < numberOfFacesInLastNFrames.size(); i++){
			numberOfFacesSeen += numberOfFacesInLastNFrames[i];
		}
		
		float averageNumberOfFacesOverNFrames = ((float)numberOfFacesSeen)/((float)numberOfFacesInLastNFrames.size());
		
		if (averageNumberOfFacesOverNFrames >= averageNumberOfFacesPerFrame) {
			shouldGrab = true;
			
			numberOfFacesInLastNFrames.clear();
			numberOfFacesInLastNFrames.resize(numberOfFramesToCheck); //sets all to 0
			positionInFaceVec = 0; //start at the beginning again			
		}
	}
	
		//Original Todd code
	//colorImage = img;
	grayImage  = colorImage;
	if(panel.getValueB("BLUR_INPUT")) grayImage.blurHeavily();	
	
		// --------------------------------------
		// Physics Spray
		// --------------------------------------
	if(interactiveMode == MODE_PHYSICS_SPRAYER) {
		
		
		ofSetVerticalSync(true);
		
		absDiffImage.absDiff(grayImage, prevImage);
		absDiffImage.dilate();
		absDiffImage.dilate();
		absDiffImage.blurHeavily();
		absDiffImage.threshold(panel.getValueF("IMAGE_THRESH_DIFF_AMT"));
		blobFinder.findContours(absDiffImage,
								20,
								width*height,
								30,
								false);
		
		
			// now go ahead and try to keep track
		tracker.trackBlobs(blobFinder.blobs);
		
		
			// ok now lets see if a blob is old enough
			// and has just died. if this is so we will
			// expolde some crazy shit ---
		int maxage = panel.getValueI("FRAME_AGE");
		for(int i=0; i<tracker.blobs.size(); i++) {
			if(!tracker.blobs[i].bUsedInSpray && tracker.blobs[i].nFramesOld >= maxage && tracker.blobs[i].nFramesOld <= maxage+1) {
					// printf("make a triangle spray here!\n");	
				tracker.blobs[i].bUsedInSpray = true;
				physicsSprayer.addShape(tracker.blobs[i], img);
			}
		}
		
			// make sure to cleanup blobs!
		tracker.cleanUpBlobs();
	}
	
		// --------------------------------------
		// Pixel Spray
		// --------------------------------------
	if(interactiveMode == MODE_PIXEL_SPRAYER) {
		
			// optical flow image
		flowImage.calc(grayImage, prevImage, 11);
		
			// pixels
		unsigned char * px = colorImage.getPixels();
		float			r, g, b;
		ofxVec2f		vel;
		float			len;
		int				index;
		
		
		for(int x=0; x<width; x+=flowImage.captureRowsStep) {
			for(int y=0; y<height; y+=flowImage.captureColsStep) {
				
				index = y*width+x;
				
					// pixel color
				r = px[(index*3)+0] / 255.0;
				g = px[(index*3)+1] / 255.0;
				b = px[(index*3)+2] / 255.0;
				
				vel = flowImage.getVelAtNorm((float)x/(float)width, (float)y/(float)height);
				vel.limit(panel.getValueF("MAX_SPEED"));
				len = vel.length();
				vel *= -len;
				
				if(len > panel.getValueF("RELEASE_MAG")) {
					
					field.addToField(x, y, vel.x, vel.y);
					
					if(interactiveMode == MODE_PIXEL_SPRAYER) {
						pixelSprayer.setPixelParticle(x, y, vel.x, vel.y, len, r, g, b);
					}
					
				}
			}
		}
	}
	
	
		// --------------------------------------
		// Slit Spray
		// --------------------------------------
	if(interactiveMode == MODE_SLIT_SPRAYER) {
		slitSprayer.updatePixels(colorImage);
		
		int tempNumberOfPortions = panel.getValueI("SLIT_PORTIONS");
		int tempNumberOfFramesToRecord = panel.getValueI("SLIT_FRAMES_TO_RECORD");
		
		if (tempNumberOfPortions > slitSprayer.getMaxPortions()){
			tempNumberOfPortions = slitSprayer.getMaxPortions();
		}
		
		if (tempNumberOfPortions > tempNumberOfFramesToRecord) {
			tempNumberOfPortions = tempNumberOfFramesToRecord;
		}
		
		if(tempNumberOfFramesToRecord > slitSprayer.getMaxFramesToRecord()){
			tempNumberOfFramesToRecord = slitSprayer.getMaxFramesToRecord();
		}
		
		slitSprayer.setReverse(panel.getValueB("SLIT_REVERSE"));
		slitSprayer.setNPortions(tempNumberOfPortions);
		slitSprayer.setFramesToRecord(tempNumberOfFramesToRecord);
		slitSprayer.setZoom(panel.getValueI("SLIT_ZOOM"));
		slitSprayer.setVertical(panel.getValueB("SLIT_DRAW_VERT"));
		slitSprayer.update();
		
		panel.setValueI("SLIT_PORTIONS", tempNumberOfPortions);
		panel.setValueI("SLIT_FRAMES_TO_RECORD", tempNumberOfFramesToRecord);		
	}	
	
	// update previous image
	prevImage = grayImage;
	
	if (!enableSaving) {
		shouldGrab = false;
	}
	
	return shouldGrab;
}	

// ---------------------------------------
void InteractiveManager::update() {
	
	if (panel.getValueB("OFX_FEEDBACK_RANDOM_PRESET_ENABLE")) {
		loadARandomPreset();
		panel.setValueB("OFX_FEEDBACK_RANDOM_PRESET_ENABLE", false);
	}
	
	// settings
	if(panel.getValueB("RESET_Z")) {
		panel.setValueF("Z_SCALE", 0.0);
		panel.setValueB("RESET_Z", false);
	}
	
	// what mode are we running in
	interactiveMode = panel.getValueI("INTERACTIVE_MODE");
	
	// the best gui ever!
	panel.update();
	
	if(interactiveMode == MODE_PIXEL_SPRAYER) {
		pixelSprayer.setFadeInMode(panel.getValueF("FADE_IN_SPEED"), panel.getValueB("ENABLE_FADE_IN"));
		pixelSprayer.setFadeOutSpeed(panel.getValueF("FADE_OUT_SPEED"));
		pixelSprayer.setDamping(panel.getValueF("DAMPING"));
		pixelSprayer.setCalcMotionTrial(panel.getValueB("DRAW_MOTION_TRAIL"));
		pixelSprayer.setUseVectorField(panel.getValueB("VF_FORCE"));
		pixelSprayer.setSizeScaleFactor(panel.getValueF("SIZE_SCL_FCT"));
		
		pixelSprayer.update(field);
		
	}
	
	if(interactiveMode == MODE_PHYSICS_SPRAYER) {
		physicsSprayer.setGravity(panel.getValueF("GRAVITY_X"), panel.getValueF("GRAVITY_Y"));
		physicsSprayer.setMaxShapeFrameAge(panel.getValueI("SHAPE_FRAME_AGE"));
		physicsSprayer.setNumResamplePoint(panel.getValueI("RESAMPLE_NUM"));
		physicsSprayer.setShapeFadeRate(panel.getValueF("SHAPE_FADE_RATE"));
		physicsSprayer.update();
	}
	
	field.fadeField(panel.getValueF("VECTOR_FADE_RATE"));
	field.limit(panel.getValueF("VECTOR_LIMIT"));	
	
	if(interactiveMode == MODE_SLIT_SPRAYER){
//			// portions
//		if (key=='+') {
//			if (nPortions < maxPortions && nPortions < nFramesToRecord) {
//				nPortions++;
//			}
//		}   
//		
//		if (key=='-') {
//			if (nPortions > 1) {
//				nPortions--;
//			}
//		}
//		
//			// frames
//		if (key == 'a') {
//			if (nFramesToRecord < maxFramesToRecord) {
//				nFramesToRecord++;
//			}
//		}
//		
//		if (key == 'z') {
//			if (nFramesToRecord > nPortions) {
//				nFramesToRecord--;
//			}
//		}	need to impose the above logic on the system
	
			//doing this all above now, that is why the black frames were coming in?
	}
	
	
}


// ---------------------------------------
void InteractiveManager::drawInputFullScreen() {
	float ratio = ((float)height/(float)width);
	float sw = ofGetWidth();
	float sh = ofGetWidth() * ratio;
	ofSetColor(0xffffff);
	
	if(panel.getValueB("CENTER_APP")) {
		glPushMatrix();
		glTranslatef((ofGetWidth()-sw)/2, (ofGetHeight()-sh)/2, panel.getValueF("Z_SCALE"));
	}
	colorImage.draw(0, 0, sw, sh);
	if(panel.getValueB("CENTER_APP"))glPopMatrix();
}

// ---------------------------------------
void InteractiveManager::drawDebug() {
	panel.draw();
}

// ---------------------------------------
void InteractiveManager::draw() {
	
	
	glPushMatrix();
	
	//glTranslatef((ofGetWidth()-width)/2, (ofGetHeight()-height)/2, 0);
	float ratio = ((float)height/(float)width);
	float sw = ofGetWidth();
	float sh = ofGetWidth() * ratio;
	if(panel.getValueB("CENTER_APP")) {
		glTranslatef((ofGetWidth()-sw)/2, (ofGetHeight()-sh)/2, panel.getValueF("Z_SCALE"));
	}
	glScalef(sw/width, sh/height, 0.0);
	if(panel.getValueB("DRAW_INPUT")) {
		ofSetColor(0xffffff);
		colorImage.draw(0, 0);
		
	}	
	if(panel.getValueB("DRAW_MOTION_VECTORS")) {
		ofSetColor(0xffffff);
		flowImage.draw();
	}
		
	if(interactiveMode == MODE_PIXEL_SPRAYER) {
		if(panel.getValueB("DRAW_PIXEL_SPRAY")) {
			pixelSprayer.draw();
		}
	}
	
	if(interactiveMode == MODE_PHYSICS_SPRAYER) {
		physicsSprayer.draw();
		if(panel.getValueB("DRAW_B2D_FLOOR")) {
			physicsSprayer.drawFloor();
		}		
	}
	
	if(interactiveMode == MODE_SLIT_SPRAYER) {
		slitSprayer.draw();
	}	
	
	if(panel.getValueB("DRAW_VF")) {
		ofSetColor(255, 100, 22);
		field.draw();
	}
	
	if(panel.getValueB("DRAW_TRACKER")) tracker.draw();
	// blobFinder.draw(0, 0);
	
	glPopMatrix();
	
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
	if(interactiveMode == MODE_PIXEL_SPRAYER)	info+="Pixel Sprayer\n";
	if(interactiveMode == MODE_PHYSICS_SPRAYER) info+="Physics Sprayer\n";
	ofSetColor(200, 200, 200);
	ofDrawBitmapString(info, 10, 10);
	
	glPopMatrix();
}


// ---------------------------------------
void InteractiveManager::keyPressed(int key) {
	if(key == 'h') panel.toggleView();
	if(key == 'f') ofToggleFullscreen();
	
	// move through the interactive modes
	if(panel.getSelectedPanel() == 0) {
		if(key == OF_KEY_UP)	interactiveMode --;
		if(key == OF_KEY_DOWN)	interactiveMode ++;
		interactiveMode %= totalModes;
		panel.setValueI("INTERACTIVE_MODE", interactiveMode);
	}
	
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
		
void InteractiveManager::loadARandomPreset(){
	int numberOfPresets = lister.listDir("presets/");
	
	int presetToLoad = random()%numberOfPresets;
	
	string path = lister.getPath(presetToLoad);
	
	cout << "Path to load is" << path << endl;
	
	refillBackground(); //pick a new background colour
	panel.loadSettings(path);
}

void InteractiveManager::fillImageWithColour(ofColor c, ofxCvColorImage& theImage){
	int imageWidth = theImage.width;
	int imageHeight = theImage.height;
	
	unsigned char * pix = new unsigned char[imageWidth*imageHeight*3];
	
	for(int i=0; i<(imageWidth*imageHeight); i++){
		
		pix[ i * 3 + 0 ] = c.r; // set red colour
		pix[ i * 3 + 1 ] = c.g; // set green colour
		pix[ i * 3 + 2 ] = c.b; // set blue colour
		
	}
	
	theImage.setFromPixels(pix, imageWidth, imageHeight);
	
	delete pix;
}

void InteractiveManager::fillImageWithRoundhouseColour(ofxCvColorImage& cvImage){
	int indexOfColourToLoad = random()%allTheRoundhouseColours.size();
	
	ofColor theNewColour = allTheRoundhouseColours[indexOfColourToLoad];
	
	fillImageWithColour(theNewColour, cvImage);
}

void InteractiveManager::refillBackground(){
	fillImageWithRoundhouseColour(imageForBlanking);
}





