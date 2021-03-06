
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Mars HiRise Project - startup scene
// 
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  You will use this source file (and include file) as a starting point
//  to implement assignment 5  (Parts I and II)
//
//  Please do not modify any of the keymappings.  I would like 
//  the input interface to be the same for each student's 
//  work.  Please also add your name/date below.

//  Please document/comment all of your work !
//  Have Fun !!
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"
#include "Util.h"

//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){

	shader.load("shadersGL3/shader");

	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bRoverLoaded = false;
	bTerrainSelected = true;
//	ofSetWindowShape(1024, 768);

	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();
	cameras.setup();
	cam = cameras.cams.at(0);

	// setup rudimentary lighting 
	initLightingAndMaterials();

	mars.loadModel("flats.obj", true);
	mars.setScaleNormalization(false);
	float scale = 2.5;

	boundingBox = new Box(
		Vector3(-13, -5, -13),
		Vector3(13, 5, 13)
	);
	
	cout << "Landscape has " << mars.getMesh(0).getNumVertices() << " verts" << endl;
	uint64_t startTime = ofGetElapsedTimeMillis();
	octree = new Octree(boundingBox, &mars.getMesh(0), 3);
	uint64_t endTime = ofGetElapsedTimeMillis();
	cout << "Octree build took " << endTime - startTime << " ms" << endl;
	
	ps = new ParticleEmitter();
	ps->setPosition(ofVec3f(0, 2, 0));
	ps->type = EmitterType::RadialEmitter;
	ps->setRate(20);
	ps->particleRadius = 0.1;
	ps->setLifespan(1);
	ps->setVelocity(ofVec3f(0, -1, 0));
	ps->particleColor = ofColor::blue;
	
	//ps->start();

	player = new Player(octree);

	font = new ofTrueTypeFont();
	font->loadFont("PROMETHEUS.ttf", 20);

	sun.setup();
	sun.enable();
	sun.setDirectional();
	sun.setAmbientColor(ofColor::yellow);
	sun.setDiffuseColor(ofColor(255, 239, 167));
	sun.setGlobalPosition(ofVec3f(10, 10, 10));
	sun.setGlobalOrientation(ofQuaternion(0.3826834, 0, 0, 0.9238795));
}

//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {

	player->update();

	player->clearForce();

	cameras.update(player->getPosition());

	// Update keys
	if (keys[OF_KEY_UP])      player->addForce(ofVec3f(0, 0, 1));
	if (keys[OF_KEY_DOWN])    player->addForce(ofVec3f(0, 0, -1));
	if (keys[OF_KEY_LEFT])    player->addForce(ofVec3f(1, 0, 0));
	if (keys[OF_KEY_RIGHT])   player->addForce(ofVec3f(-1, 0, 0));
	if (keys[GLFW_KEY_SPACE]) player->addForce(ofVec3f(0, 1, 0));

	if (keys[GLFW_KEY_SPACE]) player->startThrust();
	else                      player->stopThrust();
}
//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor(0, 42, 156), ofColor::black);

	ofSetColor(ofColor::white);
	string altStr = "ALTITUDE - " + to_string(player->altitude);
	font->drawString(altStr, 20, 40);

	cam->begin();
	ofPushMatrix();
	ofEnableLighting();
	mars.drawFaces();

	//sun.draw();
	
	//ofNoFill();
	//ofSetColor(ofColor::white);
	
	//octree->draw();
	
	player->draw();
	
	//ps->draw();
	
	ofPopMatrix();
	cam->end();
}

// 

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	keys[key] = true;

	switch (key) {
	case 'C':
	case 'c':
		cameras.toggleFollowPlayer();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'r':
		//cam.reset();
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		//cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case GLFW_KEY_SPACE:
		//octree->undraw();
		break;
	case '1':
		//Freefloating camera is index 0
		cam = cameras.cams.at(0);
		break;
	case '2':
		//Fixed position camera 1 is index 1
		cam = cameras.cams.at(1);
		break;
	case '3':
		//Fixed position camera 2 is index 2
		cam = cameras.cams.at(2);
		break;
	case '4':
		//Side Rocket cameras is index 3
		cam = cameras.cams.at(3);
		break;
	case '5':
		//Bottom rocket camera is index 4
		cam = cameras.cams.at(4);
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	keys[key] = false;

	switch (key) {
	
	case OF_KEY_ALT:
		//cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	if (bAltKeyDown) return;

    ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam->screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam->getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	uint64_t startTime = ofGetElapsedTimeMicros();

	octree->checkIntersection(ray);

	uint64_t endTime = ofGetElapsedTimeMicros();

	cout << "Selection took " << endTime - startTime << " microseconds" << endl;
}


//draw a box from a "Box" class  
//
void ofApp::drawBox(const Box &box) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
	float w = size.x();
	float h = size.y();
	float d = size.z();
	ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
	int n = mesh.getNumVertices();
	ofVec3f v = mesh.getVertex(0);
	ofVec3f max = v;
	ofVec3f min = v;
	for (int i = 1; i < n; i++) {
		ofVec3f v = mesh.getVertex(i);

		if (v.x > max.x) max.x = v.x;
		else if (v.x < min.x) min.x = v.x;

		if (v.y > max.y) max.y = v.y;
		else if (v.y < min.y) min.y = v.y;

		if (v.z > max.z) max.z = v.z;
		else if (v.z < min.z) min.z = v.z;
	}
	return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}


//
//  Select Target Point on Terrain by comparing distance of mouse to 
//  vertice points projected onto screenspace.
//  if a point is selected, return true, else return false;
//
bool ofApp::doPointSelection() {

	ofMesh mesh = mars.getMesh(0);
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	int nearestIndex = 0;

	bPointSelected = false;

	ofVec2f mouse(mouseX, mouseY);
	vector<ofVec3f> selection;

	// We check through the mesh vertices to see which ones
	// are "close" to the mouse point in screen space.  If we find 
	// points that are close, we store them in a vector (dynamic array)
	//
	for (int i = 0; i < n; i++) {
		ofVec3f vert = mesh.getVertex(i);
		ofVec3f posScreen = cam->worldToScreen(vert);
		float distance = posScreen.distance(mouse);
		if (distance < selectionRange) {
			selection.push_back(vert);
			bPointSelected = true;
		}
	}

	//  if we found selected points, we need to determine which
	//  one is closest to the eye (camera). That one is our selected target.
	//
	if (bPointSelected) {
		float distance = 0;
		for (int i = 0; i < selection.size(); i++) {
			ofVec3f point =  cam->worldToCamera(selection[i]);

			// In camera space, the camera is at (0,0,0), so distance from 
			// the camera is simply the length of the point vector
			//
			float curDist = point.length(); 

			if (i == 0 || curDist < distance) {
				distance = curDist;
				selectedPoint = selection[i];
			}
		}
	}
	return bPointSelected;
}

// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam->getZAxis(), point);

	if (rover.loadModel(dragInfo.files[0])) {
		rover.setScaleNormalization(false);
		rover.setScale(.005, .005, .005);
		rover.setPosition(point.x, point.y, point.z);
		bRoverLoaded = true;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam->screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam->getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

