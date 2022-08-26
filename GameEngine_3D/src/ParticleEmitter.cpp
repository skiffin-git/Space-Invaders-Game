

//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith  
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>

#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {
	sys = new ParticleSystem();
	createdSys = true;
	init();
}

ParticleEmitter::ParticleEmitter(ParticleSystem *s) {
	if (s == NULL)
	{
		cout << "fatal error: null particle system passed to ParticleEmitter()" << endl;
		ofExit();
	}
	sys = s;
	createdSys = false;
	init();
}

ParticleEmitter::~ParticleEmitter() {

	// deallocate particle system if emitter created one internally
	//
	if (createdSys) delete sys;
}

void ParticleEmitter::init() {
	rate = 30;
	velocity = ofVec3f(0, -1, 0);
	lifespan = .5;
	mass = 1;
	randomLife = false;
	lifeMinMax = ofVec3f(2, 4);
	started = false;
	oneShot = false;
	fired = false;
	lastSpawned = 0;
	radius = .20;
	particleRadius = .01;
	visible = true;
	type = DirectionalEmitter;
	groupSize = 30;
	damping = .99;
	particleColor = ofColor::yellow;
	position = ofVec3f(0, 0, 0);
}



void ParticleEmitter::draw() {
	if (visible) {
		switch (type) {
		case DirectionalEmitter:
			ofDrawSphere(position, radius / 10);  // just draw a small sphere for point emitters 
			break;
		case SphereEmitter:
			break;
		case RadialEmitter:
			ofDrawSphere(position, radius / 10);  // just draw a small sphere as a placeholder
			break;
		case DiscEmitter:
			ofDrawSphere(position, .1);
			break;
		default:
			break;
		}
	}
	sys->draw();
}
void ParticleEmitter::start() {
	if (started) return;
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void ParticleEmitter::stop() {
	started = false;
	fired = false;
}
void ParticleEmitter::update() {

	float time = ofGetElapsedTimeMillis();

	if (oneShot && started) {
		if (!fired) {

			// spawn a new particle(s)
			//
			for (int i = 0; i < groupSize; i++) {
				spawn(time);
			}

			lastSpawned = time;
		}
		fired = true;
		stop();
	}

	else if (((time - lastSpawned) > (1000.0 / rate)) && started) {

		// spawn a new particle(s)
		//
		for (int i = 0; i < groupSize; i++)
			spawn(time);

		lastSpawned = time;
	}

	sys->update();
}

// spawn a single particle.  time is current time of birth
//
void ParticleEmitter::spawn(float time) {

	Particle particle;

	// set initial velocity and position
	// based on emitter type
	//
	switch (type) {
	case RadialEmitter:
	{
		ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		float speed = velocity.length();
		particle.velocity = dir.getNormalized() * speed;
		particle.position.set(position);
	}
	break;
	case SphereEmitter:
		break;
	case DirectionalEmitter:
		particle.velocity = velocity;
		particle.position.set(position);
		break;
	case DiscEmitter:   // x-z plane
	{
		ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-.2, .2), ofRandom(-1, 1));
		//	dir.y = 0; 
		particle.position.set(position + (dir.normalized() * radius));
		particle.velocity = velocity;
	}
	}

	// other particle attributes
	//
	if (randomLife) {
		particle.lifespan = ofRandom(lifeMinMax.x, lifeMinMax.y);
	}
	else particle.lifespan = lifespan;
	particle.birthtime = time;
	particle.radius = particleRadius;
	particle.mass = mass;
	particle.damping = damping;
	particle.color = particleColor;

	// add to system
	//
	sys->add(particle);
}
