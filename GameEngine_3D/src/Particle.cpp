
//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith  
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>


#include "Particle.h"


Particle::Particle() {

	// initialize particle with some reasonable values first;
	//
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
	lifespan = 5;
	birthtime = 0;
	radius = .0001;
	damping = .99;
	mass = 1;
	visible = true;
	color = ofColor::aquamarine;
}

void Particle::draw() {
	if (visible) {
		ofSetColor(color);
		//	ofSetColor(ofMap(age(), 0, lifespan, 255, 10), 0, 0);
		ofDrawSphere(position, radius);
	}
}

// write your own integrator here.. (hint: it's only 3 lines of code)
//
void Particle::integrate() {

	// check for 0 framerate to avoid divide errors
	//
	float framerate = ofGetFrameRate();

	// avoid accumulating forces on startup before frame is drawn
	//
	if (framerate < 5.0)
	{
		forces.set(0, 0, 0);
		return;
	}

	// interval for this step
	//
	float dt = 1.0 / framerate;

	// update position based on velocity
	//
	position += (velocity * dt);
	//if (position.y < 0) {   //Ensures the particle does not go below the "surface"
	//	position.y = 0;
	//}

	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the particle
	accel += (forces * (1.0 / mass));
	velocity += accel * dt;


	// add a little damping for good measure
	//
	velocity *= damping;

	// clear forces on particle (they get re-added each step)
	//
	forces.set(0, 0, 0);
}

//  return age in seconds
//
float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime) / 1000.0;
}


