#pragma once
#include "state.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <string>
#include <ctime>
#include <vector>
#include"node.h"
#include "particle.h"

using namespace std;

enum Dir {ahead,back};

class Mainstate :public State
{
private:
	int musicN;
	static ifstream fin;
	float current_time;
	float frame_time;
	float init_time;
	bool rotateSwitch;
	int mapRotateTheta;
	int score;
	int hp;
	int combo;
	int charSize;
	bool Play;
	bool Clear;
	bool fClock;
	vector<Node> nodeList;
	vector<Particle> particle;
	Dir dir = ahead;
public:
	Mainstate() :init_time(0), current_time(clock() / 1000.0), frame_time(0),
		rotateSwitch(false), mapRotateTheta(0), combo(0), score(0), hp(5), charSize(29),Play(true),musicN(1),Clear(false),fClock(false)
	{
	}
	void init()
	{
		init_time = 0;
		current_time = (clock() / 1000.0);
		frame_time = 0;
		rotateSwitch = (false);
		mapRotateTheta = (0);
		combo = (0);
		score = (0);
		hp = (5);
		charSize = (29);
		Play = (true);
		musicN = (1);
		Clear=(false);
		fClock = false;
	}
	void initTime()
	{
		current_time = clock() / 1000.0;
		frame_time = 0;
	}
	void setN(int music_number)
	{
		musicN = music_number;
	}
	state update(nodeChecklist a);
	void draw(nodeChecklist a);
	bool GetNode(float current_time, int * location, bool *end_of_schedule, int musicN);
	
};


void drawLine(nodeChecklist a);