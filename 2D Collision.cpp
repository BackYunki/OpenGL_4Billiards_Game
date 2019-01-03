#define MOK1 0
#define MOK2 1
#define SOO1 2
#define SOO2 3

#define NUM 4
#define RADIUS 13.f
#include "collision.hpp"

// 당구공 객체
struct Billiard ball[4];

// 당구다이 모서리 좌표
vec2 table[NUM] = {vec2(-200,400),vec2(200,400), vec2(200,-400), vec2(-200,-400)};
// 목적구 수구 색깔
vec3 color_mok = vec3(170 / 255.f, 15 / 255.f, 20 / 255.f);
vec3 color_soo = vec3(230 / 255.f, 220 / 255.f, 150 / 255.f);
// 중력 벡터
vec4 g = vec4(0, -30.8, 0, 0);

const GLdouble dt = 0.1;

void display(void)
{
	// 당구다이
	glClearColor(20 / 255.f, 90 / 255.f, 40 / 255.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(10.f);
	glColor3d(100 / 255.f, 60 / 255.f, 40 / 255.f);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 4; i++)
			glVertex2d(table[i].x, table[i].y);
		glVertex2d(table[0].x, table[0].y);
	glEnd();

	for (int i = 0; i < NUM; i++)
	{
		if(ball[i].pos.y > -400 + RADIUS)
			ball[i].v += g * mat4(dt);
		ball[i].pos += ball[i].v * mat4(dt);
	
		// 좌우 벽과의 충돌 : x성분 뒤집기
		if (ball[i].pos.x >= 200 - RADIUS || ball[i].pos.x <= -200 + RADIUS)
		{
			ball[i].v.x = -ball[i].v.x;
		}
		// 상하 벽과의 충돌 : y성분 뒤집기
		else if (ball[i].pos.y >= 400 - RADIUS || ball[i].pos.y <= -400 + RADIUS)
		{
			ball[i].v.y = -ball[i].v.y;
		}
		// 공끼리 충돌
		for (int j = 0; j < NUM; j++)
		{
			if (j == i) continue;
			if (fastDistance(ball[i].pos, ball[j].pos) < 2 * RADIUS)
			{
				GLdouble temp = ball[i].v.x;
				ball[i].v.x = ball[j].v.x;
				ball[j].v.x = temp;
			}
		}
	}

	// 물체 위치 갱신
	for (int i = 0; i < NUM; i++)
	{
		glColor3d(ball[i].color.r, ball[i].color.g, ball[i].color.b);
		draw_Circle(ball[i].pos.x, ball[i].pos.y, RADIUS, 16);
	}

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	// 목적구와 수구의 위치 및 색깔 초기화
	ball[MOK1].color = color_mok; ball[MOK2].color = color_mok;
	ball[SOO1].color = color_soo; ball[SOO2].color = color_soo;
	ball[MOK1].pos = vec4(0, 200, 0, 0); ball[MOK2].pos = vec4(0, -200, 0, 0);
	ball[SOO1].pos = vec4(0, 300, 0, 0); ball[SOO2].pos = vec4(50, -200, 0, 0);

	ball[SOO2].v = vec4(100, 100, 0, 0);

	// GL초기화
	makeContext(argc, argv, "마찰력이 없는 2차원 충돌");

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}