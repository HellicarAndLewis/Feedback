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
#include "ofxVectorMath.h"
#include "ofxControlPanel.h"
#include "ImageLoader.h"

enum {
	MODE_PLASMA,
	MODE_PROJECTOR
};

class InteractiveManager : public ofBaseDraws {
	
protected:
	
	bool					bAllocated;
	float					width, height;

	
	// gui
	ofxControlPanel			panel;
	
	// Interactive Elements
	int						totalModes;
	int						interactiveMode;
	
		//TODO:fold this all into a neat class
	
		//for the plasma just the plasma for now

	ImageLoader		plasmaLoader;
	float			plasmaTimerStart, plasmaTime;
	float			plasmaMaxTime;
	float			plasmaFrameTimerStart, plasmaFrameTime;
	float			plasmaFrameMaxTime;
	int				plasmaFrameIndex;	
	
		//for the projector
	ImageLoader   leftProjectorLoader;
	float			leftProjectorTimerStart, leftProjectorTime;
	float			leftProjectorMaxTime;
	float			leftProjectorFrameTimerStart, leftProjectorFrameTime;
	float			leftProjectorFrameMaxTime;
	int				leftProjectorFrameIndex;	
	
	ImageLoader   rightProjectorLoader;
	float			rightProjectorTimerStart, rightProjectorTime;
	float			rightProjectorMaxTime;
	float			rightProjectorFrameTimerStart, rightProjectorFrameTime;
	float			rightProjectorFrameMaxTime;
	int				rightProjectorFrameIndex;	
	
	ofxDirList dirList;
	
	string plasmaPath;
	string leftProjectorPath;
	string rightProjectorPath;
	
	vector<string>vectorOfSlideshowPaths;
	
	float slideShowDuration;
	float slideshowTimeBetweenSlides;
public:
	
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
	void allocate(int aSlideShowDuration, int aSlideshowMillisBetweenSlides);
	void update();
	void draw();
	
	void draw(float x, float y);
	void draw(float x, float y, float w, float h);
	void drawDebug();
	
	string getUniquePath(string pathToChange);
	
	void setupVectorOfSlideshows();
};







