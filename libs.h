#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define PI 3.141592
#define DTOR (PI/180)

#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <glm.hpp>
#include <thread>

using namespace glm;

const GLdouble SCREEN_WIDTH = 800.f;
const GLdouble SCREEN_HEIGHT = 800.f;

GLuint postTick, intervalTick, totalTick, frame = 0;
/*
	accTick이 1000밀리초가 될 때까지 프레임과 경과된 틱을 축적한다. 얼마 만큼의 프레임이 그려지는 지를 알려준다.

	frame		: display 함수가 호출 될 때마다 화면을 새로그리기 때문에, 호출된 횟수는 프레임 수와 같다는 것을 알 수 있다.
	backupCount : 과거의 tick
	intervalTick: 현재tick과 과거tick의 차이값.
				=> display 함수가 다음에 호출될 때 backupTick에 값이 들어감. 맨 처음에는 currentTick에 엄청 큰 값이 들어갈 것임.
	accTick		: intervalTick이 프레임들간의 시간차이기 때문에 이들을 accTick 에 쌓아두고 1000밀리세컨드가 되면 1초가 된 것 을 알 수 있음.
				=> 1000msec 보다 크면 0으로 만들고 이 때까지 쌓인 프레임 수를 출력하면 1초 동안 몇 프레임이 재생됐는지를 알 수 있다.
*/
void printFPS()
{
	frame++;	   //Current Tick   //BackupedTick
	intervalTick = GetTickCount() - postTick;
	postTick = GetTickCount();
	totalTick += intervalTick;	    // 단위 프레임을 그리는 데 걸린 시간.

	if (totalTick > 1000)
	{
		totalTick = 0;
		printf("%u\n", frame);
		frame = 0;
	}
}

/*
	※2차원 전용

	c_x: 원중심 x
	c_y: 원중심 y
	 r : 반지름
	segments : 변의 개수

	interRadian : 세그먼트 사이의 각을 라디안으로 변환한 것. DTOR = PI/180
*/
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

void draw_Line(GLdouble sx, GLdouble sy, GLdouble ex, GLdouble ey)
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2d(sx, sy);
	glVertex2d(ex, ey);
	glEnd();
}

/*
	주어진 원을 회전시킵니다.

	dTheta : 한 프레임당 증가한 각도
*/
void rotate_Circle()
{
	GLdouble dTheta = 360 * (totalTick * 0.001);

	glPushMatrix();
	glColor3d(0.f, 0.f, 0.f);
	glRotated(dTheta, 0, 0, 1);
	//draw_Circle(cx, cy, r, segments); <= 2차원 전용
	glTranslated(200, 0, 0);
	glutSolidSphere(100, 32, 2);
	glPopMatrix();
}

// 좌표축 그리기
void draw_Axis()
{
	GLdouble length = SCREEN_WIDTH;
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	//X-axis (red)
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(length, 0);

	//Y-axis (green)
	glColor3f(0, 1, 0);
	glVertex2f(0, 0);
	glVertex2f(0, length);

	glEnd();
}

void makeNomalizedContext(int argc, char** argv, const char* title)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(title);

	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);	// znear : -(-1.0) , zfar : -(1.0) 
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
}

void makeContext(int argc, char** argv, const char* title)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(title);

	glMatrixMode(GL_PROJECTION);	glLoadIdentity();

	glOrtho(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, -1000.0, 1.0);	// znear : -(-1.0) , zfar : -(1.0) 
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
}