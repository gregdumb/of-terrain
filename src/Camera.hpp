//
//  Camera.hpp
//  of-terrain
//
//  Created by Craig on 4/29/18.
//

#pragma once

#include <stdio.h>
#include "ofMain.h"

class Camera {
public:
	vector<ofEasyCam*> cams;
	void setup();
	void draw();

	void update(ofVec3f playerLoc);

	void toggleFollowPlayer();

private:
	ofEasyCam* floatingCam;
	ofEasyCam* fixedCam1;
	ofEasyCam* fixedCam2;
	ofEasyCam* sideRocketCam;
	ofEasyCam* botRocketCam;

	bool floatingFollowPlayer;
};


