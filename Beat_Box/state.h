#pragma once
#include "cheaklist.h"
enum state { start_state, main_state };
class State
{
public:
	virtual void draw(nodeChecklist a)=0;
	virtual state update(nodeChecklist a)=0;
};