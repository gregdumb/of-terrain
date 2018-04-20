#pragma once

#ifndef _BOX_H_
#define _BOX_H_

#include <assert.h>
#include "vector3.h"
#include "ray.h"

#include "ofMain.h"

/*
 * Axis-aligned bounding box class, for use with the optimized ray-box
 * intersection test described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */

class Box {
  public:
    Box() { }
    Box(const Vector3 &min, const Vector3 &max) {
      assert(min < max);
      parameters[0] = min;
      parameters[1] = max;
    }
    // (t0, t1) is the interval for valid hits
    bool intersect(const Ray &, float t0, float t1) const;

    // corners
    Vector3 parameters[2];
	Vector3 min() { return parameters[0]; }
	Vector3 max() { return parameters[1]; }

	static void subDivideBox8(const Box* box, vector<Box*> & boxList) {
		Vector3 min = box->parameters[0];
		Vector3 max = box->parameters[1];
		Vector3 size = max - min;
		Vector3 center = size / 2 + min;
		float xdist = (max.x() - min.x()) / 2;
		float ydist = (max.y() - min.y()) / 2;
		float zdist = (max.z() - min.z()) / 2;
		Vector3 h = Vector3(0, ydist, 0);

		//  generate ground floor
		//
		Box* b[8];
		b[0] = new Box(min, center);
		b[1] = new Box(b[0]->min() + Vector3(xdist, 0, 0), b[0]->max() + Vector3(xdist, 0, 0));
		b[2] = new Box(b[1]->min() + Vector3(0, 0, zdist), b[1]->max() + Vector3(0, 0, zdist));
		b[3] = new Box(b[2]->min() + Vector3(-xdist, 0, 0), b[2]->max() + Vector3(-xdist, 0, 0));

		//boxList->clear();
		for (int i = 0; i < 4; i++)
			boxList.push_back(b[i]);

		// generate second story
		//
		for (int i = 4; i < 8; i++) {
			b[i] = new Box(b[i - 4]->min() + h, b[i - 4]->max() + h);
			boxList.push_back(b[i]);
		}
	}

	static vector<int> vertsInside(Box* box, ofMesh* mesh, vector<int> * verts = nullptr) {
		int n = 0;
		if (verts) {
			n = verts->size();
		}
		else {
			n = mesh->getNumVertices();
		}

		// Points inside
		vector<int> vect;

		float maxX = box->max().x(); // MAX(box->max().x(), box->min().x());
		float maxY = box->max().y(); // MAX(box->max().y(), box->min().y());
		float maxZ = box->max().z(); // MAX(box->max().z(), box->min().z());

		float minX = box->min().x(); // MIN(box->max().x(), box->min().x());
		float minY = box->min().y(); // MIN(box->max().y(), box->min().y());
		float minZ = box->min().z(); // MIN(box->max().z(), box->min().z());

		//ofVec3f maxCorner = ofVec3f(maxX, maxY, maxZ);
		//ofVec3f minCorner = ofVec3f(minX, minY, minZ);

		for (int i = 0; i < n; i++) {
			ofVec3f v;
			if (verts) {
				v = mesh->getVertex(verts->at(i));
				//cout << "Yes" << endl;
			}
			else {
				v = mesh->getVertex(i);
				//cout << "No vertexes passed" << endl;
			}

			if (v.x > minX && v.y > minY && v.z > minZ &&
				v.x < maxX && v.y < maxY && v.z < maxZ) {

				vect.push_back(i);
			}
		}

		return vect;
	}
};

#endif // _BOX_H_
