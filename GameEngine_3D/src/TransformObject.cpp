#pragma once
#include "TransformObject.h"

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
TransformObject::TransformObject() {
	position = ofVec3f(0, 0, 0);
	scale = ofVec3f(1, 1, 1);
	rotation = 0;
}

void TransformObject::setPosition(const ofVec3f & pos) {
	position = pos;
}
