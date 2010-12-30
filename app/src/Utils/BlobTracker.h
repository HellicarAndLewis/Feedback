/*
 *  BlobTracker.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/28/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxCvBlob.h"

	// Blob
struct Blob {
	
	int		nFramesOld;
	int		nPts;
	int		id;
	float	x, y;
	bool	matched;
	bool	newThisFrame;
	bool	dead;
	vector  <ofPoint> pts;
	
	bool	bUsedInSpray;
	
};

class BlobTracker : public ofBaseDraws {
	
protected:	
	
	bool					bCleanUpCalled;
	float					minBlobDistace;
	int						idCount;
	float					width, height;
	
	static bool				removeBlob(Blob &b);
	void					addBlob(ofxCvBlob &b);
	
public:
	
	vector <Blob>			blobs;
	
		// ---------------------------------------
	BlobTracker();
	
		// ---------------------------------------
	void	setMinDistance(float minDis);
	int		getCountLevel();
	void	resetCount();
	void	setSize(float w, float h);
	
		// ---------------------------------------
	float	getWidth()  {	return width; }
	float	getHeight() {	return height; }
	
		// ---------------------------------------
	void	trackBlobs(vector <ofxCvBlob>& newBlobs);
	void	cleanUpBlobs();
	
		// ---------------------------------------
	void	draw(float x, float y);
	void	draw(float x, float y, float w, float h);
	
		// ---------------------------------------
	void	draw();
};