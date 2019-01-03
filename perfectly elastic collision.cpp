#include "collision.hpp"

void display(void)
{
	// �ٸ��� �׸���.
	glClearColor(0.08f, 0.35f, 0.16f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	draw_DAMA();
	
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if (i == j) continue;					// �ڽ��� �˻� �� ��

			t = getCollisionTime(Ball[i], Ball[j]); // �浹�ð��� ���ؿ´�. 

			if (t < 0) continue;					// �浹 �� �ϸ� ����
			
			else if (t <= dt)						// �浹�� ������ ���
			{
				vec2 u = Ball[i].v; vec2 v = Ball[j].v;
				vec2 dp = Ball[j].p - Ball[i].p;	// dp�� �� ��ü������ �Ÿ�����

				// ��ȭ�� ���͵��� ����
				GLdouble length_u = fastLength(u);
				GLdouble length_v = fastLength(v);
				GLdouble length_dp = fastLength(dp);

				// �߽ɰŸ� ���Ϳ� �� �ӵ������� ���̰�
				GLdouble u_cos = dot(u, dp) / (length_u * length_dp);
				GLdouble v_cos = dot(v, -dp) / (length_v * length_dp);

				// �浹���� u,v�� x,y ������ �����ϰ� ���� ���͵��� x������ ��ȯ��
				vec2 ux = fastNormalize(dp) * mat2(length_u * u_cos);
				vec2 uy = u - ux;
				vec2 vx = fastNormalize(-dp) * mat2(length_v * v_cos);
				vec2 vy = v - vx;

				// �ٲ� �������� ���ο� �ӵ����͸� �����.
				Ball[i].v = (vx + uy);
				Ball[j].v = (ux + vy);

				// �浹�˻�
				if ((i == Y && j == W) || (i == W && j == Y))
				{
					printf("\t!������ ���� �ǵ�� �г�Ƽ�� �ްԵ˴ϴ�!\n");
					Ball[i].isHit = true; Ball[j].isHit = true;
				}
				else if ((i == Y && j != W) || (i == W && j != Y))
				{
					if (!Ball[j].isHit)
						j == R1 ? printf("\t1�� �������� ������ϴ�!\n") : printf("\t2�� �������� ������ϴ�!\n");
					
					Ball[j].isHit = true;
				}
			}
		}
	}

	// ������ �浹 : if���� �� ù���� �����۾�
	// ������ if���� ��� Ƚ��
	for (int i = 0; i < N; i++)
	{
		if (Ball[i].p.x < -200 + RADIUS || Ball[i].p.x > 200 - RADIUS)
		{
			Ball[i].p += -fastNormalize(Ball[i].v)*mat2(fastLength(Ball[i].v)*dt);
			Ball[i].v.x *= -1;
			Ball[i].v *= 0.65;

			if (i == turn)
			{
				cusion[turn]++;
				i == Y ? printf("\tY: ��� %d\n", cusion[Y]) : printf("\tW: ��� %d\n", cusion[W]);
			}
		}
		else if (Ball[i].p.y < -400 + RADIUS || Ball[i].p.y > 400 - RADIUS)
		{
			Ball[i].p += -fastNormalize(Ball[i].v)*mat2(fastLength(Ball[i].v)*dt);
			Ball[i].v.y *= -1;
			Ball[i].v *= 0.65;

			if (i == turn)
			{
				cusion[turn]++;
				i == Y ? printf("\tY: ��� %d\n", cusion[Y]) : printf("\tW: ��� %d\n", cusion[W]);
			}
		}
	}

	// ������ ���
	for (int i = 0; i < N; ++i) Ball[i].a = -fastNormalize(Ball[i].v) * mat2(friction);

	// �� ��ġ ������Ʈ
	updatePosition(dt);

	// ���̵弱 �׸��� �� ��Ȳ�� ���
	if (isStop)
	{
		if (action == DRAG)
		{
			vec2 guid = Ball[turn].p + mat2(1000)*fastNormalize(Ball[turn].p - vec2(mouse_x, mouse_y));	// ������

			glColor3d(0, 0, 1);
			draw_Line(Ball[turn].p.x, Ball[turn].p.y, mouse_x, mouse_y);
			glColor3d(0.5, 0.5, 0.5);
			draw_Line(Ball[turn].p.x, Ball[turn].p.y, guid.x, guid.y);
		}
		else if (action == RELEASED)
		{
			// ���콺�� ���� �� ���� �ӵ��� �ο��Ѵ�.
			Ball[turn].v = (Ball[turn].p - vec2(mouse_x, mouse_y)) * mat2(0.6);
			showInfo = true;
			action = NOTHING;

			for (int i = 0; i < 2; i++) cusion[i] = 0;
			promptClear();
			promptClear();
		}

		else if (showInfo)	// ���� �ݺ��� �������� ��ġ
		{
			showInfo = false;
			if (Ball[Y].isHit && Ball[W].isHit)   // ������ ���� ���
			{
				// �г�Ƽ �ش���
				score[turn] += 10;

				// ���� �ٲٰ�
				changeTurn();
				// ���� ��� �� �浹���θ� �ʱ�ȭ�Ѵ�.
				initHitCount();
			}

			// ������ 2�� �� ������ ���
			else if (Ball[R1].isHit && Ball[R2].isHit)
			{
				score[turn] -= 10;

				if (score[turn] < 0 && cusion[turn] == cusionUnit)   // ����� Ƚ���� ������ �� �ִ�.
				{
					turn == Y ? printf("\n����� �¸�!!\n") : printf("\n�Ͼ�� �¸�!!\n");
					exit(1);
				}
				else if (score[turn] < 0 && cusion[turn] != 0) score[turn] = 0;

				turn == Y ? printf("\tY: ���̽���!\n") : printf("\tW: ���̽���!\n");
				initHitCount();
			}

			// �ϳ��� �� ������ ���
			else if (Ball[R1].isHit == false && Ball[R2].isHit == false)
			{
				turn == Y ? printf("\tY: fails to hit!\n") : printf("\tW: fail to hit!\n");

				score[turn] += 10;
				changeTurn();
				initHitCount();
			}

			// �� �߿� �ϳ��� ������ ���
			else if ((Ball[R1].isHit && !Ball[R2].isHit) || (!Ball[R1].isHit && Ball[R2].isHit))
			{
				changeTurn();
				initHitCount();
			}
			
			if (score[turn] <= 0)
				turn == Y ? printf("[Score] Y: Ŭ���� ����!\tW: %d\n", score[W]) 
						  : printf("[Score] Y: %d\tW: Ŭ���� ����!\n", score[Y]);

			else showScore();
		}
	}

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char ** argv)
{
	initBalls();
	makeContext(argc, argv, "4 Billiards");
	glutDisplayFunc(display);
	glutKeyboardFunc(MyKeyboard);
	glutSpecialFunc(MySpecialKeys);
	glutMouseFunc(MyMouse);
	glutMotionFunc(MyMotion);
	glutIdleFunc(display);

	// GUI
	printf("������� �籸���\n");
	printf("hŰ�� ���� ������ �� �� �ֽ��ϴ�.\n");
	printf("\n������� �����Դϴ�.\n");
	printf("[Score] Y: %d\t W: %d\n", score[Y], score[W]);
	glutMainLoop();

	return 0;
}