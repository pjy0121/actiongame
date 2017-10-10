// �����ó - http://dsiceeast.tistory.com/entry/opengl%EB%A1%9C-%EB%A7%8C%EB%93%A0-%EB%B0%B0%EA%B2%BD-%EA%B7%B8%EB%A6%BC

#pragma once

#include<windows.h>
#include<gl/GL.h>
#include<gl/glut.h>
#include<math.h>
#include<time.h>


void glInit(void) {				   // GL�ʱ�ȭ �Լ�.
	glClearColor(1.0, 1.0, 1.0, 0.0); // ��� ���.
	glViewport(0, 0, 300, 100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 400, 0, 300); // ��Ÿ�� 2D ���� ����.
	srand(time(NULL)); // �ð��� �̿��� �����Լ����� �ʱ�ȭ.
}

void drawStar() { //���׸��� �Լ�.

	int ran_X, ran_Y;
	float ran_R, ran_G, ran_B;

	glPointSize(1.0f);	// ��ũ��� 1������.
	glBegin(GL_POINTS);

	for (int i = 0; i<60; i++) { // �������� 60���� ���� �׸�.

		ran_R = (rand() % 51 + 50) / 100.0f;	// RGB ���� �������� ����.
		ran_G = (rand() % 51 + 50) / 100.0f;	// ���� ���� �ణ ��¦�� �� ���̰� �ϱ� ����
		ran_B = (rand() % 51 + 50) / 100.0f;   // ������ RGB�� 0.5 �̻��� ���� ������ ��.	

		ran_X = rand() % 401; //�� ��ġ ���� ������ ��� 2/3 ���� �ȿ��� ���� ����.
		ran_Y = (rand() % 201) + 100;

		glColor3f(ran_R, ran_G, ran_B);
		glVertex2f(ran_X, ran_Y);
	}

	glEnd();

}
void drawMeteor()
{
	for (int i = 0; i<15; ++i)
	{

		glPointSize(3.0f);
		glBegin(GL_POINTS);

		int ran_X, ran_Y;
		ran_X = rand() % 401; //�� ��ġ ���� ������ ��� 2/3 ���� �ȿ��� ���� ����.
		ran_Y = (rand() % 201) + 200;

		glVertex2f(ran_X, ran_Y);
		glEnd();

		/// ���� �ڿ� ���̴� �ܻ�.
		glBegin(GL_LINES);

		float ran_R, ran_G, ran_B;
		ran_R = (rand() % 51 + 50) / 100.0f;	// RGB ���� �������� ����.
		ran_G = (rand() % 51 + 50) / 100.0f;	// ���� ���� �ణ ��¦�� �� ���̰� �ϱ� ����
		ran_B = (rand() % 51 + 50) / 100.0f;   // ������ RGB�� 0.5 �̻��� ���� ������ ��.	

		int ran_Len = rand() % 20 + 5;
		glColor3f(ran_R, ran_G, ran_B);

		glVertex2f(ran_X, ran_Y);
		glVertex2f(ran_X + ran_Len, ran_Y + ran_Len);

		glEnd();
	}
}

/// sin �̿��� �� �׸���.
void drawMountian()
{

	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.4f, 0.0f);
	float f = 0.0f;

	const int xx = 10;
	const int yy = 10;
	const int high = 100;
	const int low = high / 2;
	while (f <= 360.0f)
	{
		glVertex2f(f*xx, sin(f) * yy + high);
		glVertex2f(f*xx, low);


		f += 0.1f;
	}

	glEnd();
}
/// ���� �ٴ� �׸��� ( �ĵ� )
void drawSea()
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.4f);
	float f = 0.0f;

	const int xx = 10;
	const int yy = 7;
	const int high = 10;
	const int low = 0;
	while (f <= 360.0f)
	{
		glVertex2f(f*xx, sin(f) * yy + high);
		glVertex2f(f*xx, low);

		glVertex2f(f*xx, cos(f) * yy + high);
		glVertex2f(f*xx, low);


		f += 0.1f;
	}

	glEnd();
}
/// ���� �ٴ� �׸���.
void drawSea2()
{
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.0f, 0.0f, 0.7f);

	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 70.0f);
	glVertex2f(400.0f, 0.0f);
	glVertex2f(400.0f, 70.0f);

	glEnd();
}

/// ��� �׸���.
void drawBackGround()
{

	glColor3f(1.0f, 0.4f, 0.2f);

	glBegin(GL_QUAD_STRIP);

	glVertex2f(0.0f, 0.0f);
	glVertex2f(400.0f, 0.0f);

	glColor3f(0.1f, 0.1f, 0.6f);

	glVertex2f(0.0f, 300.0f);
	glVertex2f(400.0f, 300.0f);

	glEnd();

}
/// ������
void drawPerfectMoon()
{
	glColor3f(1.0f, 1.0f, 0.3f);
	glBegin(GL_LINE_STRIP);

	const int MoonPosX = 300;
	const int MoonPosY = 200;
	for (int i = 0; i <= 360; ++i)
	{
		glVertex2f(cos(float(i)) * 20 + MoonPosX, sin(float(i)) * 20 + MoonPosY);
		glVertex2f(MoonPosX, MoonPosY);
	}
	glEnd();
}

void view_Display(void) {		// ���� �׸���.

	glClear(GL_COLOR_BUFFER_BIT);

	drawBackGround(); //��� �׸���.
	drawStar();	// �� �׸���.
	drawMeteor();// ���� �׸���.
	drawMountian(); // �� �׸���.
	drawSea2();
	drawSea();
	drawPerfectMoon();

	glFlush();

}