#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Particle.h"

class ParticleSystem {
public:

	vector<Particle> particles;
	float strength = 9.0;
	float currentTime = 0;
	

	ParticleSystem() {}

	void explode(glm::vec3 pos) {
		for (int i = 0; i < strength / 3 ; i++) {
			float rand1 = strength * ofRandom(-10, 10);
			float rand2 = strength * ofRandom(-10, 10);
			glm::vec3 force = glm::vec3(rand1, rand2, 0);
			glm::vec3 velocity = glm::vec3(rand1, rand2, 0);
			float startTime = 0;
			Particle particle = Particle(pos, force, velocity, false);
			particles.push_back(particle);
		}
	}

	void bombExplode(glm::vec3 pos) {
		for (int i = 0; i < 3 * strength; i++) {
			float rand1 = strength * ofRandom(-150, 150);
			float rand2 = strength * ofRandom(-150, 150);
			glm::vec3 force = glm::vec3(rand1, rand2, 0);
			glm::vec3 velocity = glm::vec3(rand1, rand2, 0);
			float startTime = 0;
			Particle particle = Particle(pos, force, velocity, true);
			particles.push_back(particle);
		}
	}

	void update() {
		for (int i = 0; i < particles.size(); i++) {
			particles[i].update();

			
			// disappear effect
			for (int i = 0; i < particles.size(); i++) {
				particles[i].color.a = 30 + 220 * (0 + particles[i].lifespan - particles[i].age)
					/ particles[i].lifespan;
			}

			// Delete if age over lifeSpan
			for (int i = 0; i < particles.size(); i++) {
				if (particles[i].age >= particles[i].lifespan) {
					particles.erase(particles.begin() + i);
				}
			}
		}
		
		
		
	}

	void draw() {
		for (int i = 0; i < particles.size(); i++) {
			particles[i].draw();
		}
	}

};

