/*
 *  BlobTracker.cpp
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/28/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#include "BlobTracker.h"


	// ---------------------------------------
BlobTracker::BlobTracker() {
	minBlobDistace	= 15.0;
	idCount			= 0;
	bCleanUpCalled  = true;
}

	// ---------------------------------------
bool BlobTracker::removeBlob(Blob &b) {
	return b.dead;
}

	// ---------------------------------------
void BlobTracker::setMinDistance(float minDis) {
	minBlobDistace = minDis;
}
int BlobTracker::getCountLevel() {
	return idCount;
}
void BlobTracker::resetCount() {
	idCount = 0;	
}
void BlobTracker::setSize(float w, float h) {
	width = w; height = h;
}

	// ---------------------------------------
void BlobTracker::addBlob(ofxCvBlob &b) {
	
	Blob cent;
	cent.id				= idCount;
	cent.x				= b.centroid.x;
	cent.y				= b.centroid.y;
	cent.matched		= false;
	cent.newThisFrame   = true;
	cent.nPts			= b.nPts;
	cent.dead			= false;
	cent.nFramesOld		= 0;
	cent.bUsedInSpray   = false;
	
		// copy over the points
	cent.pts.assign(b.nPts, ofPoint());
	for(int i=0; i<cent.nPts; i++) {
		cent.pts[i] = b.pts[i];	
	}
	
	blobs.push_back(cent);
	idCount ++;
	
}

	// ---------------------------------------
void BlobTracker::trackBlobs(vector <ofxCvBlob>& newBlobs) {
	
	if(!bCleanUpCalled) {
		printf("-- you need to call cleanup after this call --\n");
		return;
	}
	
	bCleanUpCalled = false;
	
		// array for matching 
	int  nBlobs = newBlobs.size();
	bool foundThisFrame[nBlobs];
	for(int i=0; i<nBlobs; i++) {
		foundThisFrame[i] = false;
	}
	
	
	
		// reset the tracked blobs 
	for(int i=0; i<blobs.size(); i++) {
		blobs[i].matched = false;
		blobs[i].nFramesOld ++;
	}
	
	
		// find the blob closest to me
	for(int i=0; i<blobs.size(); i++) {
		
		float minDis	= 10000.0;
		int	  minIndex	= -1;
		
		for(int j=0; j<nBlobs; j++) {
			
			float dx  = blobs[i].x - newBlobs[j].centroid.x;
			float dy  = blobs[i].y - newBlobs[j].centroid.y;
			float dis = (float)sqrt(dx*dx + dy*dy);
			
			
			if(minDis > dis) {
				minDis   = dis;
				minIndex = j;
					//printf("minDis:%f - id:%i\n", minDis, minIndex);
			}
		}
		
		
			// ok match me
		if(minIndex != -1 && minDis <= minBlobDistace) {
			
			foundThisFrame[minIndex] = true;	
			
			blobs[i].x				 = newBlobs[minIndex].centroid.x; 
			blobs[i].y				 = newBlobs[minIndex].centroid.y;
			blobs[i].newThisFrame	 = false;
			blobs[i].matched		 = true;
			
				// update the contour points
			blobs[i].pts.clear();
			blobs[i].nPts = newBlobs[minIndex].nPts;
			blobs[i].pts.assign(blobs[i].nPts, ofPoint());
			for(int j=0; j<blobs[i].nPts; j++) {
				blobs[i].pts[j] = newBlobs[minIndex].pts[j];	
			}
			
		}
		
	}		
	
		// now mark blobs that have not been match
		// to dead so that we can remove them
	for(int i=0; i<blobs.size(); i++) {
		if(blobs[i].matched == false) blobs[i].dead = true;
	}
	
	
		// for all the blobs not matched 
		// we need to add them to the system
	for(int i=0; i<nBlobs; i++) {
		if(foundThisFrame[i] == false) {
			addBlob(newBlobs[i]);
		}
	}
	
}

	// ---------------------------------------
void BlobTracker::cleanUpBlobs() {
	
		// clean up and delete
	blobs.erase( blobs.begin(), partition(blobs.begin(), blobs.end(), BlobTracker::removeBlob));
	bCleanUpCalled = true;
	
}

	// ---------------------------------------
void BlobTracker::draw(float x, float y) {
	draw(x,y,width,height);
}
void BlobTracker::draw(float x, float y, float w, float h) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(w/width, h/height, 0.0);
	draw();
	glPopMatrix();
}

	// ---------------------------------------
void BlobTracker::draw() {
	for(int i=0; i<blobs.size(); i++) {
		glPushMatrix();
		glTranslatef(blobs[i].x, blobs[i].y, 0);
		ofSetColor(0x444444);
		ofNoFill();
		ofCircle(0, 0, 20);
		ofSetColor(0xff0000);
		ofDrawBitmapString(ofToString(blobs[i].id)+"/"+ofToString(blobs[i].nFramesOld), -5, 5);
		glPopMatrix();
		
			// draw the contour
		ofNoFill();
		ofSetColor(255, 90, 2);
		ofBeginShape();
		for(int j=0; j<blobs[i].pts.size(); j++) {
			ofVertex(blobs[i].pts[j].x, blobs[i].pts[j].y);
		}
		ofEndShape(true);
	}
}
