#pragma once

//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith  
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>

#include "ofMain.h"
#include "Particle.h"


//  Pure Virtual Function Class - must be subclassed to create new forces.
//
class ParticleForce {
protected:
public:
	bool applyOnce = false;
	bool applied = false;
	bool stopTurbulence = false;
	bool stop = false;
	virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
	void add(const Particle &);
	void addForce(ParticleForce *);
	void remove(int);
	void update();
	void setLifespan(float);
	void reset();
	int removeNear(const ofVec3f & point, float dist);
	void draw();
	vector<Particle> particles;
	vector<ParticleForce *> forces;
};



// Some convenient built-in forces
//
class GravityForce : public ParticleForce {
	ofVec3f gravity;
	
public:
	void set(const ofVec3f &g) { gravity = g; }
	GravityForce(const ofVec3f & gravity);
	void updateForce(Particle *);
};

class TurbulenceForce : public ParticleForce {
	ofVec3f tmin, tmax;
public:
	void set(const ofVec3f &min, const ofVec3f &max) { tmin = min; tmax = max; }
	TurbulenceForce(const ofVec3f & min, const ofVec3f &max);
	TurbulenceForce() { tmin.set(0, 0, 0); tmax.set(0, 0, 0); }
	void updateForce(Particle *);
};

class ImpulseRadialForce : public ParticleForce {
	float magnitude = 1.0;
	float height = .2;
public:
	void set(float mag) { magnitude = mag; }
	void setHeight(float h) { height = h; }
	ImpulseRadialForce(float magnitude);
	ImpulseRadialForce() {}
	void updateForce(Particle *);
};

//  Added thruster force for the rocket

class ThrusterForce : public ParticleForce {
	ofVec3f direction;
public:
	void set(const ofVec3f &dir) { direction = dir; }
	ThrusterForce(const ofVec3f &dir);
	ThrusterForce() { direction.set(0, 0, 0); }
	void updateForce(Particle *);
};


// Added Collision Force
class CollisionForce : public ParticleForce {
	float frames;
	float restitution = 0.8;
	ofVec3f norm = ofVec3f(0, 0, 1);
	ofVec3f extraBounce;
public :
	CollisionForce(float frameRate,int extraType);
	CollisionForce() {}
	void updateForce(Particle *);
};

//Cyclic Force
class CyclicForce : public ParticleForce {
	float magnitude;
public:
	void set(float mag) { magnitude = mag; }
	CyclicForce(float magnitude);
	void updateForce(Particle *);
};
