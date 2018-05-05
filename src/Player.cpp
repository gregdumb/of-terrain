#include "Player.h"


Player::Player(Octree* octree) {

	this->octree = octree;

	// Mesh
	playerMesh = new ofxAssimpModelLoader();
	playerMesh->loadModel("monkey.obj");
	playerMesh->setScaleNormalization(false);
	
	// Physics
	position = ofVec3f(0, 3, 0);
	gravity = ofVec3f(0, 0, 0);
	mass = 1;

	altitude = 0;

	// Particle System
	ps = new ParticleEmitter();
	ps->type = EmitterType::RadialEmitter;
	ps->setRate(20);
	ps->particleRadius = 0.1;
	ps->setLifespan(1);
	ps->setVelocity(ofVec3f(0, -1, 0));
	ps->particleColor = ofColor::orange;
	ps->start();

	psOffset = ofVec3f(0, -0.5, 0);
}


Player::~Player() {
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
	Ray ray = Ray(Vector3::vectorify(position), Vector3(0, -1, 0));

	vector<Node*> hitNodes = octree->checkIntersection(ray);
	if (hitNodes.size() > 0) {
		ofVec3f hitLoc = hitNodes.at(0)->getCenter();
		//cout << "Hit the floor at " << hitLoc.x << ", " << hitLoc.y << ", " << hitLoc.z << endl;

		float height = position.y - hitLoc.y;
		altitude = height;
	}
}

void Player::draw() {
	// f = m * a, a = f/m
	// v = v + a*t
	// x = x + v*t

	// Perform integration
	force = inputForce + gravity;
	acceleration = force * mass;
	velocity = (velocity + acceleration * ofGetLastFrameTime()) * 0.99f;
	position = position + velocity * ofGetLastFrameTime();

	playerMesh->setPosition(position.x, position.y, position.z);
	playerMesh->drawFaces();

	// Particle system
	ps->setPosition(position + psOffset);
	ps->update();
	ps->draw();

	cout << "Altitude: " << altitude << endl;
}