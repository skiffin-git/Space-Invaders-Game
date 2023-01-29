#pragma once

#include "ofMain.h"

class Particle {
public:

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f force;
	float	damping;
	float   mass;
	float   lifespan;
	float   age;
	float	currentTime;
	float	radious;
	float	scale;
	ofImage explosionSprite;

	ofColor color;

	Particle(glm::vec3 pos, glm::vec3 force, glm::vec3 velocity, float bomb) {

		if (!explosionSprite.load("images/explosion.png")) {
			ofLogFatalError("can't load image: images/explosion.png");
			ofExit();
		}

		// initialize particle with some reasonable values first;
		//
		this->velocity = velocity;
		acceleration.set(0, 0, 0);
		position.set(pos);
		this->force = force;
		age = 0;
		lifespan = 1;
		damping = 0.98;
		radious = 5.0;
		scale = 0.3;
		if (!bomb) {
			color = ofColor(255, ofRandom(0, 255), 0);
		}
		else {
			color = ofColor(0, ofRandom(0, 255), ofRandom(0, 255));
		}
		
	}

	// Implement physics
	void update() {
		float dt = ofGetLastFrameTime();

		// Update time
		age += dt;

		// Update position
		ofVec3f accel = acceleration;
		accel += force * (1.0 / mass);
		position += (velocity * dt);
		velocity += accel * dt;

		// Damping
		velocity *= damping;

		// Zero out forces
		force.set(0, 0, 0);
	}


	void draw() {
		ofSetColor(color);
		explosionSprite.resize(150 * scale, 150 * scale);
		glm::vec3 newPos = glm::vec3(position.x - explosionSprite.getWidth() / 2, position.y - explosionSprite.getHeight() / 2, 0);
		explosionSprite.draw(newPos);
		//ofDrawSphere(position, radious);
	}

};


