#pragma once
#include "MyHuman.h"

class MyWarrior : public MyHuman
{
public:
	vec3 attackedPos_;	// �ǰ� ��ġ 
	vec3 swordPos_;		// ���� ��ġ
	bool dash_;		// Ư����ų dash�� �ߵ� ���� ����

					// ������
	MyWarrior(bool _player) : dash_(true)
	{
		player_ = _player;
		speed_ = 0.0009 * PERFORMANCE;
		swordPos_ = center_;

		initPosition();	// ���� ��ġ ����
	}


	// ĳ������ ���� �׸���(@overriding) 
	void drawLeftArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.065);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.translate(0.05, -0.06);
		drawSword(time, RGBColors::red);
		my_canvas.endTransformation();
	}

	// ĳ������ ������ �׸���(@overriding) 
	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.05);
		my_canvas.rotate(20);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

	// Į �׸���
	void drawSword(const float & time, const vec3 & color)
	{
		my_canvas.translate(0.045, 0.8);
		my_canvas.drawFilledBox(color, 0.05, 1.6);
		my_canvas.translate(0.0, 0.95);
		my_canvas.drawFilledTriangle(color, 0.05, 0.3);
		my_canvas.translate(0.0, -1.3);
		my_canvas.drawFilledBox(color, 0.1, 0.2);
		swordPos_.x = -10.0;	// ���� ������ ��� �ִ� ������ ���� �ǰݵ��� �ʵ��� �ϱ� ����
		swordPos_.y = -10.0;
	}

	// ���� ����(����)
	void actionLeft_1(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(-90 + pow(sin(time*7.0* PERFORMANCE), 2) * 105.0);			// animation!
		my_canvas.translate(0.03, -0.025);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.18, 0.18);
		my_canvas.translate(0.02, 0.0);
		drawSword(time, RGBColors::red);
		my_canvas.scale(2.5, 10);
		my_canvas.endTransformation();

		double x = center_.x + 0.06;
		double y = center_.y + 0.12;
		swordPos_.x = x + pow(0.35*sin(time*8.0* PERFORMANCE + 90), 2);	// ������ �� ��ǥ ��ȭ
		swordPos_.y = y + pow(0.5*sin(time*8.0* PERFORMANCE), 2) - 0.22;
		// printf("%lf %lf\n", swordPos_.x, swordPos_.y);
	}

	// ������ ����(����)
	void actionRight_1(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(-90 + pow(sin(time*7.0* PERFORMANCE), 2) * 105.0);			// animation!
		my_canvas.translate(0.04, -0.02);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.18, 0.18);
		my_canvas.endTransformation();
	}

	// ���� ���
	void actionLeft_2(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.05);
		my_canvas.drawFilledBox(color, 0.09, 0.02);
		my_canvas.translate(0.0, 0.01);
		my_canvas.rotate(-90);
		my_canvas.scale(0.4, 0.1);
		my_canvas.translate(0.0, 0.3);
		drawSword(time, RGBColors::red);
		my_canvas.rotate(90);
		my_canvas.endTransformation();

		swordPos_.x = center_.x + 0.25;
		swordPos_.y = center_.y - 0.02;
	}

	// ������ ���
	void actionRight_2(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.065);
		my_canvas.drawFilledBox(color, 0.09, 0.02);
		my_canvas.endTransformation();
	}

	// ĳ���� ��ü �׸���(@Overriding)
	void draw(const float& time)
	{
		if (!player_)	// 1P�� �ƴ϶�� �¿����
		{
			my_canvas.scale(-1.0, 1.0);
		}
		drawPlayerNum();

		if (!dead_)
		{
			drawHead(RGBColors::black);
			drawBody(RGBColors::black);
			if (player_)
			{
				// ���� �׸���
				if (my_canvas.isKeyPressed(GLFW_KEY_R)) actionLeft_1(time, RGBColors::black);
				else if (my_canvas.isKeyPressed(GLFW_KEY_T)) actionLeft_2(time, RGBColors::black);
				else drawLeftArm(time, RGBColors::black);

				// ������ �׸���
				if (my_canvas.isKeyPressed(GLFW_KEY_R)) actionRight_1(time, RGBColors::black);
				else if (my_canvas.isKeyPressed(GLFW_KEY_T)) actionRight_2(time, RGBColors::black);
				else drawRightArm(time, RGBColors::black);
			}
			else
			{
				// ���� �׸���
				if (my_canvas.isKeyPressed(GLFW_KEY_K)) actionLeft_1(time, RGBColors::black);
				else if (my_canvas.isKeyPressed(GLFW_KEY_L)) actionLeft_2(time, RGBColors::black);
				else drawLeftArm(time, RGBColors::black);

				// ������ �׸���
				if (my_canvas.isKeyPressed(GLFW_KEY_K)) actionRight_1(time, RGBColors::black);
				else if (my_canvas.isKeyPressed(GLFW_KEY_L)) actionRight_2(time, RGBColors::black);
				else drawRightArm(time, RGBColors::black);
			}
			drawLeftLeg(time, RGBColors::black);
			drawRightLeg(time, RGBColors::black);
		}
		else dead(time);

		if (!player_)	// 1P�� �ƴ϶�� �¿����
		{
			my_canvas.scale(-1.0, 1.0);
		}
	}

	// �׾��� ���� ��� �׸���
	void dead(const float& time)
	{
		hp_ = 0;
		// draw head
		drawHead(RGBColors::black);

		// draw body
		my_canvas.rotate(-70);
		drawBody(RGBColors::black);

		// draw arms
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x - 0.04, center_.y - 0.05);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.07);
		my_canvas.translate(-0.025, -0.015);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.07);

		// draw sword
		my_canvas.translate(0.03, -0.1);
		my_canvas.rotate(90);
		my_canvas.scale(0.4, 0.1);
		drawSword(time, RGBColors::red);
		my_canvas.scale(2.5, 10);
		my_canvas.rotate(-90);

		// draw legs
		my_canvas.translate(-0.04, 0.1);
		my_canvas.drawFilledCircle(RGBColors::black, 0.02, 100);
		my_canvas.translate(0.02, -0.01);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.05);
		my_canvas.translate(-0.015, -0.03);
		my_canvas.drawFilledBox(RGBColors::black, 0.05, 0.02);
		my_canvas.translate(-0.015, 0.015);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.05);
		my_canvas.translate(-0.015, -0.03);
		my_canvas.drawFilledBox(RGBColors::black, 0.05, 0.02);
		my_canvas.endTransformation();
	}

	// ������ ���� - 5�ʸ��� �ѹ� �� �� �ִ� �������� Ư�� ��ų
	void dash(float& time)
	{
		if (dash_)		// dash�� �� �� ���� ��
		{
			special_effect(time, RGBColors::red);	// dash�� Ȱ������������ �˷��ִ� ����� ����Ʈ

			if (player_)	// warrior�� 1P�� ��
			{	// dash�� ����Ű ���ۿ� ���� ������ �޶���
				if (my_canvas.isKeyPressed(GLFW_KEY_A) && my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
				{
					dash_to(-0.2, 0.0); dash_ = false;
				}
				else if (my_canvas.isKeyPressed(GLFW_KEY_D) && my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
				{
					dash_to(0.2, 0.0); dash_ = false;
				}
				else if (my_canvas.isKeyPressed(GLFW_KEY_W) && my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
				{
					dash_to(0.0, 0.2); dash_ = false;
				}
				else if (my_canvas.isKeyPressed(GLFW_KEY_S) && my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
				{
					dash_to(0.0, -0.2); dash_ = false;
				}
			}
			else	// warrior�� 2P�� ��
			{
				if (my_canvas.isKeyPressed(GLFW_KEY_LEFT) && my_canvas.isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
				{
					dash_to(0.2, 0.0); dash_ = false;
				}
				else if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT) && my_canvas.isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
				{
					dash_to(-0.2, 0.0); dash_ = false;
				}
				else if (my_canvas.isKeyPressed(GLFW_KEY_UP) && my_canvas.isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
				{
					dash_to(0.0, 0.2); dash_ = false;
				}
				else if (my_canvas.isKeyPressed(GLFW_KEY_DOWN) && my_canvas.isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
				{
					dash_to(0.0, -0.2); dash_ = false;
				}
			}
		}		// �� dash�� ��Ÿ��(���� ��� �ð�)�� 5�ʷ� �α� ����
		else if ((int)time % 5 == 0 && 0 < time - (int)time && time - (int)time < 0.01) dash_ = true;
	}

	// dash�Ǵ� ��ġ
	void dash_to(double x, double y)
	{
		center_.x += x;
		center_.y += y;
	}

	// Ÿ�ݴ��� ��ġ�� ǥ���ϴ� �Լ� �Լ�
	void attacked_position(vec3* _attackedPos)
	{
		attackedPos_ = *_attackedPos;
		my_canvas.beginTransformation();
		my_canvas.translate(_attackedPos->x, _attackedPos->y);
		my_canvas.drawFilledCircle(RGBColors::red, 0.01, 10);
		my_canvas.endTransformation();
	}
};