#pragma once
#include "ofMain.h"
#include "ofxThread.h"
#include "ofxDirList.h"

//struct ThreadedImage {
//	
//	string  filePath;
//	int		id;
//	ofImage image;
//	bool	bPixelsLoaded;
//	bool	bTextureLoaded;
//	bool	ready;
//	bool	dead;
//};

class ThreadedImage {
public:
	ThreadedImage(){
		image.allocate(10,10, OF_IMAGE_COLOR);
		image.setUseTexture(false);
		id				= 0;
		filePath		= "";
		bPixelsLoaded	= false;
		bTextureLoaded	= false;
		ready			= false;
	};
	
	virtual ~ThreadedImage(){}
	
	string  filePath;
	int		id;
	ofImage image;
	bool	bPixelsLoaded;
	bool	bTextureLoaded;
	bool	ready;
	bool	dead;
	
};

#define NUM_IMAGES 42

class ImageLoader : public ofxThread {
	
private:
	
	
	ofxDirList					fileLister;
	bool						bReadForMoreImages;
	bool						bLoadingImages;
	
	bool						allPixelsAreLoaded;
	bool						allTexturesAreLoaded;
	int							maxTexturesPerFrame;		
	int							numImages;
//	vector <ThreadedImage*>		images;
	ThreadedImage * images[NUM_IMAGES];	
	
	
public:
	
	string getTimeStamp() {
		return ofToString(ofGetSeconds()) + 
		ofToString(ofGetMinutes()) + 
		ofToString(ofGetHours()) +
		ofToString(ofGetDay()) +
		ofToString(ofGetMonth()) +
		ofToString(ofGetYear());
	}
	
	//--------------------------
	ImageLoader() {
		fileLister.reset();
		fileLister.allowExt("jpg");
		
		maxTexturesPerFrame  = 1;
		bReadForMoreImages   = true;
		bLoadingImages	     = false;
		allPixelsAreLoaded   = false;
		allTexturesAreLoaded = false;
		
		for(int i=0; i < NUM_IMAGES;i++){
			images[i] = new ThreadedImage();
		}		
	}
	
	// this bool is set to true when 
	// there is nothing else to load 
	// and all threads are stopped
	//--------------------------
	bool readyForMoreImages() {
		return bReadForMoreImages;
	}
	
	// You need to have all the pixels loaded
	// and to have all the textures uploaded 
	// before you can start to draw the iamges
	//--------------------------
	bool readyForDisplaying() {
		return allTexturesAreLoaded && allPixelsAreLoaded;
	}
	
	// this gives you a pointer to the threaded image
	// you need to check and see if the image is "ready"
	// then you can draw (see testApp::draw() )
	//--------------------------
	ThreadedImage* getImage(int i) {
		return images[i];	
	}
	
	// returns the total number of images
	int totalImages() {
		return NUM_IMAGES;
	}
	
	// just pass in a folder to look for images
	// they will then be loaded and the old images 
	// will be cleared out
	//--------------------------
	void loadFromDirectory(string path) {
		if(!bLoadingImages) {
			
			
			// remove all the old images
			for(int i=0; i< NUM_IMAGES; i++) {
				images[i]->dead = true;
			}
			
			bReadForMoreImages   = false;
			allPixelsAreLoaded   = false;
			allTexturesAreLoaded = false;
			
			numImages = fileLister.listDir(path);
//			cout << "Number of images is " << numImages << "in the path " << path << endl;
			
			if(numImages > 0) {
				
				for(int i=0; i<numImages && i < NUM_IMAGES; i++) {
//					images.push_back(new ThreadedImage());
					images[i]->image.clear();
					images[i]->image.setUseTexture(false);
					images[i]->id			  = 0;
					images[i]->filePath		  = fileLister.getPath(i);
					images[i]->bPixelsLoaded  = false;
					images[i]->bTextureLoaded = false;
					images[i]->ready		  = false;
				}
				
				printf("--- about to load:%i ---", numImages);
				bLoadingImages = true;
				start();
			}
		}
	}
	
	//--------------------------
	void init(int texturesPerFrame) {
		bReadForMoreImages = false;
		bLoadingImages	   = false;
		maxTexturesPerFrame = texturesPerFrame;
	}
	
	//--------------------------
	void update() {
		
		if(bLoadingImages && allPixelsAreLoaded && !allTexturesAreLoaded) {
			printf("-- start loading texures --\n");	
			
			int textureLoadCount = 0;
			for(int i=0; i < NUM_IMAGES; i++) {
				if(textureLoadCount >= maxTexturesPerFrame) {
					continue;
				}
				if(images[i]->bPixelsLoaded && !images[i]->bTextureLoaded) {
					
					images[i]->image.setUseTexture(true);
					images[i]->image.allocate(images[i]->image.getWidth(),
											  images[i]->image.getHeight(),
											  images[i]->image.type);
					
					images[i]->image.setFromPixels(images[i]->image.getPixels(),
												   images[i]->image.getWidth(),
												   images[i]->image.getHeight(),
												   images[i]->image.type);
					
					images[i]->bTextureLoaded = true;	
					images[i]->ready = true;
					textureLoadCount ++; 
				}
			}
			
			bool allDone = true;
			for(int i=0; i< NUM_IMAGES; i++) {
				if(!images[i]->bTextureLoaded) {
					allDone = false;
				}
			}
			if(allDone) {
				printf("-- all textures loaded --\n");	
			
				allTexturesAreLoaded = true;
				bLoadingImages = false;
				bReadForMoreImages = true;
			}
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
	void threadedFunction() {
		
		while(isThreadRunning() != 0) {
			if(lock()) {
				
				for(int i=0; i< NUM_IMAGES; i++) {
					if(images[i]->bPixelsLoaded == false) {
						images[i]->bPixelsLoaded = images[i]->image.loadImage(images[i]->filePath);
						if(images[i]->bPixelsLoaded) {
							//printf("--- pixels loaded:%s ---\n", images[i]->filePath.c_str());
						}
					}
				}
				
				bool allDone = true;
				for(int i=0; i< NUM_IMAGES; i++) {
					if(!images[i]->bPixelsLoaded) {
						allDone = false;
					}
				}
				
				unlock();
				ofSleepMillis(200);
				
				
				if(allDone) {
					stop();
					allPixelsAreLoaded = true;
					printf("--- all done loading pixles --\n");
				}
				
				
			}
		}
	}
	
	
	
	
};
