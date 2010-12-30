/*
 *  PixelSprayer.cpp
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/27/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#include "PixelSprayer.h"

PixelSprayer::PixelSprayer() {
}

	// ---------------------------------------
void PixelSprayer::init(float w, float h) {
	
	width = w; height = h;
	
		// set particles to nothing
	count				= 0;
	fadeInSpeed			= 0.1;
	fadeOutSpeed		= 0.02;
	
	damping				= 0.6;
	bFadeIn				= false;
	bEnableMotionTrails = false;
	
	
	for(int i=0; i<NUM_PARTICLES; i++) {
		setParticleSize(i, ofRandom(2, 10));
		fadeTrigger[i] = false;
		bAlive[i]	   = false;
	}
	
		// Generate the VBO
	glGenBuffersARB(3, &vbo[0]);
	
		// color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, NUM_PARTICLES*4*sizeof(float), color, GL_STREAM_DRAW_ARB);
	
		// positions
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, NUM_PARTICLES*2*sizeof(float), pos, GL_STREAM_DRAW_ARB);
	
		// position history
		// need to figure out this math... urgh
		// glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[2]);
		// glBufferDataARB(GL_ARRAY_BUFFER_ARB, (NUM_PARTICLES*HISTORY_LENGTH)*2*sizeof(float), posHistory, GL_STREAM_DRAW_ARB);
	
	
		// load the VBO shader
	shader.loadShader("shaders/VBOShader");
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
		// textures
	ofDisableArbTex();
	ofImage tempImage;
	tempImage.loadImage("textures/dot.png");
	dotTexture.allocate(tempImage.getWidth(), tempImage.getHeight(), GL_RGBA);
	dotTexture.loadData(tempImage.getPixels(), tempImage.getWidth(), tempImage.getHeight(), GL_RGBA);	
	
	
	
}


	// ---------------------------------------
void PixelSprayer::setFadeOutSpeed(float amt) {
	if(amt > 0.0) fadeOutSpeed = amt;
}

void PixelSprayer::setDamping(float amt) {
	if(amt > 0.0) damping = amt;
}
void PixelSprayer::setFadeInMode(float speed, bool fadeIn) {
	if(fadeIn > 1.0) fadeIn = 1.0;
	if(fadeIn < 0.0) fadeIn = 0.0;
	bFadeIn = fadeIn; fadeInSpeed = speed;
}
void PixelSprayer::setCalcMotionTrial(bool bDoit) {
	bEnableMotionTrails = bDoit;
}
void PixelSprayer::setUseVectorField(bool buseit) {
	bUseField = buseit;
}
void PixelSprayer::setSizeScaleFactor(float fct) {
	sizeMagScale = fct;
}

	// ---------------------------------------
void PixelSprayer::update(vectorField &field) {
	
	ofSetVerticalSync(true);
	ofPoint vfrc;
	for(int i=0; i<NUM_PARTICLES; i++) {
		
		if(!bAlive[i]) continue;
		
		if(bUseField) {
			vfrc = field.getForceFromPos(pos[i][0], pos[i][1]);
			frc[i][0] += vfrc.x;
			frc[i][1] += vfrc.y;
		}
		
		for(int j=0; j<2; j++) {
			
				// verlets
			vel[i][j] += frc[i][j];
			pos[i][j] += vel[i][j];
			
				// damping
			vel[i][j] *= damping;
				// reset force
			frc[i][j] = 0;
			
			
				// the motion history
				//	posHistory[i][0][j] = pos[i][j];
				//	posHistory[i][1][j] = frc[i][j];
			if(bEnableMotionTrails) {
				posHistory[i][0][j] = pos[i][j];
				for(int k=HISTORY_LENGTH; k>0; k--) {
					posHistory[i][k][j] = posHistory[i][k-1][j];
				}
			}
		}
		
		
		
		/*for(int k=2; k<HISTORY_LENGTH; k++) {
		 
		 //posHistory[i][k][j] = posHistory[i][k-1][j];
		 float dx  = posHistory[i][k][0] - posHistory[i][k-2][0];
		 float dy  = posHistory[i][k][1] - posHistory[i][k-2][1];
		 float len = sqrt(dx*dx + dy*dy);
		 
		 posHistory[i][k][0] = posHistory[i][k-1][0] + (dx*3.0) / len;
		 posHistory[i][k][1] = posHistory[i][k-1][1] + (dy*3.0) / len;			
		 }*/
		
		
		
			// fade in/out alpha channel
		if(bFadeIn) {
			if(!fadeTrigger[i]) {
				color[i][3] += fadeInSpeed;
				if(color[i][3] >= 1.0) fadeTrigger[i] = true;
			}
			else if(fadeTrigger[i]) {
				if(color[i][3] > 0)		 color[i][3] *= fadeOutSpeed;
				if(color[i][3] < 0.01) {
					color[i][3] = 0;
					resetPixel(i);
				}
			}
		}
		else {
			if(color[i][3] > 0) color[i][3] *= fadeOutSpeed;
			if(color[i][3] <= 0.1) { 
				color[i][3] = 0;
				resetPixel(i);
			}
		}
		
	}
	
}

	// ---------------------------------------
void PixelSprayer::setParticleSize(int i, float size) {
	if(i >= 0 || i <= NUM_PARTICLES-1) particleSize[i] = size;
}

	// ---------------------------------------
void PixelSprayer::resetPixel(int i) {
	
		// im dead blaahhh
	bAlive[i] = false;
	
	for(int j=0; j<2; j++) {
		pos[i][j] = 0;
		frc[i][j] = 0;
		vel[i][j] = 0;
		
			// history
		if(bEnableMotionTrails) {
			for(int k=0; k<HISTORY_LENGTH; k++) {
				posHistory[i][k][j] = 0;
			}
		}
		
	}
	
	setParticleSize(i, 10);
	fadeTrigger[i] = false;	
}

	// ---------------------------------------
void PixelSprayer::setPixelParticle(float x, float y, float vx, float vy, float mag, float r, float g, float b) {
	
	resetPixel(count);
	bAlive[count] = true;
	
		//vel[count][0] = ofRandom(-1.0, 1.0);
		//vel[count][1] = ofRandom(-1.0, 1.0);
	
	fadeTrigger[count] = false;
	pos[count][0] = x;	pos[count][1] = y;
	frc[count][0] = vx;	frc[count][1] = vy;
	
		// history
	if(bEnableMotionTrails) {
		for(int k=0; k<HISTORY_LENGTH; k++) {
			posHistory[count][k][0] = x;
			posHistory[count][k][1] = y;
		}
	}
	
	color[count][0] = r;
	color[count][1] = g;
	color[count][2] = b;
	color[count][3] = bFadeIn ? 0.0 : 1.0;
	
	mag *= sizeMagScale;
	setParticleSize(count, ofRandom(5, mag));
	
	count ++;
	count %= NUM_PARTICLES; 
}

	// ---------------------------------------
void PixelSprayer::drawParticles() {
	
		//	fbo.begin();
	
	shader.setShaderActive(true); // Turn on the Shader
	
		// Get the attribute and bind it
	GLint pixel_loc = glGetAttribLocationARB(shader.shader, "particleSize");
	glVertexAttribPointerARB(pixel_loc, 1, GL_FLOAT, false, 0, particleSize);
	glBindAttribLocationARB(shader.shader, pixel_loc, "particleSize");
		// printf("Size Location: %i\n", pixel_loc);
	
	
		// Enable Sprites and texture replace
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);			// Enable Vertex Points
	glEnableClientState(GL_VERTEX_ARRAY);			// Enable Vertex
	glEnableClientState(GL_COLOR_ARRAY);			// Enable Color
	
	
		// Color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);	// Bind the color VBO
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, NUM_PARTICLES * 4 * sizeof(float), color);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	
		// pos
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);	// Bind the pos VBO
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, NUM_PARTICLES * 2 * sizeof(float), pos);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	
	
		// Enable the PixelSize Attribute
	glEnableVertexAttribArrayARB(pixel_loc);
	
	
	
	ofEnableAlphaBlending();
	
		// Additive Blending
	glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
		//	shader.setUniformVariable3f("theColor", 1.0, 1.0, 0.0);	// Set Shader to Yellow
	dotTexture.bind();
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);				// Draw Just Points
	dotTexture.unbind();
		//shader.setUniformVariable3f("theColor", 1.0, 1.0, 1.0);	// Set Shader to White
	ofSetColor(255, 255, 255);
		//glDrawArrays(GL_LINES, 0, NUM_PARTICLES);				// Draw Lines
	ofDisableAlphaBlending();
	
	
		// Clean up
		//glDisable(GL_DEPTH_TEST);
	glDisable(GL_POINT_SPRITE);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableVertexAttribArrayARB(pixel_loc);
	
		// bind other buffer
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	shader.setShaderActive(false);
	
		//fbo.end();
	
}

	// ---------------------------------------
void PixelSprayer::draw() {
	
	ofSetColor(0xffffff);
	drawParticles();
	
	if(bEnableMotionTrails) {
		ofEnableAlphaBlending();
		for(int i=0; i<NUM_PARTICLES; i++) {
			
			if(!bAlive[i]) continue;
			
			ofNoFill();
			glColor4f(color[i][0], color[i][1], color[i][2], color[i][3]);
				//glBegin(GL_POINTS);
			glBegin(GL_LINE_STRIP);
			for(int j=0; j<HISTORY_LENGTH; j++) {
				float px = posHistory[i][j][0];
				float py = posHistory[i][j][1]; 
				glVertex2f(px, py);
			}	
			glEnd();
		}	
		ofDisableAlphaBlending();
	}
	
	bool drawSlowWay = false;
	if(drawSlowWay) {
		ofEnableAlphaBlending();
		glBegin(GL_POINTS);
		for(int i=0; i<NUM_PARTICLES; i++) {
			glColor4f(color[i][0], color[i][1], color[i][2], color[i][3]);
			glVertex2f(pos[i][0], pos[i][1]);
		}
		glEnd();
		ofDisableAlphaBlending();
	}
	
}






