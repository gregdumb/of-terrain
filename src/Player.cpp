#include "Player.h"
#include <algorithm>

Player::Player(Octree* octree) {

	this->octree = octree;

	// Mesh
	playerMesh = new ofxAssimpModelLoader();
	playerMesh->loadModel("ship.obj");
	playerMesh->setScaleNormalization(false);

	// HUD
	
	// Physics
	position = ofVec3f(0.1, 1, 0.1);
	gravity = ofVec3f(0, -0.1, 0);
	mass = 1;

	altitude = 0;
	touchingFloor = false;

	groundPoints.push_back(ofVec3f(0, -0.3, 0));  // Center
	groundPoints.push_back(ofVec3f(0, -0.35, 1)); // Front
	groundPoints.push_back(ofVec3f(0, -0.15, -1.1)); // Back

	// Sound
	thrusterSoundPlayer.load("thrusters.wav");
	thrusterSoundPlayer.setLoop(true);

	// Particle System
	float maxT = 10;
	TurbulenceForce* tf = new TurbulenceForce(ofVec3f(-maxT, 0, -maxT), ofVec3f(maxT, 0, maxT));

	ps = new ParticleEmitter();
	ps->type = EmitterType::RadialEmitter;
	ps->visible = false;
	ps->setRate(100);
	ps->particleRadius = 0.025;
	ps->setLifespan(1);
	ps->setVelocity(ofVec3f(0, 0, 0));
	ps->particleColor = ofColor(0, 255, 255);
	ps->sys->addForce(tf);

	psOffset = ofVec3f(0, -0.1, 0); // ofVec3f(0, 0, -1);
}


Player::~Player() {
}

void Player::startThrust() {
	if (!ps->started) ps->start();
	if (!thrusterSoundPlayer.isPlaying()) thrusterSoundPlayer.play();
}

void Player::stopThrust() {
	if (ps->started) ps->stop();
	if (thrusterSoundPlayer.isPlaying()) thrusterSoundPlayer.stop();
}

ofVec3f Player::getPosition() {
	return position;
}

void Player::addForce(ofVec3f f) {
	inputForce += f;
}

void Player::clearForce() {
	inputForce= ofVec3f(0, 0, 0);
}

void Player::update() {
	// Perform integration
	force = inputForce + (touchingFloor ? ofVec3f(0, 0, 0) : gravity);
	acceleration = force * mass;
	velocity = (velocity + acceleration * deltaTime()) * 0.99f;
	position = position + velocity * deltaTime();

	vector<float> altitudes;

	// Check collision & altitude
	for (ofVec3f gp : groundPoints) {
		ofVec3f startLoc = position + gp;
		Ray ray = Ray(Vector3::vectorify(startLoc + ofVec3f(0, 10, 0)), Vector3(0, -1, 0));
		vector<Node*> hitNodes = octree->checkIntersection(ray);

		if (hitNodes.size() > 0) {
			ofVec3f hitLoc = hitNodes.at(0)->getCenter();
			float height = startLoc.y - hitLoc.y;
			altitudes.push_back(height);
		}
	}

	if (altitudes.size() > 0) {
		auto minAlt = std::min_element(altitudes.begin(), altitudes.end());
		altitude = *minAlt;
	}
	else {
		altitude = 99999;
	}

	if (altitude <= 0) {
		touchingFloor = true;
		float up = 3 * deltaTime();
		if (up > -altitude) up = -altitude;

		position.y += up;
	}
	else touchingFloor = false;
}

void Player::draw() {
	// f = m * a, a = f/m
	// v = v + a*t
	// x = x + v*t

	playerMesh->setPosition(position.x, position.y, position.z);
	playerMesh->drawFaces();

	// Particle system
	ps->setPosition(position + psOffset);
	ps->update();
	ps->draw();

	cout << "Altitude: " << altitude << endl;
	// HUD
}

float Player::deltaTime() {
	float lastFrameTime = ofGetLastFrameTime();

	if (lastFrameTime < 1) return lastFrameTime;
	else return 0;
}