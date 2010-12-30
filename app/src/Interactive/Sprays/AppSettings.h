/*
 *  AppGlobals.h
 *  emptyExample
 *
 *  Created by Todd Vanderlin on 3/30/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofxXmlSettings.h"

class AppSettings {

protected:
	ofxXmlSettings xml;
public:
	
	int APP_W;
	int APP_H;
	
	int APP_NUMBER_FRAMES_TO_LOOK; 
	int APP_NUMBER_OF_FRAMES_TO_GRAB;
	int APP_GRAB_EVERY_NTH_FRAME;
	float APP_AVG_NUMBER_OF_FACES_NEEDED_TO_TRIGGER;
	bool APP_ENABLE_GRABBING;
	
	AppSettings() {
	}
	
	void loadSettings(string file) {
		if(xml.loadFile("app_settings.xml")) {
			APP_W = xml.getValue("APP_W", 1280);
			APP_H = xml.getValue("APP_H", 768);
			
			APP_NUMBER_FRAMES_TO_LOOK = xml.getValue("APP_NUMBER_FRAMES_TO_LOOK", 5);
			APP_NUMBER_OF_FRAMES_TO_GRAB = xml.getValue("APP_NUMBER_OF_FRAMES_TO_GRAB", 42);
			APP_GRAB_EVERY_NTH_FRAME = xml.getValue("APP_GRAB_EVERY_NTH_FRAME", 2);
			APP_AVG_NUMBER_OF_FACES_NEEDED_TO_TRIGGER = xml.getValue("APP_AVG_NUMBER_OF_FACES_NEEDED_TO_TRIGGER", 1.f);
			APP_ENABLE_GRABBING = xml.getValue("APP_ENABLE_GRABBING", 1);
		}
		
	}
};


// so other apps can see it...
extern AppSettings settings;