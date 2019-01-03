#include "collision.hpp"

GLdouble dt = 0.1;
GLdouble radius = 25.f;

struct RigidBody a, b;
struct RigidBody left, right;

void display(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (distance(a.p, b.p) <= radius * 2)
	{
		double temp = a.v.x;
		a.v.x = b.v.x;
		b.v.x = temp;
	}
	else if (distance(left.p, a.p) <= radius)
	{
		a.v = -a.v;
	}
	else if (distance(right.p, b.p) <= radius)
	{
		b.v = -b.v;
	}

	a.p += a.v * mat4(dt);
	b.p += b.v * mat4(dt);

	glColor3d(0.5, 1, 0);
	draw_Circle(a.p.x, a.p.y, radius, 16);
	glColor3d(0, 0.5, 1);
	draw_Circle(b.p.x, b.p.y, radius, 16);

	glColor3d(0.f, 0.f, 0.f);
	draw_Line(-400, -25, 400, -25);
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	makeContext(argc, argv, "�������� ���� �浹");

	//��ü ��ġ �ʱ�ȭ
	a.p = vec4(-300, 0, 0, 1);
	b.p = vec4(300, 0, 0, 1);
	//��ü�� �ʱ�ӵ�
	a.v = vec4(50, 0, 0, 0);
	b.v = vec4(-15, 0, 0, 0);
	//�� ��ġ �ʱ�ȭ
	left.p = vec4(-SCREEN_WIDTH / 2, 0, 0, 0);
	right.p = vec4(SCREEN_WIDTH / 2, 0, 0, 0);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}