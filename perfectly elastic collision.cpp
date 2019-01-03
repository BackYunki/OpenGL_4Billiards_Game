#include "collision.hpp"

void display(void)
{
	// 다마를 그린다.
	glClearColor(0.08f, 0.35f, 0.16f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	draw_DAMA();
	
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if (i == j) continue;					// 자신은 검사 안 함

			t = getCollisionTime(Ball[i], Ball[j]); // 충돌시간을 구해온다. 

			if (t < 0) continue;					// 충돌 안 하면 음수
			
			else if (t <= dt)						// 충돌이 가능한 경우
			{
				vec2 u = Ball[i].v; vec2 v = Ball[j].v;
				vec2 dp = Ball[j].p - Ball[i].p;	// dp는 두 물체사이의 거리벡터

				// 변화량 벡터들의 길이
				GLdouble length_u = fastLength(u);
				GLdouble length_v = fastLength(v);
				GLdouble length_dp = fastLength(dp);

				// 중심거리 벡터와 각 속도벡터의 사이각
				GLdouble u_cos = dot(u, dp) / (length_u * length_dp);
				GLdouble v_cos = dot(v, -dp) / (length_v * length_dp);

				// 충돌시의 u,v의 x,y 성분을 분해하고 기존 벡터들의 x성분을 교환함
				vec2 ux = fastNormalize(dp) * mat2(length_u * u_cos);
				vec2 uy = u - ux;
				vec2 vx = fastNormalize(-dp) * mat2(length_v * v_cos);
				vec2 vy = v - vx;

				// 바뀐 성분으로 새로운 속도벡터를 만든다.
				Ball[i].v = (vx + uy);
				Ball[j].v = (ux + vy);

				// 충돌검사
				if ((i == Y && j == W) || (i == W && j == Y))
				{
					printf("\t!상대방의 공을 건드려 패널티를 받게됩니다!\n");
					Ball[i].isHit = true; Ball[j].isHit = true;
				}
				else if ((i == Y && j != W) || (i == W && j != Y))
				{
					if (!Ball[j].isHit)
						j == R1 ? printf("\t1번 목적구를 맞췄습니다!\n") : printf("\t2번 목적구를 맞췄습니다!\n");
					
					Ball[j].isHit = true;
				}
			}
		}
	}

	// 벽과의 충돌 : if문의 각 첫줄은 보정작업
	// 마지막 if문은 쿠션 횟수
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
				i == Y ? printf("\tY: 쿠션 %d\n", cusion[Y]) : printf("\tW: 쿠션 %d\n", cusion[W]);
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
				i == Y ? printf("\tY: 쿠션 %d\n", cusion[Y]) : printf("\tW: 쿠션 %d\n", cusion[W]);
			}
		}
	}

	// 마찰력 계산
	for (int i = 0; i < N; ++i) Ball[i].a = -fastNormalize(Ball[i].v) * mat2(friction);

	// 공 위치 업데이트
	updatePosition(dt);

	// 가이드선 그리기 및 상황판 출력
	if (isStop)
	{
		if (action == DRAG)
		{
			vec2 guid = Ball[turn].p + mat2(1000)*fastNormalize(Ball[turn].p - vec2(mouse_x, mouse_y));	// 보조선

			glColor3d(0, 0, 1);
			draw_Line(Ball[turn].p.x, Ball[turn].p.y, mouse_x, mouse_y);
			glColor3d(0.5, 0.5, 0.5);
			draw_Line(Ball[turn].p.x, Ball[turn].p.y, guid.x, guid.y);
		}
		else if (action == RELEASED)
		{
			// 마우스를 떼면 그 공에 속도를 부여한다.
			Ball[turn].v = (Ball[turn].p - vec2(mouse_x, mouse_y)) * mat2(0.6);
			showInfo = true;
			action = NOTHING;

			for (int i = 0; i < 2; i++) cusion[i] = 0;
			promptClear();
			promptClear();
		}

		else if (showInfo)	// 무한 반복을 막기위한 장치
		{
			showInfo = false;
			if (Ball[Y].isHit && Ball[W].isHit)   // 적구를 쳤을 경우
			{
				// 패널티 준다음
				score[turn] += 10;

				// 턴을 바꾸고
				changeTurn();
				// 점수 출력 후 충돌여부를 초기화한다.
				initHitCount();
			}

			// 목적구 2개 다 맞췄을 경우
			else if (Ball[R1].isHit && Ball[R2].isHit)
			{
				score[turn] -= 10;

				if (score[turn] < 0 && cusion[turn] == cusionUnit)   // 쿠션의 횟수를 설정할 수 있다.
				{
					turn == Y ? printf("\n노란공 승리!!\n") : printf("\n하얀공 승리!!\n");
					exit(1);
				}
				else if (score[turn] < 0 && cusion[turn] != 0) score[turn] = 0;

				turn == Y ? printf("\tY: 나이스샷!\n") : printf("\tW: 나이스샷!\n");
				initHitCount();
			}

			// 하나도 못 맞췄을 경우
			else if (Ball[R1].isHit == false && Ball[R2].isHit == false)
			{
				turn == Y ? printf("\tY: fails to hit!\n") : printf("\tW: fail to hit!\n");

				score[turn] += 10;
				changeTurn();
				initHitCount();
			}

			// 둘 중에 하나만 맞췄을 경우
			else if ((Ball[R1].isHit && !Ball[R2].isHit) || (!Ball[R1].isHit && Ball[R2].isHit))
			{
				changeTurn();
				initHitCount();
			}
			
			if (score[turn] <= 0)
				turn == Y ? printf("[Score] Y: 클리어 도전!\tW: %d\n", score[W]) 
						  : printf("[Score] Y: %d\tW: 클리어 도전!\n", score[Y]);

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
	printf("※윤기네 당구장※\n");
	printf("h키를 눌러 도움말을 볼 수 있습니다.\n");
	printf("\n노랑공의 차례입니다.\n");
	printf("[Score] Y: %d\t W: %d\n", score[Y], score[W]);
	glutMainLoop();

	return 0;
}