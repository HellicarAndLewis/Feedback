/*
 *  SlitSprayer.cpp
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 6/4/2010.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#include "SlitSprayer.h"

SlitSprayer::SlitSprayer() {
}

// ---------------------------------------
void SlitSprayer::init(float w, float h) {
	
	imgW = (int)w;
    imgH = (int)h;
    imgColors = 3;
    
    nRecorded = 0;
    
    nFramesToRecord = 40;
    
	nPortions = 10;
	
    maxFramesToRecord = 1000;
    maxPortions = imgH; // we can't have more portions than vertical pixels
    
		// if you want to render the slitscan bigger...
    zoom = 1;
	
	reverse = false;
    
		// video buffer --------------------------------------------------------
	
    videoData = new unsigned char *[maxFramesToRecord];
    for (int i = 0; i < maxFramesToRecord; i++){
        videoData[i] = new unsigned char[imgW*imgH*imgColors];
    }
	
	slitData = new unsigned char *[maxFramesToRecord];
    for (int i = 0; i < maxFramesToRecord; i++){
        slitData[i] = new unsigned char[imgW*imgH*imgColors];
    }
	
		//portions ---------------------------------------------------------------
    strPortions = new Portion * [maxPortions];
	
    for (int i = 0; i < maxPortions; i++){
        strPortions[i]     = new Portion;
        strPortions[i]->w  = imgW;
        
    }
	
		// vert 
	slitPx = new unsigned char [imgW * imgH * imgColors];
	slitTex.allocate(imgW, imgH, GL_RGB);
	
		// horiz ----
	horizPw = 20;
	horizPh = 240;
	horizP = new unsigned char [imgW*imgH*imgColors];
	horizPtex.allocate(imgW, imgH, GL_RGB);	
}

void SlitSprayer::updatePixels(ofxCvColorImage &img){
	memcpy(videoData[nRecorded], img.getPixels(), imgW*imgH*imgColors);
}

// ---------------------------------------
void SlitSprayer::update() {
	pW = imgW;
	pH = imgH / nPortions;
	
		// to fill the whole img height... 
		// if we have a remainder, we'll have to share it up between portions
	pMod = imgH % nPortions;
	
	
	int tempY    = imgH;
	int posInImg;
		//posInImg = imgW * imgH * imgColors; 
	if (!reverse) 
		posInImg = 0;
	else 
		posInImg = imgW * imgH * imgColors;	
	
	int tempSize, tempH;
	
		// delay in frames between portions
	
	int frameDiff =  (nFramesToRecord / nPortions); //  this is reversed so it goes from bottom to top.
	int portionFrame;		
	for (int i = 0; i < nPortions; i ++) {
		tempH = pH;
			// remainder correction. 
		if (pMod > 0) {
			tempH ++;
			pMod--;
		}
		
		tempSize  = pW * tempH * imgColors;
		
			// which frame of the videoData are we going to use for this portion?
		portionFrame = (nRecorded - (i*frameDiff))%nFramesToRecord;
		if (portionFrame < 0) portionFrame = nFramesToRecord +portionFrame;
		
		if (!reverse) {
			memcpy(slitPx + posInImg  , videoData[portionFrame]+  (posInImg), tempSize);			
			posInImg += tempSize;
		} else {
			posInImg -= tempSize;
			memcpy(slitPx + posInImg  , videoData[portionFrame]+  (posInImg), tempSize);
		}
		
			// horiz.
		horizPw = imgW/nPortions;
		for (int j = 0; j < horizPh; j++) {
			memcpy((horizPw*i *3) + horizP + j *imgW * 3, (horizPw*i *3) +videoData[portionFrame]+ j*imgW*3, horizPw * 3); 
				//memcpy((horizPw*i *3) + horizP + j *imgW * 3, (horizPw*i *3) +slitData[portionFrame]+ j*imgW*3, horizPw * 3);  
		}
		memcpy(slitData[nRecorded], slitPx, imgW*imgH*imgColors);
	}
	
	slitTex.loadData(slitPx, imgW, imgH, GL_RGB);
	horizPtex.loadData(horizP, imgW, imgH, GL_RGB);
	
	nRecorded++;
	nRecorded %= nFramesToRecord;
}

void SlitSprayer::draw() {	
	ofSetColor(255, 255, 255);
	
	ofRect(100,100,100,100);
	
	if(drawVert){
		slitTex.draw(0, 0, imgW*zoom, imgH*zoom);		
	}else{
		horizPtex.draw(0, 0, imgW*zoom, imgH*zoom);		
	}	
}






