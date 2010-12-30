/***********************************************************************
 -----------------------------------
 
 Copyright (c) 2009, Memo Akten, www.memo.tv
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ***********************************************************************/

#pragma once

#include "ofxMSASplineTemplate.h"

float fabs(const ofxVec2f& v);


/**************************** 2D Spline (of ofxVec2) ****************************/
class ofxMSASpline2D : public ofxMSASpline<ofxVec2f> {
public:
	void drawRaw(int dotSize = 20, int lineWidth = 4){
		int numItems = size();
		
		if(lineWidth) {
			glLineWidth(lineWidth);
			glBegin(GL_LINE_STRIP);
			for(int i=0; i<numItems; i++) {
				float x = at(i).x;
				float y = at(i).y;
				glVertex2f(x, y);
			}
			if(lineWidth) glEnd();
		}
		
		if(dotSize) {
			glPointSize(dotSize);
			glBegin(GL_POINTS);
			for(int i=0; i<numItems; i++) {
				float x = at(i).x;
				float y = at(i).y;
				glVertex2f(x, y);
			}
			if(lineWidth) glEnd();
		}
		
	}
	
	
	void drawSmooth(int numSteps, int dotSize = 8, int lineWidth = 2) {
		if(size() < 4) return;
		
		float spacing = 1.0/numSteps;
		
		if(lineWidth) {
			glLineWidth(lineWidth);
			glBegin(GL_LINE_STRIP);
			for(float f=0; f<1; f+= spacing) {
				ofxVec2f v = sampleAt(f);
				glVertex2f(v.x, v.y);
			}
			glEnd();
		}
		
		if(dotSize) {
			glPointSize(dotSize);
			glBegin(GL_POINTS);
			for(float f=0; f<1; f+= spacing) {
				ofxVec2f v = sampleAt(f);
				glVertex2f(v.x, v.y);
			}
			glEnd();
		}
		
		glPointSize(1.0);
		glLineWidth(1.0);
	}
};



