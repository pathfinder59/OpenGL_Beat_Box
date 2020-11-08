#include "node.h"


void Node::draw()
{
	switch (x)
	{
	case -40:
	case 40:
		glColor4f(1, 0.270588, 0.0,1);
		break;
	case -20:
	case 20:
		glColor4f(0.180392, 0.545098, 0.341176, 1);
		break;
	case 0:
		glColor3f(1, 1, 0);
		break;

	}
	glPushMatrix();
	glTranslatef(x, 0, z);
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glVertex3f(-10,5,10);
	glVertex3f(-10, 5, -10);
	glVertex3f(-10, -5, -10);
	glVertex3f(-10, -5, 10);

	glNormal3f(1, 0, 0);
	glVertex3f(10, 5, -10);
	glVertex3f(10, 5, 10);
	glVertex3f(10, -5, 10);
	glVertex3f(10, -5, -10);

	glNormal3f(0, 1, 0);
	glVertex3f(10, 5, 10);
	glVertex3f(10, 5, -10);
	glVertex3f(-10, 5, -10);
	glVertex3f(-10, 5, 10);

	glNormal3f(0, -1, 0);
	glVertex3f(10, -5, -10);
	glVertex3f(10, -5, 10);
	glVertex3f(-10, -5, 10);
	glVertex3f(-10, -5, -10);

	glNormal3f(0, 0, 1);
	glVertex3f(10, -5, 10);
	glVertex3f(10, 5, 10);
	glVertex3f(-10, 5, 10);
	glVertex3f(-10, -5, 10);

	glNormal3f(0, 0, -1);
	glVertex3f(-10, 5, -10);
	glVertex3f(10, 5, -10);
	glVertex3f(10, -5, -10);
	glVertex3f(-10, -5, -10);

	glEnd();
	//glScalef(0.2, 0.1, 0.2);
	//glutSolidCube(100);
	glPopMatrix();
}
bool Node::update(float& frameTime, float& point_per_sec, nodeChecklist& a, Nodestate& state)
{
	if (a.nTrack == x)
	{
		float distance;
		if (z < 0)
			distance = z + 200; //-20라인은 -200추가 -40라인은 -600
		else
			distance = z + -200;
		
		if (distance < 0)
			distance *= -1;
		
		if (distance < 5)
		{
			state = perpect;
			return true;
		}
		else if (distance < 10)
		{
			state = good;
			return true;
		}
		else if (distance < 25)
		{
			state = bad;
			return true;
		}
	}

	if (moveVector > 0)
	{
		if (z > -170)
		{
			state = pass;
			return true;
		}
	}
	else if (moveVector < 0)
	{
		if (z < 170)
		{
			state = pass;
			return true;
		}
	}
	z += point_per_sec * frameTime*moveVector;
	state = normal;
	return false;
}