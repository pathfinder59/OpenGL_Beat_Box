#define _CRT_SECURE_NO_WARNINGS
#include "startState.h"
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include<MMSystem.h>
#include "TextureManager.h"
#include "Bitmap.h"


//����ȭ�鿡�� ����� update
state Startstate::update(nodeChecklist a)
{
 	if (a.checkSpace == true)
	{
		if (Rotatesec > -90)
		{
			Rotatesec -= 10;
		}

	}
	if (a.cheak1 == true)
	{
		if (RotatX > -180)
		{
			RotatX -= 10;
		}
	}
	if (a.cheak2 == true)
	{
		if (RotatX > -90)
		{
			RotatX -= 10;
		}

		else if (RotatX < -90)
		{
			RotatX += 10;
		}
	}

	if (a.cheak3 == true)
	{
		if (RotatX < 90)
		{
			RotatX += 10;
		}
	}
	return start_state; //���� �ܰ�� ������ �ƴ϶�� �׻� �ڱ� state���� ��ȯ
}
int a = -800;
//����ȭ���� �׸��� �Լ�
void Startstate::draw(nodeChecklist a)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30.0f, 600 / 600, 10.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);

	gluLookAt(0, 0, 950, 0, 0, 1, 0, 1, 0);  //start stat
	//glClearColor(0.12549, 0.698039, 0.666667, 1.0f); // ������ 
	//
	//
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_DEPTH_TEST);  // ���� ����
	//glFrontFace(GL_CCW);   // �ո� ���� 




	// �ؽ�ó�� ��ü�� ����
	glPushMatrix();
	

	glRotatef(Rotatesec, 0, 1, 0);
	glRotatef(RotatX, 0, 0, 1);

	//����
	glPushMatrix();
	glTranslatef(0, 0, 200);
	auto tmpRetrunVal = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::MAIN);
	glBegin(GL_POLYGON);
	{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-200, 200, 0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-200, -200, 0);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(200, -200, 0);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(200, 200, 0);
	}
	glEnd();

	CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::MAIN, tmpRetrunVal);
	glPopMatrix();


	//GameText
	glPushMatrix();
	glTranslatef(200, 0, 0);

	glRotatef(90, 0, 1, 0);
	auto tmpRetrunVal1 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::TEXT);
	glBegin(GL_POLYGON);
	{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-200, 200, 0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-200, -200, 0);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(200, -200, 0);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(200, 200, 0);
	}
	glEnd();

	CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::TEXT, tmpRetrunVal1);
	glPopMatrix();


	//POPSTARS
	glPushMatrix();
	glTranslatef(0, 200, 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	auto tmpRetrunVal2 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SONG2);
	glBegin(GL_POLYGON);
	{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(200, 200, 0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(200, -200, 0);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(-200, -200, 0);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(-200, 200, 0);
	}
	glEnd();
	//FLRAN
	CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SONG2, tmpRetrunVal2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, -200, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	auto tmpRetrunVal3 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SONG3);
	glBegin(GL_POLYGON);
	{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-200, 200, 0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-200, -200, 0);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(200, -200, 0);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(200, 200, 0);
	}
	glEnd();

	CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SONG3, tmpRetrunVal3);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-200, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(180, 0, 0, 1);
	//HandClap
	auto tmpRetrunVal4 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SONG1);
	glBegin(GL_POLYGON);
	{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(200, 200, 0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(200, -200, 0);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(-200, -200, 0);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(-200, 200, 0);
	}
	glEnd();

	CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SONG1, tmpRetrunVal4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -200);
	glRotatef(180, 0, 1, 0);
	auto tmpRetrunVal5 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::MAIN);
	glBegin(GL_POLYGON);
	{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-200, 200, 0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-200, -200, 0);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(200, -200, 0);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(200, 200, 0);
	}
	glEnd();

	CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::MAIN, tmpRetrunVal5);
	glPopMatrix();
	glPopMatrix();

	
}