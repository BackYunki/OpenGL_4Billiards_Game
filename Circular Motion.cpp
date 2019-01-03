#define _CRT_SECURE_NO_WARNINGS				
#define PI 3.141592							//������
#define DTOR (PI/180)						//����->����

#include <glut.h>						
#include <stdio.h>							
#include <math.h>							
#include <Windows.h>						
#include <glm.hpp>							//OpenGL ���� �� ��� ���� ���̺����
using namespace glm;

const GLdouble SCREEN_WIDTH = 800.f;
const GLdouble SCREEN_HEIGHT = 800.f;

//-------Circle--------
vec4 c = vec4(200, 0, 0, 1);				//�����߽�
vec4 o = vec4(0, 0, 0, 1);					//ȸ���� �߽�
vec4 r = c - o;								//r(t)����
vec4 a;										//���ӵ�
vec4 v = vec4(0, 0.2, 0, 0);				//�ʱ�ӵ�

GLdouble R = sqrt(dot(r, r));				//ȸ�� ������
GLfloat dt = 1.f;							//�ִϸ��̼� �ð� ����
GLfloat dir_dt = 1.f;
//------Pendulum-------
vec4 C = vec4(0, SCREEN_HEIGHT / 2, 0, 1);	//��C
vec4 P = vec4(300, 0, 0, 1);				//��P

vec4 g = vec4(0, -100, 0, 0);				//�߷°��ӵ� g
vec4 A = vec4(0, 0, 0, 0);					//���ӵ��� ��ȭ
vec4 V = vec4(0, 0, 0, 0);					//�ӵ��� ��ȭ

vec4 in = C - P;							//�� P�� C�� �մ� ����
GLdouble R2 = sqrt(dot(in, in));			//���� ����
GLdouble v_backup;

int CIRCLE_MODE = -1;						//��� ����ġ
int PENDULUM_MODE = -1;						//���ڿ ����ġ

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

void display(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// ���
	if (CIRCLE_MODE == 1)
	{
		glColor3d(0.f, 0.f, 1.f);
		draw_Circle(r.x, r.y, 100, 32);
		a = mat4(-dot(v, v) / R) * normalize(r);
		v += a;
		r += mat4(dt) * v;

		r = mat4(R) * normalize(r);
	}
	// ���ڿ
	else if (PENDULUM_MODE == 1)
	{
		v_backup = length(V);

		in = C - P;
		vec4 normalized_in = normalize(in);
		vec4 crossVector = vec4(cross(vec3(0, -1, 0), -vec3(normalized_in.x, normalized_in.y, 0)), 0);
		A = mat4(normalize(crossVector).z * length(g)) * length(crossVector) * normalize(vec4(cross(vec3(0, 0, 1), vec3(in.x, in.y, 0)), 0));
		V += A;
		P += V * mat4(dt);

		P = C - mat4(R2) * normalize(C - P);	//����

		// õ���߽�
		glColor3d(0.f, 0.f, 0.f);
		draw_Circle(0, SCREEN_HEIGHT / 2, 25, 16);

		// ��
		glColor3f(0.f, 0.f, 0.f);
		draw_Line(C.x, C.y, P.x, P.y);

		// ����
		glColor3d(0.f, 0.f, 1.f);
		draw_Circle(P.x, P.y, 25, 16);

		// �ӵ��� �پ��� ���߱�
		if (abs(v_backup - length(V)) < 0.02)
		{
			V.x = 0; V.y = 0;
			A.x = 0; A.y = 0;
		}
		else
		{
			printf("%.2lf\n", length(V));
		}

		Sleep(60);
	}

	draw_Axis();	//��ǥ�� ���	

	glFlush();
	glutSwapBuffers();
}

/*
	GUI
	c: ���
	p: ���ڿ
	q�Ǵ�Q�Ǵ�Esc: ���α׷� ����
*/
void MyKeyboard(unsigned char KeyPressed, int X, int Y)
{
	switch (KeyPressed)
	{
	case 'c': CIRCLE_MODE *= -1; break;
	case 'p': PENDULUM_MODE *= -1; break;
	case 27: exit(1); break;
	}
}

int main(int argc, char** argv)
{
	//GUI
	printf("� ����\n");
	printf("c : ȸ�� �\n");
	printf("p : ���� �\n");
	printf("Exc: ���α׷� ����\n");

	//INIT
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("����� ���ڿ");

	glMatrixMode(GL_PROJECTION);	glLoadIdentity();

	glOrtho(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, -1000.0, 1.0);	// znear : -(-1.0) , zfar : -(1.0) 
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(MyKeyboard);
	glutMainLoop();

	return 0;
}