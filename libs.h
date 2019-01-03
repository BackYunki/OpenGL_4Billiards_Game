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
	accTick�� 1000�и��ʰ� �� ������ �����Ӱ� ����� ƽ�� �����Ѵ�. �� ��ŭ�� �������� �׷����� ���� �˷��ش�.

	frame		: display �Լ��� ȣ�� �� ������ ȭ���� ���α׸��� ������, ȣ��� Ƚ���� ������ ���� ���ٴ� ���� �� �� �ִ�.
	backupCount : ������ tick
	intervalTick: ����tick�� ����tick�� ���̰�.
				=> display �Լ��� ������ ȣ��� �� backupTick�� ���� ��. �� ó������ currentTick�� ��û ū ���� �� ����.
	accTick		: intervalTick�� �����ӵ鰣�� �ð����̱� ������ �̵��� accTick �� �׾Ƶΰ� 1000�и������尡 �Ǹ� 1�ʰ� �� �� �� �� �� ����.
				=> 1000msec ���� ũ�� 0���� ����� �� ������ ���� ������ ���� ����ϸ� 1�� ���� �� �������� ����ƴ����� �� �� �ִ�.
*/
void printFPS()
{
	frame++;	   //Current Tick   //BackupedTick
	intervalTick = GetTickCount() - postTick;
	postTick = GetTickCount();
	totalTick += intervalTick;	    // ���� �������� �׸��� �� �ɸ� �ð�.

	if (totalTick > 1000)
	{
		totalTick = 0;
		printf("%u\n", frame);
		frame = 0;
	}
}

/*
	��2���� ����

	c_x: ���߽� x
	c_y: ���߽� y
	 r : ������
	segments : ���� ����

	interRadian : ���׸�Ʈ ������ ���� �������� ��ȯ�� ��. DTOR = PI/180
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
		// ��ȯ �� ���� �߽����� �����̵��� ���� �׸���.
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
	�־��� ���� ȸ����ŵ�ϴ�.

	dTheta : �� �����Ӵ� ������ ����
*/
void rotate_Circle()
{
	GLdouble dTheta = 360 * (totalTick * 0.001);

	glPushMatrix();
	glColor3d(0.f, 0.f, 0.f);
	glRotated(dTheta, 0, 0, 1);
	//draw_Circle(cx, cy, r, segments); <= 2���� ����
	glTranslated(200, 0, 0);
	glutSolidSphere(100, 32, 2);
	glPopMatrix();
}

// ��ǥ�� �׸���
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