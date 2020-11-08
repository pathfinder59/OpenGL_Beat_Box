#pragma once
#include <GL/freeglut.h>
#include "cheaklist.h"
enum Nodestate {bad,good,perpect,pass,normal};
class Node
{
private:
	int x;
	//int y;
	int z;
	
	int moveVector;
public:
	Node(int x,int z,int direct):x(x),z(z),moveVector(direct){}
	void draw();
	int getX()
	{
		return x;
	}
	int getZ()
	{
		return z;
	}
	bool update(float& frameTime,float& point_per_sec,nodeChecklist& a,Nodestate& state);
};

