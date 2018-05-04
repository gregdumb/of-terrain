#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"
#include "ParticleEmitter.h"

class Player
{
public:
	Player();
	~Player();

	ofxAssimpModelLoader* playerMesh;

	void draw();

	void addForce(ofVec3f f);
	void clearForce();

private:

	// Particle System
	ParticleEmitter* ps;
	ofVec3f psOffset;

	// Physics

	float mass;

	ofVec3f inputForce;
	ofVec3f gravity;

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f force;
};

