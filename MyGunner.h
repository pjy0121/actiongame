#pragma once
#include "MyHuman.h"

class MyGunner : public MyHuman
{
public:
	vec3 gunPos_;	// 총알의 위치
	bool fadeaway_;	// 특수스킬 fadeaway의 발동 가능 여부

	// 생성자
	MyGunner(bool _player) :fadeaway_(true)
	{
		gunPos_ = center_;
		player_ = _player;

		initPosition();	// 시작 위치 설정
	}

	// 캐릭터의 왼팔 그리기(@Overriding)
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

	// 캐릭터의 오른팔 그리기(@Overriding)
	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.05);
		my_canvas.rotate(-15);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

	// 총 그리기
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

	// 캐릭터 전체 그리기(@Overriding)
	void draw(const float& time)
	{
		if (!player_)	// 1P가 아니라면 좌우반전
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

		if (!player_)	// 1P가 아니라면 좌우반전
		{
			my_canvas.scale(-1.0, 1.0);
		}
	}

	// 죽었을 때의 모션 그리기
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

	// 근거리 공격 이펙트
	void close_shot()
	{
		my_canvas.beginTransformation();
		if (!player_)	// gunner가 2P일 경우
		{
			my_canvas.scale(-1.0, 1.0);	// 좌우대칭
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
		if (!player_)	// gunner가 2P일 경우
		{
			my_canvas.scale(-1.0, 1.0);	// 좌우대칭
		}
		my_canvas.endTransformation();
	}

	// 뒤로 빠르게 물러서기 - 7초마다 한번 쓸 수 있는 gunner의 특수 스킬
	void fadeaway(float& time)
	{
		if (fadeaway_)		// fadeaway를 쓸 수 있을 때
		{
			special_effect(time, RGBColors::blue);	// fadeaway가 활성상태인지를 알려주는 공모양 이펙트
			if (player_)	// gunner가 1P일 때
			{
				if (my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_W))		// 위쪽으로 fadeaway
				{
					fade_to(-0.4, 0.4); fadeaway_ = false;
				}

				if (my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_S))		// 아래쪽으로 fadeaway
				{
					fade_to(-0.4, -0.4); fadeaway_ = false;
				}
			}
			else	// gunner가 2P일 때
			{
				if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_UP))		// 위쪽으로 fadeaway
				{
					fade_to(-0.4, 0.4); fadeaway_ = false;
				}

				if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_DOWN))		// 아래쪽으로 fadeaway
				{
					fade_to(-0.4, -0.4); fadeaway_ = false;
				}
			}
		}		// ↓ fadeaway의 쿨타임(재사용 대기 시간)을 7초로 두기 위함
		else if ((int)time % 7 == 0 && 0 < time - (int)time && time - (int)time < 0.1) fadeaway_ = true;
	}

	// fadeaway 되는 위치
	void fade_to(double x, double y)
	{
		center_.x += x;
		center_.y += y;
	}
};