/*
 *  VideoFrame.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/29/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class VideoFrame {
public:
	
	VideoFrame() {
	//	printf("--- frame created ---\n");
		bRemove = false;
		refCount = 0;
	}
	~VideoFrame() {
		printf("--- frame removed: %i ---\n", refCount);
	}
	
	ofImage img;
	bool    bRemove;
	int		refCount;
	
};