/*
 *  BaseSprayer.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/28/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"

class BaseSprayer {

public:

	float width, height;

	BaseSprayer() {
	}
	
	virtual void init(float w, float h)		{ }	
	virtual void update()					{ }
	virtual void draw()						{ }
	
};