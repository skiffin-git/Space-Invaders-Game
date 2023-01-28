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


class Agent : public Shape {
public:

	// physics
	float thrust = 500;
	float restitution = 0.85;
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	glm::vec3 force = glm::vec3(0, 0, 0);
	glm::vec3 bounceForce = glm::vec3(0, 0, 0);
	float mass = 1.0;
	float damping = .99;
	float angularForce = 0;
	float angularVelocity = 0.0;
	float angularAcceleration = 0.0;
	bool bThrust = false;
	float prevDist = 0;

	//float oldSpeed = 0;
	//float speed = 100;
	bool goForward = false;
	bool goReverse = false;
	bool turnRight = false;
	bool turnLeft = false;
	bool showSprite = false;
	bool showHeading = false;
	bool draggable = false;
	bool stop1 = false;
	bool stop2 = false;
	float launchTime = 0;
	float life = 0;
	float age = 0;
	float SpawnRate = 0;
	glm::vec3 delta = glm::vec3(0, 0, 0);
	ofImage agentSprite;

	Agent() {}
	Agent(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
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
			glm::vec3 newPos = glm::vec3(0 - agentSprite.getWidth() / 2, 0 - agentSprite.getHeight() / 2, 0);
			agentSprite.draw(newPos);
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

	void update(Player player, int level) {
		// update age
		age += ofGetLastFrameTime();

		// -------------------------------------------------------

		// phycis
		// init current framerate (or you can use ofGetLastFrameTime())
		float framerate = ofGetFrameRate();
		float dt = 1.0 / framerate;

		// linear motion
		pos += (velocity * dt);
		glm::vec3 accel = acceleration;
		accel += ((force + bounceForce) * 1.0 / mass);
		bounceForce = glm::vec3(0, 0, 0);
		velocity += accel * dt;
		velocity *= damping;

		// angular motion
		/*
		rot += (dt * (angularVelocity));
		float a = angularAcceleration;;
		a += (angularForce * 1.0 / mass);
		angularVelocity += a * dt;
		angularVelocity *= damping;
		*/

		force = glm::vec3(0, 0, 0);
		angularForce = 0;

		// slip control
		//velocity += 20 * glm::cross(velocity, glm::vec3(0, 0, -angularVelocity)) / (float)thrust / (float)thrust;
		 // Slip control
		glm::vec3 rightVelocity = glm::length(velocity) * heading();
		glm::vec3 delta = (rightVelocity - velocity) / 20;
		velocity = velocity + delta;
		// --------------------------------------------------------

		// aim to player
		glm::vec3 relative = player.pos - pos;
		float r = ofRadToDeg(atan2(-relative.y, relative.x));
		if (r > 90) {
			r = 450 - r;
		}
		else {
			r = 90 - r;
		}
		rot = ofLerpDegrees(rot, r, 800 * ofGetLastFrameTime()/ 200);

		// fly to player
		if (age >= launchTime) {
			goForward = true;
		}
		else {
			rot = ofLerpDegrees(rot, r, 500 * ofGetLastFrameTime() / 200);
		}
		
		
		// agent movement
		if (goForward) {
			//pos += heading() * speed * ofGetLastFrameTime();
			force = (float)thrust * heading();
		}
		if (goReverse) {
			//pos -= heading() * speed * ofGetLastFrameTime();
			force = (float)thrust * heading();
		}
			
		if (turnLeft) {
			//rot -= speed * ofGetLastFrameTime();
			angularForce = thrust;
		}
			
		if (turnRight) {
			//rot += speed * ofGetLastFrameTime();
			angularForce = -thrust;
		}
			
		// set agent boundries
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