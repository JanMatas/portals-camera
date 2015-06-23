/*
 * Object.h
 *
 *  Created on: Jun 16, 2015
 *      Author: andrej
 */

#ifndef OBJECT_H_
#define OBJECT_H_
#include <string>

using namespace std;

class Object {
public:
	Object();
	virtual ~Object();

	int getXPos();
	void setXPos(int x);

	int getYPos();
	void setYPos(int Y);
private:
	int xPos, yPos;
	string type;

};

#endif /* OBJECT_H_ */
