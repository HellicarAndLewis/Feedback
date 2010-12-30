/*
 *  PhysicsSprayer.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/28/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "BaseSprayer.h"
#include "ofxBox2d.h"
#include "ofxMSASpline.h"
#include "BlobTracker.h"
#include "ofxVectorMath.h"
#include "ofxDelaunay.h"
#include "ofxOpenCv.h"
#include "TextureTriangle.h"
#include "AppSettings.h"

#define MAX_PHYSICS_SHAPES 200


	// ---------------------------------------
class PhysicsSprayer : public BaseSprayer {
	
private:
	
	float						floorYpos;
	float						shapeFadeRate;
	int							maxFrameAge;
	int							numResamplePts;
	ofxDelaunay					delaunayTriangles;
	ofxMSASpline2D				spline;
	ofxBox2d					box2d;
	vector <TextureTriangle>	triangles;
	vector <VideoFrame*>		videoFrames;
	ofxBox2dRect				floor;
	
public:
	
		// ---------------------------------------
	PhysicsSprayer();
	
	
		// ---------------------------------------
	void init(float w, float h);
	
		// ---------------------------------------
	void setNumResamplePoint(int num);
	
		// ---------------------------------------
	void setMaxShapeFrameAge(int numFrames);
	
		// ---------------------------------------
	void setShapeFadeRate(float rate);
	
		// ---------------------------------------
	void setGravity(float x, float y);
	
		// ---------------------------------------
	void setFloorYPos(float y);
	
		// ---------------------------------------
	void addShape(Blob &b, ofxCvColorImage &image);
	
		// ---------------------------------------
	void update();
	
		// ---------------------------------------
	void draw();
	
		// ---------------------------------------
	void drawFloor();
	
		// ---------------------------------------
	
	void drawFrame();
	
};