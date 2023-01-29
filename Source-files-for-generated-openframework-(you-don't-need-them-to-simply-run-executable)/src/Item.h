#pragma once
#include "Shape.h"
#include "ofImage.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/ofGraphics.h>
#include <app/ofAppRunner.h>
#include "Player.h"
#include "ofApp.h"
#include "ofxGui.h"

class Item : public Shape {
public:

	float speed = 100;
	bool goForward = false;
	bool goReverse = false;
	bool turnRight = false;
	bool turnLeft = false;
	bool showSprite = false;
	bool showHeading = false;
	bool draggable = false;
	float launchTime = 0;
	float life = 0;
	float age = 0;
	float SpawnRate = 0;
	glm::vec3 delta = glm::vec3(0, 0, 0);
	ofImage itemSprite;

	Item() {}
	Item(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		vert.push_back(p1);
		vert.push_back(p2);
		vert.push_back(p3);
	}

	glm::vec3 heading() {
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1));
		return rotation * glm::vec4(0, -1, 0, 1);
	}


	bool inside(float x, float y) {
		glm::vec3 newP = glm::inverse(transform()) * glm::vec4(x, y, 0, 1);
		glm::vec3 p1 = vert[0];
		glm::vec3 p2 = vert[1];
		glm::vec3 p3 = vert[2];
		glm::vec3 v1 = glm::normalize(p1 - newP);
		glm::vec3 v2 = glm::normalize(p2 - newP);
		glm::vec3 v3 = glm::normalize(p3 - newP);
		float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
		float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
		float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
		if (a1 < 0 && a2 < 0 && a3 < 0) return true;
		else return false;
	}

	glm::vec3 transformedVert(glm::vec3 vert) {
		glm::vec3 p1 = transform() * glm::vec4(vert, 1);
		return p1;
	}
	void draw() {

		// push tranformation to matrix
		ofPushMatrix();
		ofMultMatrix(transform());

		// draw sprite
		if (showSprite) {
			glm::vec3 newPos = glm::vec3(0 - itemSprite.getWidth() / 2, 0 - itemSprite.getHeight() / 2, 0);
			itemSprite.draw(newPos);
		}
		// else draw triangle
		else {
			ofDrawTriangle(vert[0], vert[1], vert[2]);
		}

		// pop matrix
		ofPopMatrix();

		// draw a heading line
		if (showHeading)
			ofDrawLine(pos, pos + heading() * 120.0);

	}

	void update(Player player) {
		// update age
		age += ofGetLastFrameTime();

		// rotate item
		rot += ofGetLastFrameTime() * speed;

		// agent movement
		if (goForward)
			pos += heading() * speed * ofGetLastFrameTime();
		if (goReverse)
			pos -= heading() * speed * ofGetLastFrameTime();
		if (turnLeft)
			rot -= speed * ofGetLastFrameTime();
		if (turnRight)
			rot += speed * ofGetLastFrameTime();

		// set item boundries
		if (pos.x < 0)
			pos.x = 0;
		if (pos.y < 50)
			pos.y = 50;
		if (pos.x > ofGetWidth())
			pos.x = ofGetWidth();
		if (pos.y > ofGetHeight())
			pos.y = ofGetHeight();
	}
};