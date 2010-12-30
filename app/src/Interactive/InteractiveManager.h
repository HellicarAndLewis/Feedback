/*
 *  InteractiveManager.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/27/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvOpticalFlowLK.h";
#include "ofxVectorMath.h"
#include "ofxControlPanel.h"
#include "vectorField.h"
#include "BlobTracker.h"
#include "ofxBackground.h"
#include "CPUImageFilter.h"
#include "ofxCvHaarFinder.h"

// Interactive Elements
#include "PixelSprayer.h"
#include "PhysicsSprayer.h"
#include "SlitSprayer.h"

enum {
	MODE_PIXEL_SPRAYER,
	MODE_PHYSICS_SPRAYER,
	MODE_SLIT_SPRAYER
};

class InteractiveManager : public ofBaseDraws {
	
protected:
	
	// opencv images
	bool					bAllocated;
	float					width, height;
	ofxCvColorImage			colorImage;
	ofxCvGrayscaleImage		grayImage;
	ofxCvGrayscaleImage		prevImage;
	
	//Motion History
	BlobTracker				tracker;
	ofxCvContourFinder		blobFinder;
	ofxCvGrayscaleImage		absDiffImage;
	ofxCvFloatImage			grayFloatImage;
	ofxCvFloatImage			adaptiveFloatImage;
	ofxCvFloatImage			prevGrayFloat;
	
	
	// optival flow
	int						stepSize;
	ofxCvOpticalFlowLK		flowImage;
	vectorField				field;
	
	// gui
	ofxControlPanel			panel;
	
	// Interacive Elements
	int						totalModes;
	int						interactiveMode;
	
	PixelSprayer			pixelSprayer;
	PhysicsSprayer			physicsSprayer;
		//JGL
	SlitSprayer				slitSprayer;
	
	// box2d particles
	// etc..
	
		//JGL additions
	ofxBackground		backgroundAddon;	//the addon that allows you to use a variety of methods for background/foreground segmentation

	ofxCvColorImage		colorImageJustForeground;
	ofxCvColorImage		imageForBlanking;	
	ofxCvGrayscaleImage binaryForeground;
	
	CPUImageFilter foregroundMaskAffected;
	
	simpleFileLister lister;
	
	ofxCvHaarFinder finder;
	
	ofxCvGrayscaleImage		grayImageForHaar;
	
	vector<int> numberOfFacesInLastNFrames;
	int positionInFaceVec;
	
	int numberOfFramesToLook;
	
	vector<ofColor> allTheRoundhouseColours;
	
	float averageNumberOfFacesPerFrame;
	
	bool enableSaving;
public:
	
	void guiEvent(guiCallbackData &data);
	
	// ---------------------------------------
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	
	// ---------------------------------------
	InteractiveManager();
	~InteractiveManager();
	
	// ---------------------------------------
	float getWidth()  { return width;  }
	float getHeight() {	return height; }
	
	// ---------------------------------------
	void allocate(int w, int h, int aNumberOfFramesToLook, float anAverageNumberOfFacesPerFrame, bool anEnableSaving);
	bool updateImage(ofxCvColorImage &img);
	void update();
	void draw();
	
	void draw(float x, float y);
	void draw(float x, float y, float w, float h);
	void drawInputFullScreen();
	void drawDebug();
	
		//jgl additions
	void loadARandomPreset();
	void fillImageWithColour(ofColor c, ofxCvColorImage& theImage);
	void fillImageWithRoundhouseColour(ofxCvColorImage& cvImage);
	void refillBackground();
};







