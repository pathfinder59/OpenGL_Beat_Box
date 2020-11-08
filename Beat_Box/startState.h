#pragma once
#include "state.h"
#include <GL/freeglut.h>
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include "cheaklist.h"
class Startstate :public State
{
private:
	float Rotatesec;
	int RotatX;

public:
	Startstate() :Rotatesec(0), RotatX(0)
	{}
	void init()
	{
		Rotatesec = -90;
		RotatX = 0;

	}
	state update(nodeChecklist a);
	void draw(nodeChecklist a);



};