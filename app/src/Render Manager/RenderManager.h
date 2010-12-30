/*
 *  RenderManager.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/29/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxFBOTexture.h"

class RenderManager {

private:
	
	ofxFBOTexture texture;
	
public:
	
	// ---------------------------------------
	RenderManager() {
	}
	
	// ---------------------------------------
	void setupScreen(int w, int h) {
		texture.allocate(w, h);
	}
	// ---------------------------------------
	void begin() { texture.begin(); }
	void end()   { texture.end();   }
	void clear() { texture.clear(); }
	
	// ---------------------------------------
	void update() {
	}
	
	
	// ---------------------------------------
	void draw() {
		texture.draw(0, 0);
	}
	
};