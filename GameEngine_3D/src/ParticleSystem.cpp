

//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith  
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>

#include "ParticleSystem.h"

void ParticleSystem::add(const Particle &p) {
	particles.push_back(p);
}

void ParticleSystem::addForce(ParticleForce *f) {
	forces.push_back(f);
}

void ParticleSystem::remove(int i) {
	particles.erase(particles.begin() + i);
}

void ParticleSystem::setLifespan(float l) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].lifespan = l;
	}
}

void ParticleSystem::reset() {
	for (int i = 0; i < forces.size(); i++) {
		forces[i]->applied = false;
	}
}

void ParticleSystem::update() {
	// check if empty and just return
	if (particles.size() == 0) return;

	vector<Particle>::iterator p = particles.begin();
	vector<Particle>::iterator tmp;

	// check which particles have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, we need to use an iterator.
	//
	while (p != particles.end()) {
		if (p->lifespan != -1 && p->age() > p->lifespan) {
			tmp = particles.erase(p);
			p = tmp;
		}
		else p++;
	}

	// update forces on all particles first 
	//
	for (int i = 0; i < particles.size(); i++) {
		for (int k = 0; k < forces.size(); k++) {
			if (!forces[k]->applied)
				forces[k]->updateForce(&particles[i]);
		}
	}

	// update all forces only applied once to "applied"
	// so they are not applied again.
	//
	for (int i = 0; i < forces.size(); i++) {
		if (forces[i]->applyOnce)
			forces[i]->applied = true;
	}

	// integrate all the particles in the store
	//
	for (int i = 0; i < particles.size(); i++)
		particles[i].integrate();

}

// remove all particlies within "dist" of point (not implemented as yet)
//
int ParticleSystem::removeNear(const ofVec3f & point, float dist) { return 0; }

//  draw the particle cloud
//
void ParticleSystem::draw() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
}


// Gravity Force Field 
//
GravityForce::GravityForce(const ofVec3f &g) {
	gravity = g;
	
}

void GravityForce::updateForce(Particle * particle) {
	//
	// f = mg
	//
	if (!stop) {
		particle->forces += gravity * particle->mass;
	}
}

// Turbulence Force Field 
//
TurbulenceForce::TurbulenceForce(const ofVec3f &min, const ofVec3f &max) {
	tmin = min;
	tmax = max;
}

void TurbulenceForce::updateForce(Particle * particle) {
	//
	// We are going to add a little "noise" to a particles
	// forces to achieve a more natual look to the motion
	//
	particle->forces.x += ofRandom(tmin.x, tmax.x);
	particle->forces.y += ofRandom(tmin.y, tmax.y);
	particle->forces.z += ofRandom(tmin.z, tmax.z);

	if (stopTurbulence) { //I stop using turbulence when it "lands" again
		applyOnce = true;
	}
}

//Thruster force that simulates the rocket's thrust in a direction depending on velocity
ThrusterForce::ThrusterForce(const ofVec3f &dir) {
	direction = dir;
}

void ThrusterForce::updateForce(Particle *particle) {

	//
	//To make it more realistic, the thrust force starts off at a third
	//of it's value, to simulate a "ramping up" phase
	//
	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			particle->forces.x += direction.x / 3;
			particle->forces.y += direction.y / 3;
			particle->forces.z += direction.z / 3;
		}
		else {
			particle->forces.x += direction.x;
			particle->forces.y += direction.y;
			particle->forces.z += direction.z;
		}
	}
	applyOnce = true;
}

// Impulse Radial Force - this is a "one shot" force that
// eminates radially outward in random directions.
//
ImpulseRadialForce::ImpulseRadialForce(float magnitude) {
	this->magnitude = magnitude;
	applyOnce = true;
}

void ImpulseRadialForce::updateForce(Particle * particle) {

	// we basically create a random direction for each particle
	// the force is only added once after it is triggered.
	//
	ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-height / 2.0, height / 2.0), ofRandom(-1, 1));
	particle->forces += dir.getNormalized() * magnitude;
}


CollisionForce::CollisionForce(float frameRate,int extraType) {
	frames = frameRate;
	applyOnce = true;
	switch (extraType) {
	case '0':
		//extraBounce = ofVec3f(0, -0.5, 0);
		//norm = ofVec3f(0, 0, 1);
		break;
	case '1':
		extraBounce = ofVec3f(0, -0.5, 0);
	//	norm = ofVec3f(0, 0, -1);
		break;
	case '2':
		extraBounce = ofVec3f(0, -0.5, 0);
	//	norm = ofVec3f(1, 0, 0);
		break;
	case '3':
		extraBounce = ofVec3f(0, -0.5, 0);
	//	norm = ofVec3f(-1, 0, 0);
		break;

	}
}

void CollisionForce::updateForce(Particle * particle) {
	ofVec3f vel = particle->velocity;
	//vel += extraBounce; 
	
	ofVec3f colForce = (restitution + 1.0)*((-vel.dot(norm))*norm);
	particle->forces += frames * colForce;
}

CyclicForce::CyclicForce(float magnitude) {
	this->magnitude = magnitude;
}

void CyclicForce::updateForce(Particle * particle) {

	ofVec3f position = particle->position;

	ofVec3f norm = position.getNormalized();
	ofVec3f dir = norm.cross(ofVec3f(0, 1, 0));
	particle->forces += dir.getNormalized() * magnitude;
}