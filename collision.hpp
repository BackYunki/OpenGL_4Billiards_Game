#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define GLM_ENABLE_EXPERIMENTAL

#define PI 3.141592
#define RADIUS 17.6f
#define DTOR (PI/180)

#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <glm.hpp>
#include <gtx/fast_square_root.hpp>
using namespace glm;

// 공번호
const int Y = 0;
const int W = 1;
const int R1 = 2;
const int R2 = 3;
const int N = 4;

// 상수들
const double dt = 0.06f;
const double epsilon = 0.1f;
const double friction = 0.12f;
const double SCREEN_WIDTH = 400.f;
const double SCREEN_HEIGHT = 800.f;

// 마우스 상태, 차례, 게임 진행 횟수, 쿠션 목표치
typedef enum Action
{
	NOTHING,
	CLICKED,
	DRAG,
	RELEASED
}Action;
int turn = Y;
int action = NOTHING;
bool showInfo = 0;
int cusionUnit = 3;

// 공, 충돌시간
typedef struct RigidBody
{
	vec2 p;
	vec3 c;
	vec2 v;
	vec2 a;
	bool isHit;
}RigidBody;
RigidBody Ball[4];
GLdouble t = 0.f;

// 정지여부, 연속치기 상태
bool isStop = false;

// 점수, 쿠션 횟수
int score[] = { 0, 0 };
int cusion[] = { 0, 0 };

// 마우스 좌표
double mouse_x, mouse_y;

/*
   input   : 서로 다른 물체
   output   : 물체끼리의 충돌 시간
*/
GLdouble getCollisionTime(RigidBody u, RigidBody v)
{
	vec2 dv = u.v - v.v;
	vec2 dp = u.p - v.p;

	GLdouble a = dot(dv, dv);
	GLdouble b = dot(dv, dp);
	GLdouble c = dot(dp, dp) - 4 * RADIUS*RADIUS;
	GLdouble d = (b * b) - (a * c);

	if (d <= 0) t = -1.f;
	else t = (-b - sqrt(d)) / a;

	return t;
}

void showScore() { printf("[Score] Y: %d\tW: %d\n", score[Y], score[W]); }
void initBalls()
{
	// 공과 벽의 위치 및 색깔 초기화
	Ball[Y].p = vec2(0, 300);
	Ball[W].p = vec2(50, -200);
	Ball[R1].p = vec2(0, 200);
	Ball[R2].p = vec2(0, -200);
	Ball[Y].c = vec3(1.f, 0.8f, 0.f);
	Ball[W].c = vec3(1.f, 1.f, 1.f);
	Ball[R1].c = vec3(1.f, 0.f, 0.f);
	Ball[R2].c = vec3(1.f, 0.f, 0.f);
}

void MyMouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT)
	{
		switch (state)
		{
		case GLUT_DOWN: action = CLICKED; break;
		case GLUT_UP: if (isStop) { action = RELEASED; break; }
		}
	}
}
void MyMotion(int x, int y)
{
	if (isStop) action = DRAG;
	else action = NOTHING;
	mouse_x = x - 200; mouse_y = 400 - y; // GLUT좌표계와 glortho 의 좌표계가 달라서 변환해줘야한다.
}
void MySpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:		if (cusionUnit > 0) { cusionUnit--; printf("\n목표쿠션: %d\n", cusionUnit); } break;
	case GLUT_KEY_RIGHT:	cusionUnit++; printf("\n목표쿠션: %d\n", cusionUnit); break;
	case GLUT_KEY_UP:		score[Y] += 10; score[W] += 10; showScore(); break;
	case GLUT_KEY_DOWN:		if (score[Y] > 0 && score[W] > 0) { score[Y] -= 10; score[W] -= 10; showScore(); } break;
	}
}

void changeTurn()
{
	if (turn == Y) turn = W;
	else if (turn == W) turn = Y;
}
void initHitCount() { for (int i = 0; i < N; i++) Ball[i].isHit = false; }
void promptClear() { printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); }
void gameReset()
{
	score[0] = 0; score[1] = 0;
	initBalls();
	for (int i = 0; i < N; i++)
	{
		Ball[i].v = vec2(0, 0);
	}
	turn = Y;
	printf("==========game reseted=========\n");
}
void showHelp()
{
	printf(">========================게임규칙========================<\n");
	printf("   1. 노란공으로 시작한다.\n");
	printf("   2. 공을 클릭하면 회색 보조선이 나타나고\n\t   파란색 선의 길이에 따라 속도가 결정된다.\n");
	printf("   3. 0점에 도달한 사람은 목적구를 2개 침과 동시에\n\t   쿠션회수를 만족하면 게임에서 승리한다.\n");
	printf("   4. 자신의 공으로 상대방의 공을 타격하거나\n\t   목적구를 하나도 치지 못하면 패널티를 받는다.\n");
	printf("   5. ↑ ↓ 화살표: 목표점수 증/감\n");
	printf("   6. ← → 화살표: 목표쿠션 증/감\n");
	printf("   7. Esc 게임종료, Enter 화면청소\n");
	printf(">========================================================<\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void MyKeyboard(unsigned char KeyPressed, int x, int y)
{
	switch (KeyPressed)
	{
	case 13: promptClear(); break;		//Enter	: 화면청소
	case 27: exit(1); break;			//EXC키	: 종료
	case 'r': gameReset(); break;		//'r'	: 게임 리셋
	case 'h': showHelp(); break;		//'h'	: 게임 설명
	}
}

/*
	다마를 그려준다.
	sx: 시점 x
	sy: 시점 y
	ex: 종점 x
	ey: 종점 y
*/
void draw_DAMA()
{
	glLineWidth(10.f);
	glColor3d(0.4f, 0.24f, 0.16f);
	glBegin(GL_LINE_STRIP);
	glVertex2d(-200.f, 400.f);
	glVertex2d(-200.f, -400.f);
	glVertex2d(200.f, -400.f);
	glVertex2d(200.f, 400.f);
	glVertex2d(-200.f, 400.f);
	glEnd();
}

/*
	선분을 그린다.
	sx: 시점 x
	sy: 시점 y
	ex: 종점 x
	ey: 종점 y
*/
void draw_Line(GLdouble sx, GLdouble sy, GLdouble ex, GLdouble ey)
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2d(sx, sy);
	glVertex2d(ex, ey);
	glEnd();
}

void draw_Circle(GLdouble cx, GLdouble cy, GLdouble r, int segments)
{
	GLdouble x, y = 0.f;
	GLdouble interRad = (360.0 / segments) * DTOR;

	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < segments; ++i)
	{
		x = r * cosf(i*interRad);
		y = r * sinf(i*interRad);
		// 변환 후 원의 중심으로 평행이동한 점을 그린다.
		glVertex2d(cx + x, cy + y);
	}
	glEnd();
}

/*
   위치를 업데이트함과 동시에 물체의 정지여부를 판단한다.
*/
void updatePosition(GLdouble T)
{
	int stopCnt = 0;
	for (int i = 0; i < N; ++i)
	{
		// 공들이 적당히 느려졌다 싶으면 정지시키고 
		// stopCnt를 늘려서 stopCnt가 4가 되면 모두 멈춘 것이므로 다음 턴으로 넘어간다.
		if (length(Ball[i].v) < epsilon)
		{
			Ball[i].v = vec4(0, 0, 0, 0);
			Ball[i].a = vec4(0, 0, 0, 0);
			stopCnt++;
		}
		else
		{
			Ball[i].v += Ball[i].a;
			Ball[i].p += Ball[i].v * mat2(T);
		}

		glColor3d(Ball[i].c.r, Ball[i].c.g, Ball[i].c.b);
		draw_Circle(Ball[i].p.x, Ball[i].p.y, RADIUS, 16);
	}
	if (stopCnt == 4) isStop = true;
	else isStop = false;
}

void makeContext(int argc, char** argv, const char* title)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(500, 100);
	glutCreateWindow(title);

	glMatrixMode(GL_PROJECTION);	glLoadIdentity();

	glOrtho(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, -1000.0, 1.0);	// znear : -(-1.0) , zfar : -(1.0) 
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
}