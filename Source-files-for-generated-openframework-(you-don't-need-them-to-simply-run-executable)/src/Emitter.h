#pragma once
#include <vector>;
#include "Agent.h"
#include "Item.h"
#include "Player.h"
#include <stdlib.h>
#include "ofImage.h"
#include "ofxGui.h"
#include "ParticleSystem.h"

class Emitter {
public:

	// agents
	std::vector<Agent*> agentArr = {};
	ofImage agentSprite;
	float agentRate = 1;
	int agentPerRate = 0;
	float agentTimer = 0;
	float agentNewSpeed = 0;
	bool agentTimerStop = false;

	

	Emitter() {
		if (!agentSprite.load("images/agent.png")) {
			ofLogFatalError("can't load image: images/agent.png");
			ofExit();
		}
		agentSprite.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		if (!batterySpite.load("images/battery.png")) {
			ofLogFatalError("can't load image: images/battery.png");
			ofExit();
		}
		batterySpite.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		if (!bombSpite.load("images/bomb.png")) {
			ofLogFatalError("can't load image: images/bomb.png");
			ofExit();
		}
		bombSpite.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		bombSpite.resize(64, 69);
	}

	// determine collision with Agents
	void agentsCollision(Player &player, ofxFloatSlider &player_energyLevel, ParticleSystem& particleSys) {
		for (int i = 0; i < agentArr.size(); i++) {
			if (!agentArr.empty()) {
				if (player.inside(agentArr[i]->transformedVert(agentArr[i]->vert[0]).x, agentArr[i]->transformedVert(agentArr[i]->vert[0]).y)
					|| player.inside(agentArr[i]->transformedVert(agentArr[i]->vert[1]).x, agentArr[i]->transformedVert(agentArr[i]->vert[1]).y)
					|| player.inside(agentArr[i]->transformedVert(agentArr[i]->vert[2]).x, agentArr[i]->transformedVert(agentArr[i]->vert[2]).y))
				{
					particleSys.explode(agentArr[i]->pos);
					agentArr.erase(agentArr.begin() + i);
					player.energyLevel--;
					player_energyLevel = player_energyLevel - 1;
				}
			}
		}
		
	}


	// delete agents when age equals life
	void deleteAgents(ParticleSystem& particleSys) {
		for (int i = 0; i < agentArr.size(); i++) {
			if (agentArr[i]->age >= agentArr[i]->life) {
				particleSys.explode(agentArr[i]->pos);
				agentArr.erase(agentArr.begin() + i);
				
			}
		}
		
	}

	

	// set pos for all agents
	void multPosAgents(float x, float y) {
		for (int i = 0; i < agentArr.size(); i++) {
			agentArr[i]->pos.x *= x;
			agentArr[i]->pos.y *= y;
		}
	}

	

	// delete all agents
	void deleteAllAgents() {
		agentArr.clear();
	}

	

	// Creates agents
	void createAgents(
		Player player,
		ofxFloatSlider& agent_speed,
		ofxFloatSlider& agent_scale,
		ofxToggle& agent_sprite,
		ofxFloatSlider& agent_launch_time,
		ofxFloatSlider& agent_rate,
		ofxFloatSlider& agent_per_rate,
		ofxFloatSlider& agent_life
					) {

		agentRate = agent_rate;
		agentPerRate = trunc(agent_per_rate);

		if (agentTimer >= agentRate) {
			for (int i = agentPerRate; i != 0; i--) {
			
				// calc rand that are not near player (not best but easy)
				int rand1, rand2;
				do {
					rand1 = rand() % (ofGetWindowWidth() - 100) + 100;
					rand2 = rand() % (ofGetWindowHeight() - 100) + 100;
				} while (glm::distance(player.pos, glm::vec3(rand1, rand2, 0)) < 300);
	
				
				Agent* agent = new Agent(glm::vec3(-12, 20, 0),
					glm::vec3(12, 20, 0),
					glm::vec3(0, -26, 0));

				agent->agentSprite = agentSprite;
				agent->showSprite = agent_sprite; 
				agent->pos = glm::vec3(rand1, rand2, 0);
				agent->scl.x = agent_scale;
				agent->scl.y = agent_scale;
				if (!agentTimerStop) {
					//agent->force = 500 * agentArr[i]->heading();
				}
				else {
					//agent->force = 100 * agentArr[i]->heading();
				}
				agent->age = 0;
				agent->life = agent_life;
				agent->launchTime = agent_launch_time;

				agentArr.push_back(agent);
			}
			agentTimer = 0;
		}
	}

	

	void drawAgents() {
		for (int i = 0; i < agentArr.size(); i++) {
			if(!agentArr.empty()){
				agentArr[i]->draw();
			}
		}
	}

	
	
	void updateAgents(Player player, int level) {
		agentTimer += ofGetLastFrameTime();
		for (int i = 0; i < agentArr.size(); i++) {
			if (!agentArr.empty()) {
				agentArr[i]->update(player, level);
			}
		}
	}

	

	void freezeAgentsForce() {
		for (int i = 0; i < agentArr.size(); i++) {
			if (!agentArr.empty()) {
				agentArr[i]->force = -50 * agentArr[i]->heading();
				agentTimerStop = true;
			}
		}
	}

	void unfreezeAgentsForce() {
		for (int i = 0; i < agentArr.size(); i++) {
			if (!agentArr.empty()) {
				agentArr[i]->force = 500 * agentArr[i]->heading();
			}
		}
	}

	

	// batteries -----------------------------------------

	std::vector<Item*> batteryArr = {};
	ofImage batterySpite;
	float batteryRate = 1;
	int batteryPerRate = 0;
	float batteryTimer = 0;
	float batteryNewSpeed = 0;
	bool batteryTimerStop = false;

	// Creates batteries
	void createBatteries(Player player,
						ofxFloatSlider& battery_speed,
						ofxFloatSlider& battery_scale,
						ofxToggle& battery_sprite,
						ofxFloatSlider& battery_rate,
						ofxFloatSlider& battery_per_rate,
						ofxFloatSlider& battery_life) {

		batteryRate = battery_rate;
		batteryPerRate = battery_per_rate;

		if (batteryTimer >= batteryRate) {
			for (int i = batteryPerRate; i != 0; i--) {

				// calc rand that are not near player (not best but easy)
				int rand1, rand2;
				do {
					rand1 = rand() % (ofGetWindowWidth() - 100) + 100;
					rand2 = rand() % (ofGetWindowHeight() - 100) + 100;
				} while (glm::distance(player.pos, glm::vec3(rand1, rand2, 0)) < 300);


				Item* battery = new Item(glm::vec3(-20, 20, 0),
					glm::vec3(20, 20, 0),
					glm::vec3(0, -17.32, 0));

				battery->itemSprite = batterySpite;
				battery->showSprite = battery_sprite;
				battery->pos = glm::vec3(rand1, rand2, 0);
				battery->scl.x = battery_scale;
				battery->scl.y = battery_scale;
				if (!batteryTimerStop) {
					battery->speed = battery_speed;
				}
				else {
					battery->speed = battery_speed / 2;
				}
				battery->age = 0;
				battery->life = battery_life;

				batteryArr.push_back(battery);
			}
			batteryTimer = 0;
		}
	}


	// determine collision with Batteries
	void batteriesCollision(Player& player, ofxFloatSlider& player_energyLevel) {
		for (int i = 0; i < batteryArr.size(); i++) {
			if (!batteryArr.empty()) {
				if (player.inside(batteryArr[i]->transformedVert(batteryArr[i]->vert[0]).x, batteryArr[i]->transformedVert(batteryArr[i]->vert[0]).y)
					|| player.inside(batteryArr[i]->transformedVert(batteryArr[i]->vert[1]).x, batteryArr[i]->transformedVert(batteryArr[i]->vert[1]).y)
					|| player.inside(batteryArr[i]->transformedVert(batteryArr[i]->vert[2]).x, batteryArr[i]->transformedVert(batteryArr[i]->vert[2]).y))
				{
					batteryArr.erase(batteryArr.begin() + i);
					player.energyLevel++;
					player_energyLevel = player_energyLevel + 1;
				}
			}
		}
	}

	// delete batteries when age equals life
	void deleteBatteries() {
		for (int i = 0; i < batteryArr.size(); i++) {
			if (batteryArr[i]->age >= batteryArr[i]->life) {
				batteryArr.erase(batteryArr.begin() + i);
			}
		}
	}

	// set pos for all batteries
	void multPosBatteries(float x, float y) {
		for (int i = 0; i < batteryArr.size(); i++) {
			batteryArr[i]->pos.x *= x;
			batteryArr[i]->pos.y *= y;
		}
	}

	// delete all batteries
	void deleteAllBatteries() {
		batteryArr.clear();
	}

	void setBatteriesSpeed(float speed) {
		for (int i = 0; i < batteryArr.size(); i++) {
			if (!batteryArr.empty()) {
				batteryArr[i]->speed = speed;
				batteryNewSpeed = speed;
				batteryTimerStop = true;
			}
		}
	}

	void drawBatteries() {
		for (int i = 0; i < batteryArr.size(); i++) {
			if (!batteryArr.empty()) {
				batteryArr[i]->draw();
			}
		}
	}

	void updateBatteries(Player player) {
		batteryTimer += ofGetLastFrameTime();
		for (int i = 0; i < batteryArr.size(); i++) {
			if (!batteryArr.empty()) {
				batteryArr[i]->update(player);
			}
		}
	}

	// bombs -----------------------------------------

	std::vector<Item*> bombArr = {};
	ofImage bombSpite;
	float bombRate = 1;
	int bombPerRate = 0;
	float bombTimer = 0;
	float bombNewSpeed = 0;
	bool bombTimerStop = false;

	// Creates bombs
	void createBombs(Player player,
		ofxFloatSlider& bomb_speed,
		ofxFloatSlider& bomb_scale,
		ofxToggle& bomb_sprite,
		ofxFloatSlider& bomb_rate,
		ofxFloatSlider& bomb_per_rate,
		ofxFloatSlider& bomb_life) {

		bombRate = bomb_rate;
		bombPerRate = bomb_per_rate;

		if (bombTimer >= bombRate) {
			for (int i = bombPerRate; i != 0; i--) {

				// calc rand that are not near player (not best but easy)
				int rand1, rand2;
				do {
					rand1 = rand() % (ofGetWindowWidth() - 100) + 100;
					rand2 = rand() % (ofGetWindowHeight() - 100) + 100;
				} while (glm::distance(player.pos, glm::vec3(rand1, rand2, 0)) < 300);


 				Item* bomb = new Item(glm::vec3(-20, 18.66, 0),
					glm::vec3(20, 18.66, 0),
					glm::vec3(0, -18.66, 0));

				bomb->itemSprite = bombSpite;
				bomb->showSprite = bomb_sprite;
				bomb->pos = glm::vec3(rand1, rand2, 0);
				bomb->scl.x = bomb_scale;
				bomb->scl.y = bomb_scale;
				if (!bombTimerStop) {
					bomb->speed = bomb_speed;
				}
				else {
					bomb->speed = bomb_speed / 2;
				}
				bomb->age = 0;
				bomb->life = bomb_life;

				bombArr.push_back(bomb);
			}
			bombTimer = 0;
		}
	}


	// determine collision with bombs
	void bombCollision(Player& player, int& bombs) {
		for (int i = 0; i < bombArr.size(); i++) {
			if (!bombArr.empty()) {
				if (player.inside(bombArr[i]->transformedVert(bombArr[i]->vert[0]).x, bombArr[i]->transformedVert(bombArr[i]->vert[0]).y)
					|| player.inside(bombArr[i]->transformedVert(bombArr[i]->vert[1]).x, bombArr[i]->transformedVert(bombArr[i]->vert[1]).y)
					|| player.inside(bombArr[i]->transformedVert(bombArr[i]->vert[2]).x, bombArr[i]->transformedVert(bombArr[i]->vert[2]).y))
				{
					bombArr.erase(bombArr.begin() + i);
					bombs++;
				}
			}
		}
	}

	// delete bombs when age equals life
	void deleteBombs() {
		for (int i = 0; i < bombArr.size(); i++) {
			if (bombArr[i]->age >= bombArr[i]->life) {
				bombArr.erase(bombArr.begin() + i);
			}
		}
	}

	// set pos for all bombs
	void multPosBombs(float x, float y) {
		for (int i = 0; i < bombArr.size(); i++) {
			bombArr[i]->pos.x *= x;
			bombArr[i]->pos.y *= y;
		}
	}

	// delete all bombs
	void deleteAllBombs() {
		bombArr.clear();
	}

	void setBombsSpeed(float speed) {
		for (int i = 0; i < bombArr.size(); i++) {
			if (!bombArr.empty()) {
				bombArr[i]->speed = speed;
				bombNewSpeed = speed;
				bombTimerStop = true;
			}
		}
	}

	void drawBombs() {
		for (int i = 0; i < bombArr.size(); i++) {
			if (!bombArr.empty()) {
				bombArr[i]->draw();
			}
		}
	}

	void updateBombs(Player player) {
		bombTimer += ofGetLastFrameTime();
		for (int i = 0; i < bombArr.size(); i++) {
			if (!bombArr.empty()) {
				bombArr[i]->update(player);
			}
		}
	}
};