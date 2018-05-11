//
//  Camera.cpp
//  of-terrain
//
//  Created by Craig on 4/29/18.
//

#include "Camera.hpp"
//I'll need to pass the starting point of the rocketship to know where to point the first camera
void Camera::setup()
{
	floatingFollowPlayer = false;

	floatingCam = new ofEasyCam();
	fixedCam1 = new ofEasyCam();
	fixedCam2 = new ofEasyCam();
	sideRocketCam = new ofEasyCam();
	botRocketCam = new ofEasyCam();

	cams.push_back(floatingCam);
	cams.push_back(fixedCam1);
	cams.push_back(fixedCam2);
	cams.push_back(sideRocketCam);
	cams.push_back(botRocketCam);

	for (int i = 0; i < cams.size(); i++) {
		cams.at(i)->setNearClip(.1);
		cams.at(i)->setFov(65.5);   // approx equivalent to 28mm in 35mm format
		cams.at(i)->disableMouseInput();
		cams.at(i)->setDistance(20);
	}

	floatingCam->enableMouseInput();

	fixedCam1->setPosition(ofVec3f(-10, 3, -10));
	fixedCam1->setTarget(ofVec3f(0, 3, 0));

	fixedCam2->setPosition(ofVec3f(5, 3, -10));
}

void Camera::toggleFollowPlayer() {
	floatingFollowPlayer = !floatingFollowPlayer;
}

void Camera::update(ofVec3f playerLoc) {

	fixedCam2->setTarget(playerLoc);

	ofVec3f sideLoc = playerLoc + ofVec3f(0, 1, -2);
	ofVec3f sideTarget = playerLoc + ofVec3f(0, 0.75, 0);
	sideRocketCam->setPosition(sideLoc);
	sideRocketCam->setTarget(sideTarget);

	ofVec3f botLoc = playerLoc + ofVec3f(0, 0, -1);
	ofVec3f botTarget = botLoc + ofVec3f(0, -1, 0.05);
	botRocketCam->setPosition(botLoc);
	botRocketCam->setTarget(botTarget);

	if(floatingFollowPlayer) floatingCam->setTarget(playerLoc);
}

void Camera::draw() {
	/*
	floatingCam.begin();
	fixedCam1.begin();
	fixedCam2.begin();
	sideRocketCam.begin();
	botRocketCam.begin();

	floatingCam.end();
	fixedCam1.end();
	fixedCam2.end();
	sideRocketCam.end();
	botRocketCam.end();*/

}