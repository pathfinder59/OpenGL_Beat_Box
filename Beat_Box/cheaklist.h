#pragma once

typedef struct nodeChecklist
{
	bool checkA;
	bool checkD;
	bool checkJ;
	bool checkL;
	bool checkSpace;
	int nTrack;
	bool cheak1;
	bool cheak2;
	bool cheak3;
	nodeChecklist()
	{
		checkA = false;
		checkD = false;
		checkJ = false;
		checkL = false;
		checkSpace = false;
		nTrack = -200;
		cheak1 = false;
		cheak2 = false;
		cheak3 = false;
	}
	void init()
	{
		checkA = false;
		checkD = false;
		checkJ = false;
		checkL = false;
		checkSpace = false;
		cheak1 = false;
		cheak2 = false;
		cheak3 = false;
		nTrack = -200;

	}
};
