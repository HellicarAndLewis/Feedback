/*
 *  PhysicsSprayer.cpp
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/28/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */



#include "PhysicsSprayer.h"

static bool removeShape(TextureTriangle &b) {
	return b.dead;
}


static bool removeVideoFrame(VideoFrame *frame) {
	bool bRemove = false;
	if(frame != NULL) {
		bRemove = frame->bRemove;
		if(bRemove) {
			delete frame;
			frame = NULL;
		}
	}
	
	return bRemove;
}

	// ---------------------------------------
PhysicsSprayer::PhysicsSprayer() {
}


	// ---------------------------------------
void PhysicsSprayer::init(float w, float h) {
	
	width = w; height = h;
	
		// defaults
	numResamplePts = 10;
	maxFrameAge = 200;
	shapeFadeRate = 0.2;
		// Box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.setFPS(30.0);
	
		// make a floor
	floor.setPhysics(10, 0, 0);
	floor.setup(box2d.getWorld(), 0, floorYpos, w, 5, true);
	
	delaunayTriangles.setSize(w, h);
}

	// ---------------------------------------
void PhysicsSprayer::setNumResamplePoint(int num) {
	numResamplePts = num;
}

	// ---------------------------------------
void PhysicsSprayer::setMaxShapeFrameAge(int numFrames) {
	maxFrameAge = numFrames;
}

	// ---------------------------------------
void PhysicsSprayer::setGravity(float x, float y) {
	box2d.setGravity(x, y);
}

	// ---------------------------------------
void PhysicsSprayer::setShapeFadeRate(float rate) {
	shapeFadeRate = rate;
}

	// ---------------------------------------
void PhysicsSprayer::setFloorYPos(float y) {
	floorYpos = y;
	if(floor.alive) {
		floor.setPosition(0, y);
	}
}


	// ---------------------------------------
void PhysicsSprayer::addShape(Blob &b, ofxCvColorImage &image) {
	
	if(triangles.size() >= MAX_PHYSICS_SHAPES) return;
	
		// add a new frame of video
	ofEnableArbTex();
	int recW = image.getWidth();
	int recH = image.getHeight();
	videoFrames.push_back(new VideoFrame());
	videoFrames.back()->img.allocate(recW, recH, OF_IMAGE_COLOR);
	videoFrames.back()->img.setFromPixels(image.getPixels(), recW, recH, OF_IMAGE_COLOR);
	videoFrames.back()->bRemove = false;
	
		// simplify the points
	spline.clear();
	spline.reserve(b.nPts);
	for(int i=0; i<b.nPts; i++) {
		spline.push_back(b.pts[i]);	
	}
	
		// if we have enough points
		// lets make triangles
	if(spline.size() > 4) {
		vector <ofPoint> tempPts;
		float spacing  = 1.0/(float)numResamplePts;
		for(float f=0; f<1; f+=spacing) {
			ofPoint v = spline.sampleAt(f);
			tempPts.push_back(v);
		}
		delaunayTriangles.findTriangles(tempPts);
	}
	
		// now make the box2d triangle
	for(int i=0; i<delaunayTriangles.numTriangles(); i++) {
		DelaunayTriangle tripts = delaunayTriangles.getTriangle(i);
		
		TextureTriangle tri;
		tri.setPhysics(1.5, 0.4, 0.09);
		tri.addVertex(tripts.a);
		tri.addVertex(tripts.b);
		tri.addVertex(tripts.c);
		tri.createShape(box2d.getWorld(), 0, 0);
		
		triangles.push_back(tri);
		triangles.back().launchPts.push_back(tripts.a);
		triangles.back().launchPts.push_back(tripts.b);
		triangles.back().launchPts.push_back(tripts.c);
		triangles.back().frameRef = videoFrames.back();
		videoFrames.back()->refCount ++;
	}	
}

	// ---------------------------------------
void PhysicsSprayer::update() {
	
	box2d.update();
	
	
	float top		= 0;
	float bottom	= ofGetHeight() - height;
	float right		= ofGetWidth();
	float left		= 0;
	
	for(int i=0; i<triangles.size(); i++) {
		
		triangles[i].update();
		triangles[i].frameAge ++;
		
			// if we are old enough start to fade out and destroy
		if(triangles[i].frameAge >= maxFrameAge && !triangles[i].bFadeOut) {
			triangles[i].bFadeOut = true;
		}
		
			// fade out the shape
		if(triangles[i].bFadeOut) {
			triangles[i].alpha -= shapeFadeRate;
		}
		
			// are we fading out and the alpha is zero
			// remove the shape.
		if(triangles[i].bFadeOut && triangles[i].alpha <= 0.0) {
			triangles[i].destroyShape();
			continue;
		}
		
			// out of bounds just remove the shape
		ofPoint p = triangles[i].getCenter();
		if(p.y > bottom) {
			triangles[i].destroyShape();
			continue;
		}
		if(p.y < top) {
			triangles[i].destroyShape();
			continue;
		}
		if(p.x > right) {
			triangles[i].destroyShape();
			continue;
		}
		if(p.x < left) {
			triangles[i].destroyShape();
			continue;
		}
	}
	
		// is the triangle dead...
	for(int i=0; i<triangles.size(); i++) {
		if(triangles[i].dead) {
			if(triangles[i].frameRef) triangles[i].frameRef->refCount --;
		}
	}
	
		// remove the videoframe if there is
		// a reference count of zero or les
	for(int i=0; i<videoFrames.size(); i++) {
		if(videoFrames[i]->refCount <= 0) {
			videoFrames[i]->bRemove = true;	
		}
	}
	
		// do the cleaning up
	triangles.erase( triangles.begin(), partition(triangles.begin(), triangles.end(), removeShape));
	videoFrames.erase( videoFrames.begin(), partition(videoFrames.begin(), videoFrames.end(), removeVideoFrame));
	
}

	// ---------------------------------------
void PhysicsSprayer::draw() {
	
		// the bounds of the box2d world
	float top		= 0;
	float bottom	= height;
	float right		= width;
	float left		= 0;
	ofNoFill();
	ofSetColor(255, 90, 2);
		//ofRect(left, top, right, bottom);
	
	
	for(int i=0; i<triangles.size(); i++) {
		triangles[i].draw();
	}
	
		// floor.draw();
	
}

	// ---------------------------------------
void PhysicsSprayer::drawFloor() {
	floor.draw();
}

	// ---------------------------------------

void PhysicsSprayer::drawFrame() {
	for(int i=0; i<videoFrames.size(); i++) {
		ofSetColor(0xffffff);
		videoFrames[i]->img.draw(i*videoFrames[i]->img.getWidth(), 0);
	}
}