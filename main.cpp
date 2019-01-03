#include "inc.h"

void display(void)
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);
	glColor3d(1.f, 0.f, 0.f);
	for (int y = -100; y <= 100; y++)
	{
		for (int x = -100; x <= 100; x++)
		{
			glVertex2d(x, y);
		}
	}
	glEnd();
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("사각형 출력");

	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glOrtho(-150, 150, -150, 150, -1, 1);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}