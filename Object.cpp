/*
 * Object.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: andrej
 */

#include "Object.h"

Object::Object() {
	// TODO Auto-generated constructor stub

}

Object::~Object() {
	// TODO Auto-generated destructor stub
}

int Object::getXPos(){


	return Object::xPos;
}
void Object::setXPos(int x){
	Object::xPos = x;
	xPos = x;
}

int Object::getYPos(){


	return Object::yPos;
}
void Object::setYPos(int y){
	Object::yPos = y;
	yPos = y;
}
