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
	vector<ofEasyCam> cams;
	void setup();
	void draw();

private:
	ofEasyCam floatingCam;
	ofEasyCam fixedCam1;
	ofEasyCam fixedCam2;
	ofEasyCam sideRocketCam;
	ofEasyCam botRocketCam;
};


