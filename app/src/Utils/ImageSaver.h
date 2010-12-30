#pragma once
#include "ofMain.h"
#include "ofxThread.h"


struct GrabbedImage {

	ofImage image;
	bool bPixelsLoaded;
	bool bImageSaved;
	
};


class ImageSaver : public ofxThread {

private:
	
	string			folderName;
	bool			bStartGrabbing;
	bool			bStartSaving;
	
	int				numImagesInSequence;
	int				count;
	int				frameCount, frameCountMax;
	GrabbedImage *	images;
	
public:
	
	string getTimeStamp() {
		//return ofToString(ofGetSeconds()) + 
//		ofToString(ofGetMinutes()) + 
//		ofToString(ofGetHours()) +
//		ofToString(ofGetDay()) +
//		ofToString(ofGetMonth()) +
//		ofToString(ofGetYear());
		
		std::ostringstream localOSS;
		string aPrettyStamp;
		
		localOSS << setw(4) << setfill('0') << ofGetYear() << "_"
			<< setw(2) << setfill('0') << ofGetMonth() << "_"
			<< setw(2) << setfill('0') << ofGetDay() << "_"
			<< setw(2) << setfill('0') << ofGetHours() << "_"
			<< setw(2) << setfill('0') << ofGetMinutes() << "_"
			<< setw(2) << setfill('0') << ofGetSeconds();
		
		aPrettyStamp = localOSS.str();		
		
		return aPrettyStamp;
	}
	
	//--------------------------
	ImageSaver() {
		bStartSaving = false;
		images = NULL;
		numImagesInSequence = 0;
		bStartGrabbing = false;
		count = 0;
		frameCount = 0;
	}
	
	//--------------------------
	void init(int numImages, int grabEveryFrame) {
		
		numImagesInSequence = numImages;
		frameCountMax		= grabEveryFrame;
		
		images = new GrabbedImage[numImages];
		
		for(int i=0; i<numImages; i++) {
			images[i].image.setUseTexture(false);
			images[i].bPixelsLoaded = false;
			images[i].bImageSaved = false;
		}
		
		bStartGrabbing = false;
		bStartSaving = false;
	}

	//--------------------------
	void grabAndSaveImages() {
		if(!bStartSaving && !bStartGrabbing) {
			
			frameCount = 0;
			count = 0;
			bStartGrabbing = true;
			bStartSaving = false;
			stop();
		}
	}
	
	//--------------------------
	void start(){
		startThread(true, false);
	}
	
	//--------------------------
	void stop(){
		stopThread();
	}
	
	//--------------------------
	void threadedFunction(){
		
		while(isThreadRunning() != 0) {
			if(lock()) {
				
				for(int i=0; i<numImagesInSequence; i++) {
					
					if(images[i].bPixelsLoaded) {
						std::ostringstream localOSS;
						string aPrettyFileNumber;
						
						localOSS << setw(6) << setfill('0') << i;
						
						aPrettyFileNumber = localOSS.str();	
						
						images[i].image.saveImage("capture/"+folderName+"/pic_"+aPrettyFileNumber+".jpg");
						images[i].bImageSaved = true;
					}
				}
				
				bool allDone = true;
				for(int i=0; i<numImagesInSequence; i++) {
					if(!images[i].bImageSaved) {
						allDone = false;
					}
				}	
				
				
				unlock();
				ofSleepMillis(1 * 1000);
			
				if(allDone) {
					
					for(int i=0; i<numImagesInSequence; i++) {
						images[i].bPixelsLoaded = false;
						images[i].bImageSaved = false;
					}					
					bStartGrabbing = false;
					bStartSaving = false;
					
					printf("done saving and turned off!\n");
					stop();
				}
			}
		}
	}

	//--------------------------
	bool updateInDraw() {
		
		
		if(bStartGrabbing) {
			
			if(frameCount >= frameCountMax) {	
				
				printf("grab\n");
				
				images[count].image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
				images[count].bPixelsLoaded = true;
				if(count >= numImagesInSequence-1) {
					
					// make a new folder -- only on a mac ;) 
					folderName = getTimeStamp();
					string cmd = "mkdir ../../../data/capture/"+folderName;
					system(cmd.c_str());
					
					bStartGrabbing = false;	
					printf("done\n");
					bStartSaving = true;
					start();
				}
				count ++;
				frameCount = 0;
			}
			frameCount ++;
			
		}
		
		return bStartSaving;
			
	}
	
	
	
};
