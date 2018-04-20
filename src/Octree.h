#pragma once
#include "ofMain.h"
#include "box.h"


class Node {

public:
	Box* box = nullptr;

	ofMesh* mesh = nullptr;
	vector<int> verts;

	Node* children[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	int depth = 0;

	Node(Box* newBox, int newDepth, ofMesh* newMesh, bool checkAll, vector<int> * possibleVerts = nullptr) {
		box = newBox;
		mesh = newMesh;
		depth = newDepth;

		int p = (possibleVerts) ? possibleVerts->size() : 0;
		cout << "Possible: " << p << endl;

		// Find verts that are inside this box
		if (p > 0 || checkAll) {
			verts = Box::vertsInside(box, mesh, possibleVerts);
			cout << "  Actual: " << verts.size() << endl;
		}

		if (verts.size() > 0 && depth > 0) {
			// Subdivide box into children
			// For each child box:
			//   children[i] = new Node(childbox[i], depth - 1)
			vector<Box*> childBoxes;
			Box::subDivideBox8(box, childBoxes);

			for (int i = 0; i < 8; i++) {
				vector<int> * vertPointer = new vector<int>(verts);
				children[i] = new Node(childBoxes.at(i), depth - 1, mesh, false, vertPointer);
			}
		}
	}

	bool isLeaf() {
		return children[0] == nullptr;
	}

	void draw() {
		drawBox();

		if (!isLeaf()) {
			for (Node* child : children) {
				child->draw();
			}
		}
	}


	void drawBox() {
		if (box) {
			//cout << "Drawing box" << endl;
			Vector3 min = box->parameters[0];
			Vector3 max = box->parameters[1];
			//cout << "  " << min.x() << " " << min.y() << " " << min.z() << endl;
			Vector3 size = max - min;
			Vector3 center = size / 2 + min;
			ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
			float w = size.x();
			float h = size.y();
			float d = size.z();
			ofSetColor(Node::getColor(depth));
			ofDrawBox(p, w, h, d);
		}
		else {
			cout << "Box was missing!" << endl;
		}
	}

	static ofColor getColor(int d) {
		if (d == 0) return ofColor::blue;
		if (d == 1) return ofColor::crimson;
		if (d == 2) return ofColor::coral;
		if (d == 3) return ofColor::fuchsia;
		if (d == 4) return ofColor::bisque;
		if (d == 5) return ofColor::darkSalmon;
		if (d == 6) return ofColor::cornsilk;
		if (d == 7) return ofColor::lavender;
		if (d == 8) return ofColor::gainsboro;
		if (d == 9) return ofColor::lightPink;
		else return ofColor::red;
	}
};

class Octree {

	Node* head;

	Octree(int depth) {

	}
};