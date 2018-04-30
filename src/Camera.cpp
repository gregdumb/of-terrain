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
	cams.push_back(floatingCam);
	cams.push_back(fixedCam1);
	cams.push_back(fixedCam2);
	cams.push_back(sideRocketCam);
	cams.push_back(botRocketCam);

	for (int i = 0; i < cams.size(); i++) {
		cams.at(i).setNearClip(.1);
		cams.at(i).setFov(65.5);   // approx equivalent to 28mm in 35mm format
		cams.at(i).disableMouseInput();
		cams.at(i).setDistance(20);
	}

}

void Camera::draw() {
	floatingCam.begin();
	fixedCam1.begin();
	fixedCam2.begin();
	sideRocketCam.begin();
	botRocketCam.begin();

	floatingCam.end();
	fixedCam1.end();
	fixedCam2.end();
	sideRocketCam.end();
	botRocketCam.end();

}