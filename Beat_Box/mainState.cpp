#include<stdio.h>
#include<windows.h>
#include "MainState.h"
#include "TextureManager.h"
#include "Bitmap.h"
#include<MMSystem.h>
#pragma comment(lib,"Winmm.lib")
float POINT_PER_METER= 26 / 0.1;    // 10센치당 26포인트
float KMETER_PER_HOUR= 6;  //원래는 시속 18km  5미터까지 2초걸림 지금은 3초에 5m도달
float METER_PER_HOUR = KMETER_PER_HOUR*1000;  //시속 m
float METER_PER_MIN  = METER_PER_HOUR / 60; //분속 m
float METER_PER_SEC  = METER_PER_MIN / 60; //초속 m 5
float POINT_PER_SEC  = (POINT_PER_METER * METER_PER_SEC); //초속(포인트) m
//시작화면에서 사용할 update

state Mainstate::update(nodeChecklist a)
{
	if (Play)
	{
		//콤보 크기 조절
		if (charSize <= 26)
		{
			charSize += 2;
		}
		int location;
		bool end_of_schedule;
		if (rotateSwitch)
		{
			mapRotateTheta += 10;
			if (mapRotateTheta % 180 == 0)
			{
				mapRotateTheta = mapRotateTheta % 360;
				rotateSwitch = false;
				if (dir == back)
					dir = ahead;
				else
					dir = back;
			}
		}

		frame_time = (clock() / 1000.0) - current_time;
		current_time += frame_time;
		init_time += frame_time;
 		while (true == GetNode(init_time, &location, &end_of_schedule,musicN))
		{
			//회전 시 대략 2초 걸림
			if (mapRotateTheta == 0)
				nodeList.push_back(Node(location, -1500, 1));
			else
				nodeList.push_back(Node(location, 1500, -1));
		};

		Nodestate state;
		srand((int)time(NULL));
		for (vector<Node>::iterator iter = nodeList.begin(); iter != nodeList.end(); )
		{
			if (iter->update(frame_time, POINT_PER_SEC, a, state))
			{
				if (state != pass)
				{
					charSize = 0;
					if (a.nTrack == 0)
						rotateSwitch = true;
					if (combo <= 20)
					{
						int num = rand() % 2;
						if (num == 0)
						{
							particle.push_back(Particle(iter->getX(), 0, iter->getZ(), 1, 0));
							particle.push_back(Particle(iter->getX(), 0, iter->getZ(), 2, 0));
						}
						else
						{
							particle.push_back(Particle(iter->getX(), 0, iter->getZ(), 3, 0));
							particle.push_back(Particle(iter->getX(), 0, iter->getZ(), 4, 0));
						}
					}
					else
					{
						for (int j = 0; j < 360; j += 36)
						{
							particle.push_back(Particle(iter->getX(), 0, iter->getZ(), 5, j));
						}
					}
					combo += 1;
					score += ((combo / 50) + 1) * 100;
				}
				else
				{
					if (iter->getX() == 0)
						rotateSwitch = true;
					hp -= 1;
					combo = 0;
					if (hp <= -0)
					{
						Play = false;
						sndPlaySoundA("GameOver.wav", SND_LOOP | SND_ASYNC | SND_NODEFAULT);
					}
				}
				iter = nodeList.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		for (vector<Particle>::iterator iter = particle.begin(); iter != particle.end(); )
		{
			if (iter->update(frame_time, POINT_PER_SEC))
			{

				iter = particle.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		if ((true == end_of_schedule) && nodeList.empty())
		{
			Play = false;
			Clear = true;
			sndPlaySoundA("GameClear.wav", SND_LOOP | SND_ASYNC | SND_NODEFAULT);
		}	//다음 스테이지로 이동 아 참고로 위 조건문에 음악이 끝에 도달했는지 까지 확인해야함
	}
	else  // 게임 오버 시
	{
		if (a.checkSpace == true)
		{
			init();
			sndPlaySoundA("GameMain.wav", SND_LOOP | SND_ASYNC | SND_NODEFAULT);
			return start_state;
		}
	 }
	
	return main_state; //다음 단계로 진행이 아니라면 항상 자기 state변수 반환
}

//시작화면을 그리는 함수
void drawLine(nodeChecklist a)
{
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	//조명
	GLfloat AmbientLight[] = { 0.9f, 0.9f, 0.9f, 0.0f }; // 녹색조명
	GLfloat DiffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 적색조명 
	GLfloat SpecularLight[] = { 0.5, 0.5, 0.5, 1 }; // 백색조명
	GLfloat lightPos[] = { 0.0, 30.0, 0.0, 1.0 }; // 위치: (0, 200, 0) 
	

	glEnable(GL_LIGHTING);
	glShadeModel(GL_FLAT);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientLight);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseLight);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularLight);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPopMatrix();
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	if (a.checkA)
	{
		glColor4f(1.0, 0.0, 0.0,0.5);
		glVertex3f(-50, -5, -650);
		glVertex3f(-50, -5, 650);
		glVertex3f(-30, -5, 650);
		glVertex3f(-30, -5, -650);
		
	}
	else if (a.checkL)
	{
		glColor4f(1, 0, 0,0.5);
		glVertex3f(30, -5, -650);
		glVertex3f(30, -5, 650);
		glVertex3f(50, -5, 650);
		glVertex3f(50, -5, -650);
		
	}
	else if (a.checkD)
	{
		glColor4f(0.12549, 0.698039, 0.666667,0.5);
		glVertex3f(-30, -5, -650);
		glVertex3f(-30, -5, 650);
		glVertex3f(-10, -5, 650);
		glVertex3f(-10, -5, -650);
		
	}
	else if (a.checkJ)
	{
		glColor4f(0.12549, 0.698039, 0.666667,0.5);
		glVertex3f(10, -5, -650);
		glVertex3f(10, -5, 650);
		glVertex3f(30, -5, 650);
		glVertex3f(30, -5, -650);
		
	}
	else if (a.checkSpace)
	{
		glColor4f(1, 1, 0,0.2);
		glVertex3f(-10, -5, -650);
		glVertex3f(-10, -5, 650);
		glVertex3f(10, -5, 650);
		glVertex3f(10, -5, -650);
		
	}
	glEnd();
	glPopMatrix();
}
void Mainstate::draw(nodeChecklist a)
{

	if (Play)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(30.0f, 600 / 600, 10.0, 2000.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		//gluLookAt(0, 80, 100, 0, -70, -800, 0, 1, 0); //main stat
		gluLookAt(50 * sin(mapRotateTheta*PI / 180), 80, 50 * cos(mapRotateTheta*PI / 180),
			-800 * sin(mapRotateTheta*PI / 180), -70, -800 * cos(mapRotateTheta*PI / 180), 0, 1, 0); //main stat

		glEnable(GL_NORMALIZE);


		for (vector<Node>::iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
		{
			iter->draw();
		}
		for (vector<Particle>::iterator iter = particle.begin(); iter != particle.end(); ++iter)
		{
			iter->draw();
		}
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(0, 0, 200);
		glScalef(1, 0.1, 0.2);
		glutWireCube(100);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(0, 0, -200);
		glScalef(1, 0.1, 0.2);
		glutWireCube(100);
		glPopMatrix();

		//HP 앞
		glPushMatrix();
		glTranslatef(-200, -50, -1000);

		if (hp == 5)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP5);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP5, tmpRetrunVal82);
		}
		else if (hp == 4)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP4);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP4, tmpRetrunVal82);
		}
		else if (hp == 3)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP3);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP3, tmpRetrunVal82);
		}

		else if (hp == 2)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP2);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP2, tmpRetrunVal82);
		}

		else if (hp == 1)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP1);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP1, tmpRetrunVal82);
		}

		glDisable(GL_DEPTH_TEST);
		auto tmpRetrunVa334 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP_TOOL);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(-15, 200, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(-15, 0, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(15, 0, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(15, 200, 0);
		}
		glEnd();
		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP_TOOL, tmpRetrunVa334);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();

		//HP 뒤
		glPushMatrix();
		glTranslatef(200, -50, 1000);

		if (hp == 5)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP5);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(-15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP5, tmpRetrunVal82);
		}
		else if (hp == 4)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP4);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(-15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP4, tmpRetrunVal82);
		}
		else if (hp == 3)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP3);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(-15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP3, tmpRetrunVal82);
		}
		else if (hp == 2)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP2);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(-15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP2, tmpRetrunVal82);
		}

		else if (hp == 1)
		{
			auto tmpRetrunVal82 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP1);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(15, 200, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(15, 0, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(-15, 0, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(-15, 200, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP1, tmpRetrunVal82);
		}


		glDisable(GL_DEPTH_TEST);
		auto tmpRetrunVa3341 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::HP_TOOL);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(15, 200, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(15, 0, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-15, 0, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(-15, 200, 0);
		}
		glEnd();
		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::HP_TOOL, tmpRetrunVa3341);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();




		///배경 양 옆

		glPushMatrix();
		glTranslatef(400, 50, 0);
		auto tmpRetrunVal8 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SIDE);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(0, 200, -1500);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(0, -200, -1500);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(0, -200, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(0, 200, 0);
		}
		glEnd();

		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SIDE, tmpRetrunVal8);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(400, 50, 0);
		auto tmpRetrunVal9 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SIDE);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(0, 200, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(0, -200, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(0, -200, 1500);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(0, 200, 1500);
		}
		glEnd();

		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SIDE, tmpRetrunVal9);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-400, 50, 0);
		auto tmpRetrunVal10 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SIDE);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(0, 200, -1500);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(0, -200, -1500);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(0, -200, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(0, 200, 0);
		}
		glEnd();

		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SIDE, tmpRetrunVal10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-400, 50, 0);
		auto tmpRetrunVal11 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SIDE);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(0, 200, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(0, -200, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(0, -200, 1500);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(0, 200, 1500);
		}
		glEnd();

		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SIDE, tmpRetrunVal11);
		glPopMatrix();

		//바닥
		glPushMatrix();
		glTranslatef(0, -50, 0);
		auto tmpRetrunVal12 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::UNDER);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(-400, 0, -1500);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(-400, 0, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(400, 0, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(400, 0, -1500);
		}
		glEnd();

		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::UNDER, tmpRetrunVal12);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0, -50, 0);
		auto tmpRetrunVal13 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::UNDER);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(400, 0, 1500);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(400, 0, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-400, 0, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(-400, 0, 1500);
		}
		glEnd();

		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::UNDER, tmpRetrunVal13);
		glPopMatrix();
		//배경 앞,뒤
		glPushMatrix();
		glTranslatef(0, 50, -1400);
		auto tmpRetrunVal6 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::BACKGROUND);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(-400, 200, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(-400, -200, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(400, -200, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(400, 200, 0);
		}
		glEnd();

		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::BACKGROUND, tmpRetrunVal6);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 50, 1400);
		auto tmpRetrunVal7 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::BACKGROUND);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(400, 200, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(400, -200, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-400, -200, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(-400, 200, 0);
		}
		glEnd();

		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::BACKGROUND, tmpRetrunVal7);
		glPopMatrix();



		//점수판
		//앞면
		glPushMatrix();
		glTranslatef(0, 16, -100);
		glDisable(GL_DEPTH_TEST);
		auto tmpRetrunVa66 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SCORE);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(-30, 10, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(-30, 0, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(30, 0, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(30, 10, 0);
		}
		glEnd();
		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SCORE, tmpRetrunVa66);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		//뒷면
		glPushMatrix();
		glTranslatef(0, 16, 100);
		glDisable(GL_DEPTH_TEST);
		auto tmpRetrunVa661 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SCORE);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(30, 10, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(30, 0, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-30, 0, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(-30, 10, 0);
		}
		glEnd();
		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SCORE, tmpRetrunVa661);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		//점수

		/////////////////뒷면

		glPushMatrix();
		glTranslatef(0, 20, 100);
		char string1[255];
		char *p1;
		p1 = _itoa(score, string1, 10);
		glRasterPos2f(-20, 0); // 문자 출력할 위치 설정
		int len1 = (int)strlen(string1);
		for (int i = 0; i < len1; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string1[i]);
		}

		glPopMatrix();


		//////////앞면
		glPushMatrix();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0, 20, -100);
		glRasterPos2f(20, 0); // 문자 출력할 위치 설정
		for (int i = 0; i < len1; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string1[i]);
		}
		glPopMatrix();


		drawLine(a);




		glPopMatrix();
		glPopMatrix();
		//콤보
		//glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-300, 300, -300, 300, -300, 300);
		glTranslatef(0, 150, 0);
		if (charSize <= 26)
		{
			glDisable(GL_DEPTH_TEST);
			auto tmpRetrunVa33522 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::COMBO);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-(charSize * 6), charSize, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-(charSize * 6), -charSize, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f((charSize * 6), -charSize, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f((charSize * 6), charSize, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::COMBO, tmpRetrunVa33522);
			glEnable(GL_DEPTH_TEST);
			//glPopMatrix();

			//콤보 백의 자리
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-300, 300, -300, 300, -300, 300);
			glTranslatef(-50, 210, 0);
			glDisable(GL_DEPTH_TEST);
			auto tmpRetrunVa2001 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE(combo / 100));
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-charSize, charSize, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-charSize, -charSize, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(charSize, -charSize, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(charSize, charSize, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE(combo / 100), tmpRetrunVa2001);
			glEnable(GL_DEPTH_TEST);




			//콤보 십의 자리
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-300, 300, -300, 300, -300, 300);
			glTranslatef(0, 210, 0);
			glDisable(GL_DEPTH_TEST);
			auto tmpRetrunVa0001 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE((combo % 100) / 10));
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-charSize, charSize, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-charSize, -charSize, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(charSize, -charSize, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(charSize, charSize, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE((combo % 100) / 10), tmpRetrunVa0001);
			glEnable(GL_DEPTH_TEST);


			//콤보 일의 자리
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-300, 300, -300, 300, -300, 300);
			glTranslatef(50, 210, 0);
			glDisable(GL_DEPTH_TEST);
			auto tmpRetrunVa1001 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE(combo % 10));
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-charSize, charSize, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-charSize, -charSize, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(charSize, -charSize, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(charSize, charSize, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE(combo % 10), tmpRetrunVa1001);
			glEnable(GL_DEPTH_TEST);
		}
	}
	else // 게임 오버 드로잉
	{  
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-300, 300, -300, 300, -300, 300);
		glPushMatrix();
		if (Clear == true)
		{
			auto tmpRetrunVa909 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::GAMEClear);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-300, 300, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-300, -300, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(300, -300, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(300, 300, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::GAMEClear, tmpRetrunVa909);
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			auto tmpRetrunVa999 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::GAMEOVER);
			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0f, 1.0f);
				glVertex3f(-300, 300, 0);
				glTexCoord2d(0.0f, 0.0f);
				glVertex3f(-300, -300, 0);
				glTexCoord2d(1.0f, 0.0f);
				glVertex3f(300, -300, 0);
				glTexCoord2d(1.0f, 1.0f);
				glVertex3f(300, 300, 0);
			}
			glEnd();
			CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::GAMEOVER, tmpRetrunVa999);
			glEnable(GL_DEPTH_TEST);
		}
		//점수판
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-300, 300, -300, 300, -300, 300);
		glTranslatef(0, 150, 0);
		glDisable(GL_DEPTH_TEST);
		auto tmpRetrunVa996 = CTextureManager::GetInstance()->EnableTextur(ENUM_TEXTURE::SCORE);
		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(-150, 60, 0);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(-150, 0, 0);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(150, 0, 0);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(150, 60, 0);
		}
		glEnd();
		CTextureManager::GetInstance()->DisableTextur(ENUM_TEXTURE::SCORE, tmpRetrunVa996);
		glEnable(GL_DEPTH_TEST);


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-300, 300, -300, 300, -300, 300);
		glTranslatef(110, 170, 10);
		char string1[255];
		char *p1;
		p1 = _itoa(score, string1, 10);
		glRasterPos3f(-60, 0,0); // 문자 출력할 위치 설정 
		int len1 = (int)strlen(string1);
		for (int i = 0; i < len1; i++)
		{
			//glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, string1[i]);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string1[i]);
		}
		glPopMatrix();
	 }
}

bool Mainstate::GetNode(float current_time, int * location, bool *end_of_schedule,int musicN)
{
	static bool opened = false;
	//static int number_of_process = 0;
	static bool end_of_node = false;
	static bool have_node = false;
	static float proc_arrive_time = 0;
	static int proc_location = 0;
	static ifstream fin1("HandClap_Node.txt", ios::in);
	static ifstream fin2("POPSTARS_Node.txt", ios::in);
	static ifstream fin3("FlandreS_Node.txt", ios::in);
	
	if (!fClock)
	{
		opened = false;
		end_of_node = false;
		have_node = false;
		proc_arrive_time = 0;
		proc_location = 0;
		fin1.seekg(0);
		fin2.seekg(0);
		fin3.seekg(0);
		fClock = true;
	}
	if (false == opened) {
		if (!(fin1 || fin2 || fin3)) {
			cerr << "Failed to open data.txt!\n";
			exit(1);
		}
	}

	if (true == end_of_node) {
		*location = 0;
		*end_of_schedule = true;
		return false;
	}

	if (false == have_node) {
		if(musicN==1)
		{
			fin1 >> proc_arrive_time >> proc_location;
		}
		else if (musicN == 2)
		{
			fin2 >> proc_arrive_time >> proc_location;
		}
		else if (musicN == 3)
		{
			fin3 >> proc_arrive_time >> proc_location;
		}
		have_node = true;
	}

	if (proc_arrive_time-3 > current_time) {
		*location = 0;
		*end_of_schedule = false;
		return false;
	}

	if (-1 == proc_arrive_time) {
		end_of_node = true;
		*location = 0;
		*end_of_schedule = true;
		return false;
	}

	*location = proc_location;

	*end_of_schedule = false;
	have_node = false;
	return true;
}