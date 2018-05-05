#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"
#include "ParticleEmitter.h"
#include "Octree.h"

class Player
{
public:
	Player(Octree* octree);
	~Player();

	ofxAssimpModelLoader* playerMesh;

	void update();
	void draw();

	ofVec3f getPosition();

	void addForce(ofVec3f f);
	void clearForce();

	float altitude;

private:

	Octree* octree;
	bool touchingFloor;

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

	float deltaTime();
};

