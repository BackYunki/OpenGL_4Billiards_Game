#define _CRT_SECURE_NO_WARNINGS				
#define PI 3.141592							//원주율
#define DTOR (PI/180)						//각도->라디안

#include <glut.h>						
#include <stdio.h>							
#include <math.h>							
#include <Windows.h>						
#include <glm.hpp>							//OpenGL 벡터 및 행렬 연산 라이브버리
using namespace glm;

const GLdouble SCREEN_WIDTH = 800.f;
const GLdouble SCREEN_HEIGHT = 800.f;

//-------Circle--------
vec4 c = vec4(200, 0, 0, 1);				//원의중심
vec4 o = vec4(0, 0, 0, 1);					//회전의 중심
vec4 r = c - o;								//r(t)벡터
vec4 a;										//가속도
vec4 v = vec4(0, 0.2, 0, 0);				//초기속도

GLdouble R = sqrt(dot(r, r));				//회전 반지름
GLfloat dt = 1.f;							//애니메이션 시간 단위
GLfloat dir_dt = 1.f;
//------Pendulum-------
vec4 C = vec4(0, SCREEN_HEIGHT / 2, 0, 1);	//점C
vec4 P = vec4(300, 0, 0, 1);				//점P

vec4 g = vec4(0, -100, 0, 0);				//중력가속도 g
vec4 A = vec4(0, 0, 0, 0);					//가속도의 변화
vec4 V = vec4(0, 0, 0, 0);					//속도의 변화

vec4 in = C - P;							//점 P와 C를 잇는 벡터
GLdouble R2 = sqrt(dot(in, in));			//끈의 길이
GLdouble v_backup;

int CIRCLE_MODE = -1;						//원운동 스위치
int PENDULUM_MODE = -1;						//진자운동 스위치

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

void display(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// 원운동
	if (CIRCLE_MODE == 1)
	{
		glColor3d(0.f, 0.f, 1.f);
		draw_Circle(r.x, r.y, 100, 32);
		a = mat4(-dot(v, v) / R) * normalize(r);
		v += a;
		r += mat4(dt) * v;

		r = mat4(R) * normalize(r);
	}
	// 진자운동
	else if (PENDULUM_MODE == 1)
	{
		v_backup = length(V);

		in = C - P;
		vec4 normalized_in = normalize(in);
		vec4 crossVector = vec4(cross(vec3(0, -1, 0), -vec3(normalized_in.x, normalized_in.y, 0)), 0);
		A = mat4(normalize(crossVector).z * length(g)) * length(crossVector) * normalize(vec4(cross(vec3(0, 0, 1), vec3(in.x, in.y, 0)), 0));
		V += A;
		P += V * mat4(dt);

		P = C - mat4(R2) * normalize(C - P);	//보정

		// 천장중심
		glColor3d(0.f, 0.f, 0.f);
		draw_Circle(0, SCREEN_HEIGHT / 2, 25, 16);

		// 선
		glColor3f(0.f, 0.f, 0.f);
		draw_Line(C.x, C.y, P.x, P.y);

		// 진자
		glColor3d(0.f, 0.f, 1.f);
		draw_Circle(P.x, P.y, 25, 16);

		// 속도가 줄어들면 멈추기
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

	draw_Axis();	//좌표계 출력	

	glFlush();
	glutSwapBuffers();
}

/*
	GUI
	c: 원운동
	p: 진자운동
	q또는Q또는Esc: 프로그램 종료
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
	printf("운동 선택\n");
	printf("c : 회전 운동\n");
	printf("p : 진자 운동\n");
	printf("Exc: 프로그램 종료\n");

	//INIT
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("원운동과 진자운동");

	glMatrixMode(GL_PROJECTION);	glLoadIdentity();

	glOrtho(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, -1000.0, 1.0);	// znear : -(-1.0) , zfar : -(1.0) 
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(MyKeyboard);
	glutMainLoop();

	return 0;
}