#pragma once
#include "ofMain.h"
#include "del_interface.hpp"
using namespace tpp;


static bool InsidePolygon(ofPoint *polygon,int N, ofPoint p) {
	int counter = 0;
	int i;
	double xinters;
	ofPoint p1,p2;
	
	p1 = polygon[0];
	for (i=1;i<=N;i++) {
		p2 = polygon[i % N];
		if (p.y > MIN(p1.y,p2.y)) {
			if (p.y <= MAX(p1.y,p2.y)) {
				if (p.x <= MAX(p1.x,p2.x)) {
					if (p1.y != p2.y) {
						xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
						if (p1.x == p2.x || p.x <= xinters)
							counter++;
					}
				}
			}
		}
		p1 = p2;
	}
	
	if (counter % 2 == 0)
		return false;
	else
		return true;
}

// ---------------------------------------------------------
class DelaunayTriangle {
public:
	
	ofPoint a, b, c;
	DelaunayTriangle() {
	}
	
};

// ---------------------------------------------------------
class ofxDelaunay : public ofBaseDraws {
	
private:
	
	float width, height;
	vector <ofPoint>			temp;
	vector <Delaunay::Point>	v;	
	vector <DelaunayTriangle>	triangles;
	bool						bTriangles;
	
public:
	
	
	
	// ---------------------------------------------------------
	ofxDelaunay() {
	}
	
	// ---------------------------------------------------------
	float getWidth()  { return width;  }
	float getHeight() { return height; }
	void  setSize(float w, float h) {
		width = w; height = h;
	}
	
	// ---------------------------------------------------------
	int numTriangles() {
		return (int)triangles.size();
	}
	
	// ---------------------------------------------------------
	DelaunayTriangle& getTriangle(int i) {
		return triangles[i];
	}
	
	void scalePoint(float inX, float inY, float outX, float outY) {
		for(int i=0; i<triangles.size(); i++) {
			
			triangles[i].a.x = ofMap(triangles[i].a.x, 0.0, inX, 0.0, outX);
			triangles[i].a.y = ofMap(triangles[i].a.y, 0.0, inY, 0.0, outY);
			
			triangles[i].b.x = ofMap(triangles[i].b.x, 0.0, inX, 0.0, outX);
			triangles[i].b.y = ofMap(triangles[i].b.y, 0.0, inY, 0.0, outY);
			
			triangles[i].c.x = ofMap(triangles[i].c.x, 0.0, inX, 0.0, outX);
			triangles[i].c.y = ofMap(triangles[i].c.y, 0.0, inY, 0.0, outY);
			
		}
	}
	
	// ---------------------------------------------------------
	void findTriangles(vector <ofPoint> &pointsIn) {
		
		triangles.clear();
		v.clear();
		
		//find the triagles
		if(pointsIn.size() >= 3) {
			
			ofPoint tmpPts[pointsIn.size()];
			for(int i=0; i<pointsIn.size(); i++) tmpPts[i] = pointsIn[i];
			
			
			Delaunay::Point tempP;
			for(int i=0; i<pointsIn.size(); i++) {
				tempP[0] = pointsIn[i].x;
				tempP[1] = pointsIn[i].y;
				v.push_back(tempP);
			}
			
			Delaunay delobject(v);
			delobject.Triangulate();
			
			ofPoint verts[3];
			int count = 0;
			
			for(Delaunay::fIterator fit=delobject.fbegin(); fit!=delobject.fend(); ++fit) {
				
				int pta = delobject.Org(fit);
				int ptb = delobject.Dest(fit);
				int ptc = delobject.Apex(fit);
				
				
				if(pta == -1 || ptb == -1 || ptc == -1) {
					printf("don't have a triangle man!\n");
					continue;
				}
				
				
				ofPoint center;
				
				center.x += pointsIn[pta].x;
				center.y += pointsIn[pta].y;
				
				center.x += pointsIn[ptb].x;
				center.y += pointsIn[ptb].y;					
				
				center.x += pointsIn[ptc].x;
				center.y += pointsIn[ptc].y;					
				
				center.x /= 3;
				center.y /= 3;
				
				if(!InsidePolygon(tmpPts, pointsIn.size(), center)) continue;
				
				ofPoint a = pointsIn[pta];
				ofPoint b = pointsIn[ptb];
				ofPoint c = pointsIn[ptc];
				
				float area = delobject.area(fit);
				
				float disA  = ofDist(center.x, center.y, a.x, a.y);
				float disB  = ofDist(center.x, center.y, b.x, b.y);
				float disC  = ofDist(center.x, center.y, c.x, c.y);
				
				if(area > 2.0 && 
				   disA > 2.0 &&
				   disB > 2.0 &&
				   disC > 2.0) {
					
					// add the new triangle to the group
					DelaunayTriangle tri;
					tri.a = a; tri.b = b; tri.c = c;
					triangles.push_back(tri);
					
					/*ofEnableAlphaBlending();
					 ofFill();
					 ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 100);
					 ofTriangle(ofMap(a.x, 0, cvWidth, 0.0, ofGetWidth()), 
					 ofMap(a.y, 0, cvHeight, 0.0, ofGetHeight()),
					 ofMap(b.x, 0, cvWidth, 0.0, ofGetWidth()),
					 ofMap(b.y, 0, cvHeight, 0.0, ofGetHeight()),
					 ofMap(c.x, 0, cvWidth, 0.0, ofGetWidth()),
					 ofMap(c.y, 0, cvHeight, 0.0, ofGetHeight()));
					 
					 ofNoFill();
					 ofSetColor(10, 10, 100, 100);
					 ofTriangle(ofMap(a.x, 0, cvWidth, 0.0, ofGetWidth()), 
					 ofMap(a.y, 0, cvHeight, 0.0, ofGetHeight()),
					 ofMap(b.x, 0, cvWidth, 0.0, ofGetWidth()),
					 ofMap(b.y, 0, cvHeight, 0.0, ofGetHeight()),
					 ofMap(c.x, 0, cvWidth, 0.0, ofGetWidth()),
					 ofMap(c.y, 0, cvHeight, 0.0, ofGetHeight()));
					 
					 
					 ofNoFill();
					 ofSetColor(10, 10, 10, 100);
					 ofCircle(ofMap(a.x, 0, cvWidth, 0.0, ofGetWidth()),
					 ofMap(a.y, 0, cvHeight, 0.0, ofGetHeight()), 3);
					 ofNoFill();
					 ofSetColor(10, 10, 10, 100);
					 ofCircle(ofMap(b.x, 0, cvWidth, 0.0, ofGetWidth()),
					 ofMap(b.y, 0, cvHeight, 0.0, ofGetHeight()), 3);
					 ofNoFill();
					 ofSetColor(10, 10, 10, 100);
					 ofCircle(ofMap(c.x, 0, cvWidth, 0.0, ofGetWidth()),
					 ofMap(c.y, 0, cvHeight, 0.0, ofGetHeight()), 3);
					 
					 */
					count ++;
					bTriangles = true;
				}
			}	
		}
		else {
			bTriangles = false;
		}
	}
	
	// ---------------------------------------------------------
	void draw(float x, float y) {
		draw(x, y, width, height);
	}
	
	void draw(float x, float y, float w, float h) {
		glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(w/width, h/height, 0.0);
		for(int i=0; i<triangles.size(); i++) {
			
			ofPoint a = triangles[i].a;
			ofPoint b = triangles[i].b;
			ofPoint c = triangles[i].c;
			
			ofEnableAlphaBlending();
			ofFill();
			ofSetColor(190, 90, 90, 100);
			ofTriangle(a.x, a.y,
					   b.x, b.y,
					   c.x, c.y);
			ofNoFill();
			ofSetColor(10, 10, 10);
			ofTriangle(a.x, a.y,
					   b.x, b.y,
					   c.x, c.y);
			
			ofDisableAlphaBlending();
		}
		glPopMatrix();
	}
	
};