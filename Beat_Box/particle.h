#pragma once
#include <cmath>
#include <GL/freeglut.h>
#define PI 3.141592
class Particle
{
private:
	float x;
	float y;
	float z;
	float dx;

	float moveTheta;
	
	int rotateTheta;
	int type;  //1,2:가로가 긴 직사각형(+,-) 3,4:세로가 긴 직사각형(+,-)
public:
	Particle(float x,float y,float z,int type,int theta):x(x),y(y),z(z),dx(0),moveTheta(50),rotateTheta(theta),type(type)
	{
		
	}
	bool update(float& frameTime, float& point_per_sec)
	{
		
		if (type == 2 ||type==4||type==5 )
		{
			y = dx * tan(moveTheta*PI / 180) - (9.8*powf(dx, 2) / 2 / powf(8, 2) / powf(cos(moveTheta), 2));
			dx += 2;
		}
		else
		{
			dx *= -1;
			y = dx * tan(moveTheta*PI / 180) - (9.8*powf(dx, 2) / 2 / powf(8, 2) / powf(cos(moveTheta), 2));
			dx *= -1;
			dx -= 2;
		}
		//rotateTheta += 30;
		if (y < -100)
			return true;
		
		return false;
	}
	
	void draw()
	{
		glPushMatrix();		
		//glRotatef(rotateTheta, 0, 0, 1);
		if (type != 5)
		{
			glTranslatef(x + dx, y, z);
			switch (type)
			{
			case 1:
			case 2:
				glScalef(1, 0.5*0.5, 0.4);
				break;
			case 3:
			case 4:
				glScalef(0.5, 1, 0.4);
				break;
			}
			glutSolidCube(20);
		}
		else
		{
			glTranslatef(x, y, z);
			glRotatef(rotateTheta, 0, 1, 0);
			glTranslatef(dx, 0, 0);
			glutSolidSphere(2, 20, 10);
		}
		glPopMatrix();
	}
};
