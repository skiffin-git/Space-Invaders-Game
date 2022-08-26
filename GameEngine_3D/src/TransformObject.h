#pragma once
#include "ofMain.h"


//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith  
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>


//  Base class for any object that needs a transform.
//
class TransformObject {
protected:
	TransformObject();
	ofVec3f position, scale;
	float	rotation;
	bool	bSelected;
public:
	void setPosition(const ofVec3f &);
};