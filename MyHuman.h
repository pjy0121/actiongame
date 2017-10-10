#pragma once
#include "DigitalCanvas2D.h"
#include "RGBColors.h"
#include <iostream>
#include <cmath>
#include <cstring>

#define PERFORMANCE 2.0		// ��濡 ���� �������� ������ �߻��ؼ� �ӵ� ����

using namespace std;
using namespace glm;

DigitalCanvas2D my_canvas("This is my digital canvas!", 1024, 768); // Canvas : (-1.0, -1.0) x (1.0, 1.0)

class MyHuman
{
public:
	bool defense_;	// ����� On true, Off : false
	double hp_;		// ĳ������ ���� hp��
	double speed_;	// ĳ������ ���� ���ǵ�
	bool player_;	// 1P�� true, 2P�� false
	bool dead_;	// �׾����� true, ��������� false

	vec3 center_;
	vec3 direction_;


	// ������
	MyHuman() : hp_(100), defense_(false), speed_(0.0006 * PERFORMANCE), dead_(false)
	{}

	// ĳ������ �Ӹ� �׸���
	void drawHead(const vec3& color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledCircle(RGBColors::black, 0.04);
	}

	// ĳ������ ���� �׸���
	void drawBody(const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(0.0, -0.04);
		my_canvas.drawFilledBox(RGBColors::black, 0.04, 0.16);
		my_canvas.endTransformation();
	}

	// ĳ������ ���� �׸���
	void drawLeftArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.065);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.translate(0.05, -0.06);
		my_canvas.endTransformation();
	}

	// ĳ������ ������ �׸���
	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.05);
		my_canvas.rotate(20);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

	// ĳ������ �޴ٸ� �׸���
	void drawLeftLeg(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x - 0.01, center_.y - 0.12);
		my_canvas.rotate(-30 + sin(time*7.0* PERFORMANCE) * 60);
		my_canvas.drawFilledBox(RGBColors::black, 0.021, 0.03);
		my_canvas.translate(0.0, -0.02);
		my_canvas.drawFilledBox(RGBColors::black, 0.015, 0.07);
		my_canvas.translate(0.0, -0.01);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.01);
		my_canvas.translate(0.005, -0.02);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.01);
		my_canvas.endTransformation();
	}

	// ĳ������ �����ٸ� �׸���
	void drawRightLeg(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.01, center_.y - 0.12);
		my_canvas.rotate(-30 + sin(time*7.0* PERFORMANCE) * -60);
		my_canvas.drawFilledBox(RGBColors::black, 0.021, 0.03);
		my_canvas.translate(0.0, -0.02);
		my_canvas.drawFilledBox(RGBColors::black, 0.015, 0.07);
		my_canvas.translate(0.0, -0.01);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.01);
		my_canvas.translate(0.005, -0.02);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.01);
		my_canvas.endTransformation();
	}

	// ĳ���� ��ü �׸���
	void draw()
	{
		if (player_)
		{
			my_canvas.beginTransformation();
			my_canvas.translate(center_.x, center_.y);
			my_canvas.drawFilledBox(RGBColors::green, 0.1, 0.1);
			my_canvas.endTransformation();
		}
		else
		{
			my_canvas.beginTransformation();
			my_canvas.translate(center_.x, center_.y);
			my_canvas.drawFilledBox(RGBColors::green, 0.1, 0.1);
			my_canvas.endTransformation();
		}
	}

	// ���� ���� �� �ʱ� ��ġ ����
	void initPosition()
	{
		if (player_)
		{
			center_.x = -1;
			center_.y = rand() % -1 - 0.3;
		}
		else
		{
			center_.x = -1;
			center_.y = rand() % 1 + 0.3;
		}
	}

	// ���� �� ������Ʈ
	void setValues(vec3 _center, vec3 _direction)
	{
		center_ = _center;
		direction_ = _direction;
	}

	// �⺻���� Ű����
	bool keySettings()
	{
		if (dead_) return false;	// ���� ĳ���ʹ� ������ �� ����

		if (player_)	// 1P Ű����
		{
			// move left & right side
			if (my_canvas.isKeyPressed(GLFW_KEY_A)) {
				center_.x -= speed_;
			}
			if (my_canvas.isKeyPressed(GLFW_KEY_D)) {
				center_.x += speed_;
			}

			// move up & down
			if (my_canvas.isKeyPressed(GLFW_KEY_W)) {
				center_.y += speed_;
			}
			if (my_canvas.isKeyPressed(GLFW_KEY_S)) {
				center_.y -= speed_;
			}

			if (my_canvas.isKeyPressed(GLFW_KEY_CAPS_LOCK))
				defense_ = true;
			else defense_ = false;

			return true;
		}
		else		// 2P Ű����
		{
			// move left & right side
			if (my_canvas.isKeyPressed(GLFW_KEY_LEFT)) {
				center_.x += speed_;
			}
			if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT)) {
				center_.x -= speed_;
			}

			// move up & down
			if (my_canvas.isKeyPressed(GLFW_KEY_UP)) {
				center_.y += speed_;
			}
			if (my_canvas.isKeyPressed(GLFW_KEY_DOWN)) {
				center_.y -= speed_;
			}

			if (my_canvas.isKeyPressed(GLFW_KEY_ENTER))
				defense_ = true;
			else defense_ = false;

			return true;
		}
	}

	// �ǰ� �� �ڷ� �и�(�˹�)
	void knockback()
	{
		center_.x -= 0.002 * PERFORMANCE;
		center_.y -= 0.002 * PERFORMANCE;
	}

	// ������ ǥ������ �հ��� ������
	void win()
	{
		my_canvas.beginTransformation();
		if (player_) my_canvas.translate(center_.x, center_.y + 0.05);	// ���ڰ� 1P�� ���
		else my_canvas.translate(-center_.x, center_.y + 0.05);		// 2P�� ���
		my_canvas.drawFilledTriangle(RGBColors::yellow, 0.08, 0.04);
		my_canvas.translate(-0.03, 0.0);
		my_canvas.rotate(-90);
		my_canvas.drawFilledTriangle(RGBColors::yellow, 0.04, 0.02);
		my_canvas.rotate(90);
		my_canvas.translate(0.06, 0.0);
		my_canvas.rotate(90);
		my_canvas.drawFilledTriangle(RGBColors::yellow, 0.04, 0.02);
		my_canvas.rotate(-90);
		my_canvas.endTransformation();

		drawOver();
	}

	// �� �׸��� + ���� ���� �� �ڵ�����
	void drawOver()
	{
		static float timer = 0.0;	// ���� Ÿ�̸� - �� �Լ� ���� �� �� 3�� �� ����
									// ��
		my_canvas.beginTransformation();
		my_canvas.translate(-0.4, 0.8);
		my_canvas.drawFilledBox(RGBColors::black, 0.35, 0.075);
		my_canvas.translate(0.45, 0.0);
		my_canvas.drawFilledBox(RGBColors::black, 0.35, 0.075);
		my_canvas.translate(-0.35, -0.25);
		my_canvas.drawFilledBox(RGBColors::black, 0.05, 0.3);
		my_canvas.translate(0.45, 0.0);
		my_canvas.drawFilledBox(RGBColors::black, 0.05, 0.3);

		// ��
		my_canvas.translate(-0.15, -0.4);
		my_canvas.drawFilledBox(RGBColors::black, 1.6, 0.05);

		// ��
		my_canvas.translate(0.0, -0.4);
		my_canvas.drawFilledBox(RGBColors::black, 0.5, 0.05);
		my_canvas.translate(0.0, -0.3);
		my_canvas.drawFilledBox(RGBColors::black, 0.5, 0.05);
		my_canvas.translate(0.0, -0.3);
		my_canvas.drawFilledBox(RGBColors::black, 0.5, 0.05);
		my_canvas.translate(-0.2, 0.125);
		my_canvas.drawFilledBox(RGBColors::black, 0.05, 0.25);
		my_canvas.translate(0.0, 0.35);
		my_canvas.drawFilledBox(RGBColors::black, 0.05, 0.25);

		my_canvas.endTransformation();

		timer += 1.0;

		if (timer >= 2000)
			exit(0);
	}

	// hp�� �׸���
	void drawHP()
	{
		my_canvas.translate(0.0, 0.45);
		if (!player_) my_canvas.rotate(180);	// 1P�� 2P�� HP�� ��� �����ʿ������� ����ϱ� ����
		my_canvas.drawFilledBox(RGBColors::red, 0.5, 0.05);
		my_canvas.translate(hp_ * 0.0025 - 0.25, 0.0);
		my_canvas.drawFilledBox(RGBColors::green, hp_ * 0.005, 0.05);
		if (!player_) my_canvas.rotate(-180);
	}

	// �÷��̾ 1P���� 2P������ ȭ��ǥ�� ǥ��
	void drawPlayerNum()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y + 0.2);
		my_canvas.scale(1.0, -1.0);
		my_canvas.drawFilledTriangle(RGBColors::green, 0.1, 0.1);

		if (player_)
		{
			my_canvas.translate(0.0, -0.01);
			my_canvas.drawFilledBox(RGBColors::black, 0.01, 0.05);
		}
		else
		{
			my_canvas.drawFilledBox(RGBColors::black, 0.035, 0.01);
			my_canvas.translate(0.0, -0.02);
			my_canvas.drawFilledBox(RGBColors::black, 0.035, 0.01);
			my_canvas.translate(0.0, -0.02);
			my_canvas.drawFilledBox(RGBColors::black, 0.035, 0.01);
			my_canvas.translate(-0.015, 0.01);
			my_canvas.drawFilledBox(RGBColors::black, 0.01, 0.03);
			my_canvas.translate(0.03, 0.02);
			my_canvas.drawFilledBox(RGBColors::black, 0.01, 0.03);
		}
		drawHP();
		my_canvas.endTransformation();
	}

	// �ǰݿ� ���� �Ǹ� �긮�� ���
	void bleeding(vec3 pos, float& time)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(pos.x, pos.y);
		my_canvas.rotate(sin(time*30.0* PERFORMANCE) * 270);
		my_canvas.drawFilledCircle(RGBColors::red, 0.01, 10);
		my_canvas.translate(-0.01, -0.01);
		my_canvas.drawFilledCircle(RGBColors::red, 0.01, 10);
		my_canvas.translate(-0.01, -0.01);
		my_canvas.drawFilledCircle(RGBColors::red, 0.01, 10);
		my_canvas.translate(0.02, -0.02);
		my_canvas.drawFilledCircle(RGBColors::red, 0.01, 10);
		my_canvas.translate(-0.03, 0.0);
		my_canvas.drawFilledCircle(RGBColors::red, 0.01, 10);
		my_canvas.endTransformation();
	}

	// Ư����ų�� Ȱ�� ���θ� �˷��ִ� ����� ����Ʈ
	void special_effect(float& time, const vec3& color)
	{
		my_canvas.beginTransformation();
		if (!player_)
		{
			my_canvas.scale(-1.0, 1.0);
		}
		my_canvas.translate(center_.x, center_.y + 0.2);
		my_canvas.rotate(time * 90.0 * PERFORMANCE);
		my_canvas.translate(-0.05, 0.0);
		my_canvas.drawFilledCircle(color, 0.02, 100);
		if (!player_)
		{
			my_canvas.scale(-1.0, 1.0);
		}
		my_canvas.endTransformation();
	}
};