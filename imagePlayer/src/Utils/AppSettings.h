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
	int APP_SLIDESHOW_DURATION;
	int APP_SLIDESHOW_MILLIS_BETWEEN_SLIDES;
	
	AppSettings() {
	}
	
	void loadSettings(string file) {
		if(xml.loadFile("app_settings.xml")) {
			APP_W = xml.getValue("APP_W", 1024);
			APP_H = xml.getValue("APP_H", 768);
			APP_SLIDESHOW_DURATION = xml.getValue("APP_SLIDESHOW_DURATION", 20);
			APP_SLIDESHOW_MILLIS_BETWEEN_SLIDES = xml.getValue("APP_SLIDESHOW_MILLIS_BETWEEN_SLIDES", 1050);
		}
		
	}
};


// so other apps can see it...
extern AppSettings settings;