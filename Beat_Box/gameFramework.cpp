#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include "startState.h"
#include "mainState.h"
#include<MMSystem.h>
#include "TextureManager.h"
#pragma comment(lib,"Winmm.lib")
using namespace std;

#define pi 3.1452/180

enum SongCheak{Not_song, song_HandClap, song_KDAPOPSTARS, song_FlandreS}; //Not_song : 음악 없음 , song_HandClap: HandClap 음악 , song_KDAPOPSTARS :KDAPOPSTARS 음악 , song_FlandreS : FlandreS 음악
enum StateMode{Main_Mode, Start_Mode, Game_Mode};
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void Motion(int x, int y);
void Mouse(int button, int state, int x, int y);


void SetupRC(void);
int bit = 1;
void update();
void draw();
float Modecheak = 0; // 시작하는지 확인  0:기본 1: 음악고르는곳 2:게임시작
int songcheak = 0;  //어떤 음악을 재생할지 확인
nodeChecklist List;
void main(int argc, char *argv[])
{
	srand((int)time(NULL));
	//초기화 함수들
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);  // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);  // 윈도우의 위치지정
	glutInitWindowSize(600, 600);  // 윈도우의 크기 지정
	glutCreateWindow("Example");  // 윈도우 생성 (윈도우 이름)
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(100, TimerFunction, 1);
	glutDisplayFunc(drawScene);  // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수의 지정
	SetupRC();
	CTextureManager::GetInstance();
	glutMainLoop();
	CTextureManager::DestroyInstance();
	
}
Startstate start;
Mainstate mainState;
state cur_state=start_state;
void SetupRC()
{

	glEnable(GL_DEPTH_TEST);  // 은면 제
	glFrontFace(GL_CCW);
}

// 윈도우 출력 함수
float rz = 0, ry = 0, rx = 0, xT = 0, yT = 80, zT = 0;  //main stat 
//float rz = 0, ry = 0, rx = 0, xT = 0, yT = 0, zT = 600;  //start stat 

GLvoid drawScene(GLvoid)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();

	float x, y, z;
	x = (cos(pi*rz)*cos(pi*ry)*xT) + ((cos(rz*pi)*sin(ry*pi)*sin(rx*pi) - sin(rz*pi)*cos(rx*pi))*yT) + (cos(rz*pi)*sin(ry*pi)*cos(rx*pi) + sin(rz*pi)*sin(rx*pi)) * zT;
	y = (sin(pi*rz)*cos(pi*ry)*xT) + ((sin(rz*pi)*sin(ry*pi)*sin(rx*pi) + cos(rz*pi)*cos(rx*pi))*yT) + (sin(rz*pi)*sin(ry*pi)*cos(rx*pi) - cos(rz*pi)*sin(rx*pi)) * zT;
	z = -sin(pi*ry)*xT + cos(pi*ry)*sin(rx*pi)*yT + cos(ry*pi)*cos(rx*pi) * zT;

	glPushMatrix();
	//gluLookAt(0, 80, 100, 0, s,-800, 0, 1, 0); //main stat
	//gluLookAt(x, y, 950, 0, 0, 1, 0, 1, 0);  //start stat
	switch (cur_state)
	{
	case start_state:
		cur_state = start.update(List);
		start.draw(List);
		break;

	case main_state:
		cur_state = mainState.update(List);
		mainState.draw(List);
		if (cur_state == start_state)
		{
		List.init();
		Modecheak = Start_Mode;
		start.init();
		mainState = Mainstate();
		}
		break;
	}
	//List.init();
	

	glPopMatrix();
	glutSwapBuffers();
}


void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{

	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}

void Motion(int x, int y)
{

}


void KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		List.checkA = false;
		break;
	case 'd':
	case 'D':
		List.checkD = false;
		break;
	case 'j':
	case 'J':
		List.checkJ = false;
		break;
	case 'L':
	case 'l':
		List.checkL = false;
		break;
	case ' ':
		if (Modecheak == 2)
			List.checkSpace = false;
		break;
	}
	List.nTrack = -200;
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'1':
		if (Modecheak == Start_Mode)
		{
			List.init();
			List.cheak1 = true;
			songcheak = song_HandClap;
		}
		break;
	case'2':
		if (Modecheak == Start_Mode)
		{
			List.init();
			List.cheak2 = true;
			songcheak = song_KDAPOPSTARS;
		}
		break;
	case'3':
		if (Modecheak == Start_Mode)
		{
			List.init();
			List.cheak3 = true;
			songcheak = song_FlandreS;
		}
		break;
	case 'a':
	case 'A':
		List.checkA = true;
		List.nTrack = -40;
		break;
	case 'd':
	case 'D':
		List.checkD = true;
		List.nTrack = -20;
		break;
	case 'j':
	case 'J':
		List.checkJ = true;
		List.nTrack = 20;
		break;
	case 'L':
	case 'l':
		List.checkL = true;
		List.nTrack = 40;
		break;
	case ' ':
		if (Modecheak == Game_Mode)
		{

			List.checkSpace = true;
			List.nTrack = 0;
		}
		else if (Modecheak == Main_Mode)
		{
			sndPlaySoundA("GameMain.wav", SND_LOOP|SND_ASYNC | SND_NODEFAULT);
			Modecheak = Start_Mode;
			List.checkSpace = true;
		}
		if (songcheak == song_HandClap)
		{
			sndPlaySoundA("HandClap.wav", SND_ASYNC | SND_NODEFAULT);
			cur_state = main_state;
			mainState.setN(songcheak);
			mainState.initTime();
			songcheak = Not_song;
			Modecheak = Game_Mode;
		}

		if (songcheak == song_KDAPOPSTARS)
		{
			sndPlaySoundA("KDAPOPSTARS.wav", SND_ASYNC | SND_NODEFAULT);
			cur_state = main_state;
			mainState.setN(songcheak);
			mainState.initTime();
			songcheak = Not_song;
			Modecheak = Game_Mode;
		}

		if (songcheak == song_FlandreS)
		{
			sndPlaySoundA("FlandreS.wav", SND_ASYNC | SND_NODEFAULT);
			cur_state = main_state;
			mainState.setN(songcheak);
			mainState.initTime();
			songcheak = Not_song;
			Modecheak = Game_Mode;
		}
		
		break;
	


	}
}
void TimerFunction(int value)
{

	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(30, TimerFunction, 1); // 타이머함수 재 설정
}



GLvoid Reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30.0f, w / h, 10.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);

}
void draw()
{

}

void update()
{
	

}
