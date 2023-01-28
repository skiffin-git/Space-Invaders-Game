#pragma once

#include "ofMain.h"
#include "Player.h"
#include "ofxGui.h"
#include "Agent.h"
#include "Emitter.h"
#include "ParticleSystem.h"

class ofApp : public ofBaseApp{

	public:
		
		float currentTime = 0;
		bool stageGameMenu = false;
		bool stageGameOver = false;
		bool stageGame = false;
		float levelTimer = 0;
		float gameOverTime = 0;
		float lastScore = 0;
		bool levelUp;
		int lastLevel = 1;
		bool stageLevel1 = false;
		bool stageLevel2 = false;
		bool stageLevel3 = false;
		bool stageTest = false;
		bool fullScreen = false;
		bool restart = false;
		bool pressedI = true;
		bool playerGotBattery = false;
		int level = 1;
		float score = 0;
		int oldLive = 0;
		int newLive = 0;
		bool playerGotDamage = false;
		bool playerGotBomb = false;
		int bombs = 0;
		float agentSpeedBeforeFreeze;
		int freezeEnergy = 0;
		float oldAngentsSpeed = 0;
		bool pressedE = false;
		bool pressedW = false;
		float speedEnergyTimer1 = 0;
		float speedEnergyTimer2 = 0;
		int oldBombs = 0;
		bool pressedQ = false;
		bool pressedM = false;
		bool canPlayTimeFreezeStart = true;
		bool canPlayTimeFreezeEnd = false;
		float timer1 = 0;
		float timer2 = 0;
		float timer3 = 0;
		float timer4 = 0;
		glm::vec3 expPos = glm::vec3(0,0,0); 
		bool stop1 = false;
		bool stop2 = false;
		bool stop3 = false;
		bool stop4 = false;
		bool stop5 = false;
		bool stop6 = false;
		bool stop7 = false;
		bool stop8 = false;
		bool stop9 = false;
		bool stop10 = false;
		bool stop11 = false;

		// sounds
		ofSoundPlayer stageGameMusic;
		ofSoundPlayer pickUpBattery;
		ofSoundPlayer pickUpBomb;
		ofSoundPlayer takingDamage;
		ofSoundPlayer timeFreezeStart;
		ofSoundPlayer timeFreezeEnd;
		ofSoundPlayer bombExplosion;

		// fonts
		ofTrueTypeFont PixelLCD7;
		ofTrueTypeFont PixelSansSerif;
		ofTrueTypeFont PixelMillennium;

		// background images
		ofImage backGroundDark;
		ofImage backGroundBlueSpace;

		// Player
		Player player;

		// Agent
		Emitter agentSys;

		// Battery
		Emitter batterySys;

		// Bomb
		Emitter bombSys;

		// GUI
		ofxPanel gui;
		bool showGUI;

		// Player gui
		ofxFloatSlider player_speed;
		ofxFloatSlider player_scale;
		ofxFloatSlider player_energyLevel;
		ofxToggle player_sprite;

		// Agent gui
		ofxFloatSlider agent_speed;
		ofxFloatSlider agent_scale;
		ofxToggle agent_sprite;
		ofxFloatSlider agent_launch_time;
		ofxFloatSlider agent_rate;
		ofxFloatSlider agent_per_rate;
		ofxFloatSlider agent_life;

		// Battery gui
		ofxFloatSlider battery_speed;
		ofxFloatSlider battery_scale;
		ofxToggle battery_sprite;
		ofxFloatSlider battery_rate;
		ofxFloatSlider battery_per_rate;
		ofxFloatSlider battery_life;

		// Bomb gui
		ofxFloatSlider bomb_speed;
		ofxFloatSlider bomb_scale;
		ofxToggle bomb_sprite;
		ofxFloatSlider bomb_rate;
		ofxFloatSlider bomb_per_rate;
		ofxFloatSlider bomb_life;

		// particles
		ParticleSystem particleSys;
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		
};
