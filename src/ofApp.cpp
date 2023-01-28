#include "ofApp.h"
#include <stdlib.h> 
#include <format>
#include "FreeImage.h"


//--------------------------------------------------------------
void ofApp::setup(){
	// Set FPS
	ofSetFrameRate(60);

	// Player
	player = Player(glm::vec3(-31, 10, 0), glm::vec3(31, 10, 0), glm::vec3(0, -51, 0));
	player.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
	
	// first load images, assuming it's in bin/data folder
	if (!player.playerSprite.load("images/player.png")) { 
		ofLogFatalError("can't load image: images/player.png");
		ofExit();
	}
	player.playerSprite.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	// load bg
	if (!backGroundBlueSpace.load("images/darkerBlueSpaceBG.png")) {
		ofLogFatalError("can't load image: images/darkerBlueSpaceBG.png");
		ofExit();
	}
	if (!backGroundDark.load("images/darkBG.png")) {
		ofLogFatalError("can't load image: images/darkBG.png");
		ofExit();
	}
	backGroundBlueSpace.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	
	// load fonts 
	if (!PixelLCD7.load("fonts/PixelLCD7/PixelLCD7.ttf", 10)) {
		ofLogFatalError("can't load font: fonts/PixelLCD7/PixelLCD7.ttf");
		ofExit();
	}
	if (!PixelSansSerif.load("fonts/PixelSansSerif/PixelSansSerif.ttf", 10)) {
		ofLogFatalError("can't load font: fonts/PixelSansSerif/PixelSansSerif.ttf");
		ofExit();
	}
	
	// load sound
	if (!stageGameMusic.load("sounds/phantom.mp3")) {
		ofLogFatalError("can't load sounds: sound/phantom.mp3");
		ofExit();
	}
	if (!pickUpBattery.load("sounds/pickUpBattery.mp3")) {
		ofLogFatalError("can't load sounds: sound/pickUpBattery.mp3");
		ofExit();
	}
	if (!pickUpBomb.load("sounds/pickUpBobp.mp3")) {
		ofLogFatalError("can't load sounds: sound/pickUpBobp.mp3");
		ofExit();
	}
	if (!takingDamage.load("sounds/takingDamage.mp3")) {
		ofLogFatalError("can't load sounds: sound/takingDamage.mp3");
		ofExit();
	}
	if (!timeFreezeStart.load("sounds/timeFreezeStart.mp3")) {
		ofLogFatalError("can't load sounds: sound/timeFreezeStart.mp3");
		ofExit();
	}
	if (!timeFreezeEnd.load("sounds/timeFreezeEnd.mp3")) {
		ofLogFatalError("can't load sounds: sound/timeFreezeEnd.mp3");
		ofExit();
	}
	if (!bombExplosion.load("sounds/bombExplosion.mp3")) {
		ofLogFatalError("can't load sounds: sound/bombExplosion.mp3");
		ofExit();
	}

	// VOLUME
	stageGameMusic.play();
	stageGameMusic.setLoop(true);
	stageGameMusic.setVolume(0.1);
	takingDamage.setVolume(1);
	pickUpBomb.setVolume(1);
	pickUpBattery.setVolume(1);
	timeFreezeStart.setVolume(1);
	timeFreezeEnd.setVolume(1);
	bombExplosion.setVolume(1);
	
	// GUI
	gui.setup();
	showGUI = false;

	// Player gui
	gui.add(player_speed.setup("player speed", 300, 100, 999));
	gui.add(player_scale.setup("player scale", 1.1, 0.5, 2));
	gui.add(player_sprite.setup("player sprite", player.showSprite, 20, 20));
	gui.add(player_energyLevel.setup("energy level", 3, 0, 99));
	
	// Agents gui
	gui.add(agent_speed.setup("agents speed", 400, 0, 999));
	gui.add(agent_scale.setup("agents scale", 3, 0.3, 3));
	gui.add(agent_sprite.setup("agents sprite", player.showSprite, 20, 20));
	gui.add(agent_launch_time.setup("agents launch time", 1, 0, 3));
	gui.add(agent_rate.setup("agents rate", 3, 0.5, 5));
	gui.add(agent_per_rate.setup("agents per rate", 1, 1, 9));
	gui.add(agent_life.setup("agents life", 10, 1, 9));
	
	
	// batteries
	battery_speed = 100;
	battery_scale = 0.7;
	battery_sprite = true;
	battery_rate = 15;
	battery_per_rate = 1;
	battery_life = 5;

	// bombs
	bombs = 3;
	bomb_speed = 100;
	bomb_scale = 1;
	bomb_sprite = true;
	bomb_rate = 20;
	bomb_per_rate = 1;
	bomb_life = 5;
	

	// freeze
	freezeEnergy = 30;

	// start game
	stageGameMenu = true;
}

//--------------------------------------------------------------
void ofApp::update(){

	
	// Update current time
	if (!stageGameMenu && !stageGameOver) {
		currentTime += ofGetLastFrameTime();
		levelTimer += ofGetLastFrameTime();
		if(pressedE)
			speedEnergyTimer1 += ofGetLastFrameTime();
		speedEnergyTimer2 += ofGetLastFrameTime();
	}

	// Level Up System
	if (levelTimer >= 1 && !stageGameOver && !stageTest) {
		levelTimer = 0;
		level++;
		levelUp = true;
	}
	

	// if show images
	if (pressedI) {
		player_sprite = true;
		agent_sprite = true;
	}
	else {
		player_sprite = false;
		agent_sprite = false;
	}

	// calc score
	if(!stageTest)
		score += currentTime * level / 10000;
	
	// GAME UPDATE
	
	if (stageGame || stageTest) {

		// time energy up
		if (speedEnergyTimer2 >= 0.1 && freezeEnergy <= 29 && !pressedE) {
			speedEnergyTimer2 = 0;
			freezeEnergy++;
		}
		// time energy down and use
		if (freezeEnergy >= 1) {
			if (speedEnergyTimer1 >= 0.1) {
				speedEnergyTimer1 = 0;
				freezeEnergy--;
			}

			if (pressedE && freezeEnergy >= 3) {

				player_scale = 0.6;
				if (!pressedW) {
					player_speed = 250;
				}
				else {
					player_speed = 400;
				}
				agentSpeedBeforeFreeze = agent_speed;
				agentSys.freezeAgentsForce();
			}
		}
		else {
			if (pressedW) {
				player_speed = 550;
			}
			else
				player_speed = 300;
			if (!stageTest) {
				if (agent_speed <= 999) {
					if (400 + 1 * level <= 999) {
						agentSys.freezeAgentsForce();	// if change here
					}												// also chenge in E
					else
						agentSys.freezeAgentsForce();
				}
			}
			else {
				agentSys.unfreezeAgentsForce();
			}

			player_scale = 1.1;
		}

		// SOUND FREEZE
		if (pressedE) {
			if (!timeFreezeStart.isPlaying() && canPlayTimeFreezeStart) {
				timeFreezeEnd.stop();
				timeFreezeStart.play();
				canPlayTimeFreezeStart = false;
			}
		}
		else if (canPlayTimeFreezeEnd) {
			if (!timeFreezeEnd.isPlaying() && !timeFreezeStart.isPlaying()) {
				timeFreezeStart.stop();
				timeFreezeEnd.play();
				canPlayTimeFreezeEnd = false;
			}
			canPlayTimeFreezeStart = true;
		}
		//std::cout << freezeEnergy << std::endl;



		// levelUp
		if (levelUp && !stageTest) {
			if (!stop5) {
				agent_speed = 400;
				agent_scale = 3;
				agent_launch_time = 1;
				agent_per_rate = 1;
				agent_rate = 3;
				agent_life = 10;
			}

			// scale
			if (agent_scale >= 1) {
				if (3 - 0.3 * level >= 1)
					agent_scale = 3 - 0.3 * level;
				else
					agent_scale = 1;
			}
			// launch time
			if (agent_launch_time <= 2) {
				if (1 + 0.03 * level <= 2)
					agent_launch_time = 1 + 0.03 * level;
				else
					agent_launch_time = 2;
			}
			// per rate	
			if (agent_per_rate <= 8) {
				if (1 + 0.3 * level <= 8)
					agent_per_rate = 1 + 0.3 * level;
				else
					agent_per_rate = 8;
			}
			// rate	
			if (agent_rate >= 3) {
				if (5 - 0.03 * level >= 3)
					agent_rate = 5 - 0.03 * level;
				else
					agent_rate = 3;
			}
			// life
			if (agent_life >= 7) {
				if (10 - 0.03 * level >= 7)
					agent_life = 10 - 0.03 * level;
				else
					agent_life = 7;
			}


			stop5 = true;
		}

		// TEST
		if (stageTest && !stop11) {
			// infinit bombs
			bombs = -1;

			// reset Player
			player.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
			player.rot = 0;
			player_speed = 300;
			player_scale = 1.1;
			player_energyLevel = 0;

			// reset Agent
			agent_speed = 300;
			agent_scale = 0.6;
			agent_launch_time = 1;
			agent_rate = 1;
			agent_per_rate = 1;
			agent_life = 5;

			stop11 = true;
		}


		// Player
		player.update(player_speed, player_scale,
			player_sprite, player_energyLevel);

		// Agent system
		agentSys.deleteAgents(particleSys);
		
		// turn player red
		oldLive = player_energyLevel;
		agentSys.agentsCollision(player, player_energyLevel, particleSys);
		if (oldLive > player_energyLevel) {
			playerGotDamage = true;
			takingDamage.play();
			
		}
		agentSys.createAgents(
			player,
			agent_speed, agent_scale,
		    agent_sprite, agent_launch_time,
			agent_rate, agent_per_rate, 
			agent_life);
		agentSys.updateAgents(player, level);

		// particles
		particleSys.update();

		// Batteries
		batterySys.deleteBatteries();
		// turn player green
		oldLive = player_energyLevel;
		batterySys.batteriesCollision(player, player_energyLevel);
		if (oldLive < player_energyLevel) {
			playerGotBattery = true;
			pickUpBattery.play();
		}
		batterySys.createBatteries(player, battery_speed,
			battery_scale,
			battery_sprite,
			battery_rate,
			battery_per_rate,
			battery_life);
		batterySys.updateBatteries(player);

		// Bombs
		bombSys.deleteBombs();
		// turn player orange
		oldBombs = bombs;
		bombSys.bombCollision(player, bombs);
		if (oldBombs < bombs) {
			playerGotBomb = true;
			pickUpBomb.play();
		}
		bombSys.createBombs(player, bomb_speed,
			bomb_scale,
			bomb_sprite,
			bomb_rate,
			bomb_per_rate,
			bomb_life);
		bombSys.updateBombs(player);

		// Check for GAME OVER
		if (player.energyLevel <= 0 && !stageTest) {
			stageGame = false;
			stageTest = false;
			stageGameOver = true;
			}
	}


	// RESTART
	if (restart) {

		level = 1;
		gameOverTime = 0;
		score = 0;
		bombs = 3;
		freezeEnergy = 30;

		// reset player
		player.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
		player.rot = 0;
		player_speed = 300;
		player_scale = 1.1;
		player_energyLevel = 3;

		// reset agent
		agentSys.deleteAllAgents();
		batterySys.deleteAllBatteries();
		bombSys.deleteAllBombs();

		// reset stages
		currentTime = 0;
		stageGameOver = false;
		if (pressedM) {
			stageGame = false;
			stageTest = false;
			stageGameOver = false;
			stageGameMenu = true;
			pressedM = false;
		}
		else {
			stageGame = true;
			stageGameMenu = false;
		}
		timer1 = 0;
		timer2 = 0;
		timer3 = 0;
		stop1 = false; 
		stop2 = false;
		stop3 = false;
		stop4 = false;
		stop5 = false;
		stop6 = false;
		stop7 = false;
		stop8 = false;
		stop9 = false;
		//stop10 = false;
		stop11 = false;
		pressedE = false;
		pressedQ = false;
		pressedW = false;
		playerGotDamage = false;
		playerGotBattery = false;
		playerGotBomb = false;
		canPlayTimeFreezeEnd = false;
		canPlayTimeFreezeStart = true;
		
	
		restart = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	

	// if show images
	if (pressedI && !stageGameMenu && !stageGameOver && !pressedQ) {
		ofSetColor(ofColor::darkCyan);
		backGroundBlueSpace.resize(ofGetWidth(), ofGetHeight());
		backGroundBlueSpace.draw(0, 0);
		battery_sprite = true;
		bomb_sprite = true;

	}
	else if(pressedI){
		backGroundDark.resize(ofGetWidth(), ofGetHeight());
		backGroundDark.draw(0, 0);
	}
	else {
		battery_sprite = false;
		bomb_sprite = false;
	}


	// screen draw
	if (pressedQ) {
		if (!stop7) {
			timer2 = currentTime;
		}
		ofSetColor(255,255,255);
		if(pressedI)
			backGroundBlueSpace.draw(0, 0);
		bombExplosion.play();
		stop7 = true;
		if (currentTime >= timer2 + 0.2) {
			pressedQ = false;
			stop7 = false;
		}
	}
	
	// GAME DRAW
	if (stageGame || stageTest) {

		// particles
		particleSys.draw();
		
		// player
		if (!playerGotDamage && !playerGotBattery) {
			if (pressedI) {
				ofSetColor(ofColor::white);
				player.draw();
			}
			else {
				ofSetColor(ofColor::green);
				player.draw();
			}
		}
		else if(playerGotDamage){
			if (!stop6) {
				timer1 = currentTime;
			}
			ofSetColor(ofColor::red);
			player.draw();
			stop6 = true;
			if (currentTime >= timer1 + 0.2) {
				playerGotDamage = false;
				stop6 = false;
			}
		}
		else{
			if (!stop8) {
				timer3 = currentTime;
			}
			ofSetColor(ofColor::green);
			player.draw();
			stop8 = true;
			if (currentTime >= timer3 + 0.2) {
				playerGotBattery = false;
				stop8 = false;
			}
		}
		
		
		// agents
		ofSetColor(ofColor::yellow);
		agentSys.drawAgents();

		// batteris
		ofSetColor(ofColor::white);
		batterySys.drawBatteries();

		// draw bobs
		ofSetColor(ofColor::white);
		bombSys.drawBombs();

		// HUD
		ofSetColor(ofColor::black);
		ofDrawRectangle(0, 0, ofGetWindowWidth(), 50);
		ofSetColor(ofColor::white);
		PixelLCD7.drawString("FPS:", ofGetWindowWidth() - 85, 30);
		ofSetColor(ofColor::lightGreen);
		PixelLCD7.drawString(std::to_string(static_cast<int>(ofGetFrameRate())), ofGetWindowWidth() - 50, 30);
		ofSetColor(ofColor::white);
		PixelLCD7.drawString("Energy Level:", 20, 30);
		ofSetColor(ofColor::green);
		PixelLCD7.drawString(std::to_string(player.energyLevel), 150, 30);
		ofSetColor(ofColor::white);
		PixelLCD7.drawString("Bombs:", 185, 30);
		ofSetColor(ofColor::orange);
		PixelLCD7.drawString(std::to_string(bombs), 245, 30);
		ofSetColor(ofColor::white);
		PixelLCD7.drawString("Freeze:", 275, 30);
		ofSetColor(ofColor::white);
		ofNoFill();
		ofDrawRectangle(345, 20, 100, 10);

		if (freezeEnergy > 5) {
			ofSetColor(ofColor::yellow);
			ofFill();
		}
		else {
			ofSetColor(ofColor::red);
			ofFill();
		}
		ofDrawRectangle(345, 20, freezeEnergy * 3.33, 10);
		ofSetColor(ofColor::white);
		PixelLCD7.drawString("Speed:", 455, 30);
		ofSetColor(ofColor::purple);
		PixelLCD7.drawString(std::to_string(static_cast<int>(player_speed)), 520, 30);;
		ofSetColor(ofColor::white);
		PixelLCD7.drawString("Level:", 565, 30);
		ofSetColor(ofColor::red);
		if (!stageTest) {
			PixelLCD7.drawString(std::to_string(level), 625, 30);
		}
		else {
			PixelLCD7.drawString("TEST", 625, 30);
		}
		ofSetColor(ofColor::white);
		PixelLCD7.drawString("Time:", 675, 30);
		ofSetColor(ofColor::cyan);
		PixelLCD7.drawString(std::to_string(static_cast<int>(currentTime)), 725, 30);
		ofSetColor(ofColor::white);
		PixelLCD7.drawString("Score:", 775, 30);
		ofSetColor(ofColor::lightBlue);
		PixelLCD7.drawString(std::to_string(static_cast<int>(score)), 835, 30);
		ofSetColor(ofColor::lightPink);
		
		
		
		
	}

	

	// GAME OVER
	if (stageGameOver) {
		
		// calc last game time
		
		if(gameOverTime == 0){
			gameOverTime = currentTime;
			lastScore = score;
			lastLevel = level;
		}
		
		// set precision
		std::string num_text = std::to_string(gameOverTime);
		std::string roundedTime = num_text.substr(0, num_text.find(".") + 3);
		std::string roundedScore = std::to_string(static_cast<int>(lastScore));
		std::string roundedLevel = std::to_string(lastLevel);

		ofSetColor(ofColor::green);
		PixelSansSerif.drawString("GAME OVER", ofGetWindowWidth() / 2 - 35, ofGetWindowHeight() / 2 - 30);
		ofSetColor(ofColor::white);
		PixelSansSerif.drawString("Level : ", ofGetWindowWidth() / 2 - 35, ofGetWindowHeight() / 2 + 30);
		ofSetColor(ofColor::red);
		PixelSansSerif.drawString(roundedLevel, ofGetWindowWidth() / 2 + 45, ofGetWindowHeight() / 2 + 30);
		ofSetColor(ofColor::white);
		PixelSansSerif.drawString("Time : ", ofGetWindowWidth() / 2 - 35, ofGetWindowHeight() / 2 + 30*2);
		ofSetColor(ofColor::cyan);
		PixelSansSerif.drawString(roundedTime, ofGetWindowWidth() / 2 + 25, ofGetWindowHeight() / 2 + 30*2);
		ofSetColor(ofColor::white);
		PixelSansSerif.drawString("Score : ", ofGetWindowWidth() / 2 - 35, ofGetWindowHeight() / 2 + 30 * 3);
		ofSetColor(ofColor::lightBlue);
		PixelSansSerif.drawString(roundedScore, ofGetWindowWidth() / 2 + 45, ofGetWindowHeight() / 2 + 30 * 3);
		ofSetColor(ofColor::greenYellow);
		PixelSansSerif.drawString("Press R to Restart", ofGetWindowWidth() / 2 - 35, ofGetWindowHeight() / 2 + 30 * 5);
		PixelSansSerif.drawString("Press M to go to Menu", ofGetWindowWidth() / 2 - 35, ofGetWindowHeight() / 2 + 30 * 6);
		}

	// GAME MENU
	if(stageGameMenu) {
		ofSetColor(ofColor::lightGrey);
		PixelSansSerif.drawString("MENU:", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 - 30*8);
		ofSetColor(ofColor::forestGreen);
		PixelSansSerif.drawString("SPACE to Start", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 - 30*7);
		PixelSansSerif.drawString("MOVEMENT:", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 - 30*5);
		PixelSansSerif.drawString("ARROW UP to go Forward", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 - 30 * 4);
		PixelSansSerif.drawString("ARROW DOWN to go Backwards", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 - 30 * 3);
		PixelSansSerif.drawString("ARROW LEFT to turn Left", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 - 30 *  2);
		PixelSansSerif.drawString("ARROW RIGHT to turn Right", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 - 30 * 1);
		ofSetColor(ofColor::cyan);
		PixelSansSerif.drawString("ABILITIES:", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 1);
		PixelSansSerif.drawString("Q to use a Bomb", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 2);
		PixelSansSerif.drawString("W to go Faster", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30*3);
		PixelSansSerif.drawString("E to FREEZE time and become smaller", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30*4);
		ofSetColor(ofColor::greenYellow);
		PixelSansSerif.drawString("BINDKEYS:", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 6);
		PixelSansSerif.drawString("F to make Fullscreen", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 7);
		PixelSansSerif.drawString("R to Restart", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 8);
		PixelSansSerif.drawString("M to return to Menu", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 9);
		PixelSansSerif.drawString("I to hide / show Sprites", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 10);
		ofSetColor(ofColor::darkorange);
		PixelSansSerif.drawString("TEST MODE:", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 12);
		PixelSansSerif.drawString("T to enter Test Mode", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 13);	
		PixelSansSerif.drawString("H - GUI and Draggable Space Ship", ofGetWindowWidth() / 2 - 80, ofGetWindowHeight() / 2.5 + 30 * 14);
	}
	
	// Draw gui
	if (showGUI && stageTest)
		gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {

		// Player movement
	case OF_KEY_UP:
		player.goForward = true;
		break;
	case OF_KEY_DOWN:
		player.goReverse = true;
		break;
	case OF_KEY_LEFT:
		player.turnLeft = true;
		break;
	case OF_KEY_RIGHT:
		player.turnRight = true;
		break;
	// kill all agents
	case 'Q':
	case 'q':
		if(bombs != 0 && !pressedE){
		agentSys.deleteAllAgents();
		bombs--;
		particleSys.bombExplode(player.pos);
		pressedQ = true;
		}
		break;
	// use turbo
	case 'W':
	case 'w':
		if (!pressedE) {
			player_speed = 550;
		}
		else {
			player_speed = 400;
		}
		pressedW = true;
		break;
	// slow mode
		
	case 'E':
	case 'e':
		pressedE = true;
		break;
	// Start game
	case ' ':
		if (stageGameMenu) {
			restart = true;
		}
		break;
		// Go Menu
	case 'M':
	case 'm':
			restart = true;
			pressedM = true;
		break;
		// show GUI
	case 'H':
	case 'h':
		if (showGUI == false)
			showGUI = true;
		else
			showGUI = false;
		break;
		// show GUI
	case 'I':
	case 'i':
		if (pressedI == false)
			pressedI = true;
		else
			pressedI = false;
		break;
		// make fullscreen
	case 'F':
	case 'f':
		if (fullScreen == false) {
			fullScreen = true;
			float oldW = ofGetWidth();
			float oldH = ofGetHeight();
			ofToggleFullscreen();
			player.pos.x *= (ofGetWidth() / oldW);
			player.pos.y *= (ofGetHeight() / oldH);
			agentSys.multPosAgents(ofGetWidth() / oldW, ofGetHeight() / oldH);
			batterySys.multPosAgents(ofGetWidth() / oldW, ofGetHeight() / oldH);
			bombSys.multPosAgents(ofGetWidth() / oldW, ofGetHeight() / oldH);
		}
		else {
			fullScreen = false;
			float oldW = ofGetWidth();
			float oldH = ofGetHeight();
			ofToggleFullscreen();
			player.pos.x *= (ofGetWidth() / oldW);
			player.pos.y *= (ofGetHeight() / oldH);
			agentSys.multPosAgents(ofGetWidth() / oldW, ofGetHeight() / oldH);
			batterySys.multPosAgents(ofGetWidth() / oldW, ofGetHeight() / oldH);
			bombSys.multPosAgents(ofGetWidth() / oldW, ofGetHeight() / oldH);
		}
		break;
	case 'R':
	case 'r':
		if(!stageTest)
			restart = true;
		break;
	case 'T':
	case 't':
		if (stageGameMenu == true) {
			if(stageTest == false){
				stageTest = true;
				stageGameMenu = false;
			}
		}
			break;
	}	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case OF_KEY_UP:
		player.goForward = false;
		break;
	case OF_KEY_DOWN:
		player.goReverse = false;
		break;
	case OF_KEY_LEFT:
		player.turnLeft = false;
		break;
	case OF_KEY_RIGHT:
		player.turnRight = false;
		break;

		// stop turbo
	case 'W':
	case 'w':
		if (pressedE) {
			player_speed = 250;
		}
		else
			player_speed = 300;
		pressedW = false;
		break;
		// stop slow mode
		
	case 'E':
	case 'e':
			if (pressedW) {
				player_speed = 550;
			}
			else
				player_speed = 300;
			if (!stageTest) {
				agentSpeedBeforeFreeze = agent_speed;
				agentSys.unfreezeAgentsForce();
			}
				
			else
				agentSys.unfreezeAgentsForce();
			player_scale = 1.1;
			pressedE = false;
			canPlayTimeFreezeEnd = true;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (player.draggable) {
		glm::vec3 mousePos = glm::vec3(x, y, 0);
		player.pos = mousePos - player.delta;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (stageTest) {
		cout << x << "   " << y << endl;
	}
	// is inside the triangle
	glm::vec3 mousePos = glm::vec3(x, y, 0);
	if (player.inside(mousePos.x, mousePos.y) && stageTest) {
		player.draggable = true;
		player.delta = mousePos - player.pos;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	player.draggable = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
