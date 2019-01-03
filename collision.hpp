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

// ����ȣ
const int Y = 0;
const int W = 1;
const int R1 = 2;
const int R2 = 3;
const int N = 4;

// �����
const double dt = 0.06f;
const double epsilon = 0.1f;
const double friction = 0.12f;
const double SCREEN_WIDTH = 400.f;
const double SCREEN_HEIGHT = 800.f;

// ���콺 ����, ����, ���� ���� Ƚ��, ��� ��ǥġ
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

// ��, �浹�ð�
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

// ��������, ����ġ�� ����
bool isStop = false;

// ����, ��� Ƚ��
int score[] = { 0, 0 };
int cusion[] = { 0, 0 };

// ���콺 ��ǥ
double mouse_x, mouse_y;

/*
   input   : ���� �ٸ� ��ü
   output   : ��ü������ �浹 �ð�
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
	// ���� ���� ��ġ �� ���� �ʱ�ȭ
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
	mouse_x = x - 200; mouse_y = 400 - y; // GLUT��ǥ��� glortho �� ��ǥ�谡 �޶� ��ȯ������Ѵ�.
}
void MySpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:		if (cusionUnit > 0) { cusionUnit--; printf("\n��ǥ���: %d\n", cusionUnit); } break;
	case GLUT_KEY_RIGHT:	cusionUnit++; printf("\n��ǥ���: %d\n", cusionUnit); break;
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
	printf(">========================���ӱ�Ģ========================<\n");
	printf("   1. ��������� �����Ѵ�.\n");
	printf("   2. ���� Ŭ���ϸ� ȸ�� �������� ��Ÿ����\n\t   �Ķ��� ���� ���̿� ���� �ӵ��� �����ȴ�.\n");
	printf("   3. 0���� ������ ����� �������� 2�� ħ�� ���ÿ�\n\t   ���ȸ���� �����ϸ� ���ӿ��� �¸��Ѵ�.\n");
	printf("   4. �ڽ��� ������ ������ ���� Ÿ���ϰų�\n\t   �������� �ϳ��� ġ�� ���ϸ� �г�Ƽ�� �޴´�.\n");
	printf("   5. �� �� ȭ��ǥ: ��ǥ���� ��/��\n");
	printf("   6. �� �� ȭ��ǥ: ��ǥ��� ��/��\n");
	printf("   7. Esc ��������, Enter ȭ��û��\n");
	printf(">========================================================<\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void MyKeyboard(unsigned char KeyPressed, int x, int y)
{
	switch (KeyPressed)
	{
	case 13: promptClear(); break;		//Enter	: ȭ��û��
	case 27: exit(1); break;			//EXCŰ	: ����
	case 'r': gameReset(); break;		//'r'	: ���� ����
	case 'h': showHelp(); break;		//'h'	: ���� ����
	}
}

/*
	�ٸ��� �׷��ش�.
	sx: ���� x
	sy: ���� y
	ex: ���� x
	ey: ���� y
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
	������ �׸���.
	sx: ���� x
	sy: ���� y
	ex: ���� x
	ey: ���� y
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
		// ��ȯ �� ���� �߽����� �����̵��� ���� �׸���.
		glVertex2d(cx + x, cy + y);
	}
	glEnd();
}

/*
   ��ġ�� ������Ʈ�԰� ���ÿ� ��ü�� �������θ� �Ǵ��Ѵ�.
*/
void updatePosition(GLdouble T)
{
	int stopCnt = 0;
	for (int i = 0; i < N; ++i)
	{
		// ������ ������ �������� ������ ������Ű�� 
		// stopCnt�� �÷��� stopCnt�� 4�� �Ǹ� ��� ���� ���̹Ƿ� ���� ������ �Ѿ��.
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