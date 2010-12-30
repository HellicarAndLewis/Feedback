/*
 *  SlitSprayer.h
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 6/4/2010.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once 
#include "ofMain.h"
#include "BaseSprayer.h"
#include "ofxOpenCv.h"

struct Portion {
	
    int x;
    int y;
    
    int w; 
    int h;
    int size;
    
};

class SlitSprayer : public BaseSprayer {
	
protected:
	unsigned char **    videoData;
	unsigned char **    slitData;
	
	int                 nFramesToRecord;
	int                 maxFramesToRecord;
	int                 nRecorded;
	
	int horizPw, horizPh;
	unsigned char * horizP;
	ofTexture		horizPtex;
	
	ofTexture delayed;
	ofTexture * videoTextures;
	
	
	unsigned char * slitPx;
	ofTexture		slitTex;
	
	
	bool			reverse;
	
        // array of portions;
	struct Portion **   strPortions;
	int                 maxPortions;
    
	int imgW, imgH, imgColors;
	int pW, pH, pMod;
	int zoom;
	
	int nPortions;	
	
	bool	drawVert;
public:
	
	SlitSprayer();
	
	// ---------------------------------------
	void init(float w, float h);
	void updatePixels(ofxCvColorImage &img);
	void update();	
	void draw();
	
	void setReverse(bool b){ reverse = b;};
	void setNPortions(int n){ nPortions = n;};
	void setFramesToRecord(int n){ nFramesToRecord = n;}
	void setZoom(int z){ zoom = z;};
	void setVertical(bool b){ drawVert = b;};
	int getMaxPortions(){	return maxPortions;};
	int getMaxFramesToRecord(){ return maxFramesToRecord;};
};















