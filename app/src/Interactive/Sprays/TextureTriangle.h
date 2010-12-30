/*
 *  TextureTriangle.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/29/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofxBox2d.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "VideoFrame.h"

class TextureTriangle : public ofxBox2dPolygon {
	
public:
	
	float				alpha;
	bool				bFadeOut;
	int					frameId;
	vector <ofPoint>	launchPts;
	float				frameAge;
	VideoFrame *		frameRef;
	
	TextureTriangle() {
		alpha	 = 1.0;
		frameAge = 0;
		frameId  = -1;
		frameRef = NULL;
		bFadeOut = false;
	}
	
	void draw() {
		
		ofPoint pt = getCenter();
		
		if(body != NULL ) {
			
			if(frameRef) {
				glPushMatrix();
				glTranslatef(0, 0, 0);
				ofDisableArbTex();
				ofEnableAlphaBlending();
				ofSetColor(255, 255, 255, alpha*255.0);
				frameRef->img.getTextureReference().bind();
				glBegin(GL_TRIANGLES);
				for(int j=0; j<vertices.size(); j++) {
					glTexCoord2d(launchPts[j].x, launchPts[j].y);
					glVertex2d(vertices[j].x, vertices[j].y);
				}
				glEnd();
				glPopMatrix();
				frameRef->img.getTextureReference().unbind();
				ofDisableAlphaBlending();
				
			}
			
		}
	}
	
	
	void drawTextureTrianlge() {
		/*
		 glEnable(texData.textureTarget);
		 
		 // bind the texture
		 glBindTexture( texData.textureTarget, (GLuint)texData.textureName[0] );
		 
		 GLfloat px0 = 0;		// up to you to get the aspect ratio right
		 GLfloat py0 = 0;
		 GLfloat px1 = w;
		 GLfloat py1 = h;
		 
		 if (texData.bFlipTexture == true){
		 GLint temp = (GLint)py0;
		 py0 = py1;
		 py1 = temp;
		 }
		 
		 // for rect mode center, let's do this:
		 if (ofGetRectMode() == OF_RECTMODE_CENTER){
		 px0 = -w/2;
		 py0 = -h/2;
		 px1 = +w/2;
		 py1 = +h/2;
		 }
		 
		 //we translate our drawing points by our anchor point.
		 //we still respect ofRectMode so if you have rect mode set to
		 //OF_RECTMODE_CENTER your anchor will be relative to that.
		 GLfloat anchorX;
		 GLfloat anchorY;
		 
		 if(bAnchorIsPct){
		 anchorX = anchor.x * w;
		 anchorY = anchor.y * h;
		 }else{
		 anchorX = anchor.x;
		 anchorY = anchor.y;
		 }
		 
		 px0 -= anchorX;
		 py0 -= anchorY;
		 px1 -= anchorX;
		 py1 -= anchorY;
		 
		 
		 // -------------------------------------------------
		 // complete hack to remove border artifacts.
		 // slightly, slightly alters an image, scaling...
		 // to remove the border.
		 // we need a better solution for this, but
		 // to constantly add a 2 pixel border on all uploaded images
		 // is insane..
		 
		 GLfloat offsetw = 0;
		 GLfloat offseth = 0;
		 
		 if (texData.textureTarget == GL_TEXTURE_2D && bTexHackEnabled) {
		 offsetw = 1.0f / (texData.tex_w);
		 offseth = 1.0f / (texData.tex_h);
		 }
		 // -------------------------------------------------
		 
		 GLfloat tx0 = 0+offsetw;
		 GLfloat ty0 = 0+offseth;
		 GLfloat tx1 = texData.tex_t - offsetw;
		 GLfloat ty1 = texData.tex_u - offseth;
		 
		 glPushMatrix(); 
		 
		 glTranslatef(x,y,0.0f);
		 
		 GLfloat tex_coords[] = {
		 tx0,ty0,
		 tx1,ty0,
		 tx1,ty1,
		 tx0,ty1
		 };
		 GLfloat verts[] = {
		 px0,py0,
		 px1,py0,
		 px1,py1,
		 px0,py1
		 };
		 
		 glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		 glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
		 glEnableClientState(GL_VERTEX_ARRAY);		
		 glVertexPointer(2, GL_FLOAT, 0, verts );
		 glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
		 glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		 
		 glPopMatrix();
		 glDisable(texData.textureTarget);*/
	}
	
	
};