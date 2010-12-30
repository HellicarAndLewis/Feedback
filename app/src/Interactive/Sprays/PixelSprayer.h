/*
 *  PixelSprayer.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 3/27/10.
 *  Copyright 2010 Design & Technology. All rights reserved.
 *
 */

#pragma once 
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxShader.h"
#include "BaseSprayer.h"
#include "vectorField.h"

#define NUM_PARTICLES	10000
#define HISTORY_LENGTH	10

class PixelSprayer : public BaseSprayer {
	
protected:
	
		// vbo pixels
	GLuint				vbo[3];
	float				posHistory[NUM_PARTICLES][HISTORY_LENGTH][2];
	float				pos[NUM_PARTICLES][2];
	float				vel[NUM_PARTICLES][2];
	float				frc[NUM_PARTICLES][2];
	
	float				color[NUM_PARTICLES][4];
	float				particleSize[NUM_PARTICLES];
	
	bool				bAlive[NUM_PARTICLES];
	bool				fadeTrigger[NUM_PARTICLES];
	
	float				sizeMagScale;
	int					count;
	float				fadeOutSpeed;
	float				fadeInSpeed;
	float				damping;
	bool				bFadeIn;
	bool				bEnableMotionTrails;
	bool				bUseField;
	
		// textures / shaders
	ofxShader			shader;
	ofTexture			dotTexture;
	
	void drawParticles();
	void setParticleSize(int i, float size);
	
public:
	
	PixelSprayer();
	
		// ---------------------------------------
	void setFadeOutSpeed(float amt);
	void setDamping(float amt);
	void setFadeInMode(float speed, bool fadeIn);
	void setCalcMotionTrial(bool bDoit);
	void setUseVectorField(bool buseit);
	void setSizeScaleFactor(float fct);
		// ---------------------------------------
	void resetPixel(int i);
	void setPixelParticle(float x, float y, float vx, float vy, float mag, float r, float g, float b);
	
		// ---------------------------------------
	void init(float w, float h);
	void update(vectorField &field);	
	void draw();
	
};















