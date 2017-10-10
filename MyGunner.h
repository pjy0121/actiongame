#pragma once
#include "MyHuman.h"

class MyGunner : public MyHuman
{
public:
	vec3 gunPos_;	// �Ѿ��� ��ġ
	bool fadeaway_;	// Ư����ų fadeaway�� �ߵ� ���� ����

	// ������
	MyGunner(bool _player) :fadeaway_(true)
	{
		gunPos_ = center_;
		player_ = _player;

		initPosition();	// ���� ��ġ ����
	}

	// ĳ������ ���� �׸���(@Overriding)
	void drawLeftArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.065);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.translate(0.05, -0.06);
		drawGun(time, RGBColors::blue);
		my_canvas.endTransformation();
	}

	// ĳ������ ������ �׸���(@Overriding)
	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.05);
		my_canvas.rotate(-15);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

	// �� �׸���
	void drawGun(const float & time, const vec3 & color)
	{
		my_canvas.translate(0.075, 0.1);
		my_canvas.drawFilledBox(color, 0.05, 0.4);
		my_canvas.translate(-0.08, 0.0);
		my_canvas.drawFilledBox(color, 0.05, 0.4);
		my_canvas.translate(0.2, 0.1);
		my_canvas.drawFilledBox(color, 0.3, 0.15);
		my_canvas.translate(-0.1, -0.05);
		my_canvas.drawFilledBox(color, 0.3, 0.15);

		gunPos_.x = center_.x + 0.17;
		gunPos_.y = center_.y - 0.05;
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
			drawLeftArm(time, RGBColors::black);
			drawRightArm(time, RGBColors::black);
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

		// draw gun
		my_canvas.translate(0.03, -0.1);
		my_canvas.scale(0.4, 0.1);
		drawGun(time, RGBColors::blue);
		my_canvas.scale(2.5, 10);

		// draw legs
		my_canvas.translate(-0.12, 0.11);
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

	// �ٰŸ� ���� ����Ʈ
	void close_shot()
	{
		my_canvas.beginTransformation();
		if (!player_)	// gunner�� 2P�� ���
		{
			my_canvas.scale(-1.0, 1.0);	// �¿��Ī
		}
		my_canvas.translate(gunPos_.x, gunPos_.y);
		my_canvas.scale(0.4, 0.1);
		my_canvas.translate(-0.02, 0.0);
		my_canvas.drawFilledBox(RGBColors::red, 0.4, 0.05);
		my_canvas.rotate(60);
		my_canvas.translate(0.2, 0.0);
		my_canvas.drawFilledBox(RGBColors::red, 0.25, 0.05);
		my_canvas.translate(-0.2, 0.0);
		my_canvas.rotate(-30);
		my_canvas.translate(0.1, 0.0);
		my_canvas.drawFilledBox(RGBColors::red, 0.15, 0.05);
		my_canvas.translate(-0.1, 0.0);
		my_canvas.rotate(-60);
		my_canvas.translate(0.1, 0.0);
		my_canvas.drawFilledBox(RGBColors::red, 0.15, 0.05);
		my_canvas.translate(-0.1, 0.0);
		my_canvas.rotate(-30);
		my_canvas.translate(0.2, 0.0);
		my_canvas.drawFilledBox(RGBColors::red, 0.25, 0.05);
		my_canvas.translate(-0.2, 0.0);
		if (!player_)	// gunner�� 2P�� ���
		{
			my_canvas.scale(-1.0, 1.0);	// �¿��Ī
		}
		my_canvas.endTransformation();
	}

	// �ڷ� ������ �������� - 7�ʸ��� �ѹ� �� �� �ִ� gunner�� Ư�� ��ų
	void fadeaway(float& time)
	{
		if (fadeaway_)		// fadeaway�� �� �� ���� ��
		{
			special_effect(time, RGBColors::blue);	// fadeaway�� Ȱ������������ �˷��ִ� ����� ����Ʈ
			if (player_)	// gunner�� 1P�� ��
			{
				if (my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_W))		// �������� fadeaway
				{
					fade_to(-0.4, 0.4); fadeaway_ = false;
				}

				if (my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_S))		// �Ʒ������� fadeaway
				{
					fade_to(-0.4, -0.4); fadeaway_ = false;
				}
			}
			else	// gunner�� 2P�� ��
			{
				if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_UP))		// �������� fadeaway
				{
					fade_to(-0.4, 0.4); fadeaway_ = false;
				}

				if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_DOWN))		// �Ʒ������� fadeaway
				{
					fade_to(-0.4, -0.4); fadeaway_ = false;
				}
			}
		}		// �� fadeaway�� ��Ÿ��(���� ��� �ð�)�� 7�ʷ� �α� ����
		else if ((int)time % 7 == 0 && 0 < time - (int)time && time - (int)time < 0.1) fadeaway_ = true;
	}

	// fadeaway �Ǵ� ��ġ
	void fade_to(double x, double y)
	{
		center_.x += x;
		center_.y += y;
	}
};