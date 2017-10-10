#pragma once
#include "MyHuman.h"

class MyWarrior : public MyHuman
{
public:
	vec3 attackedPos_;	// 피격 위치 
	vec3 swordPos_;		// 검의 위치
	bool dash_;		// 특수스킬 dash의 발동 가능 여부

					// 생성자
	MyWarrior(bool _player) : dash_(true)
	{
		player_ = _player;
		speed_ = 0.0009 * PERFORMANCE;
		swordPos_ = center_;

		initPosition();	// 시작 위치 설정
	}


	// 캐릭터의 왼팔 그리기(@overriding) 
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

	// 캐릭터의 오른팔 그리기(@overriding) 
	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.05);
		my_canvas.rotate(20);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

	// 칼 그리기
	void drawSword(const float & time, const vec3 & color)
	{
		my_canvas.translate(0.045, 0.8);
		my_canvas.drawFilledBox(color, 0.05, 1.6);
		my_canvas.translate(0.0, 0.95);
		my_canvas.drawFilledTriangle(color, 0.05, 0.3);
		my_canvas.translate(0.0, -1.3);
		my_canvas.drawFilledBox(color, 0.1, 0.2);
		swordPos_.x = -10.0;	// 검을 가만히 들고 있는 상태일 때는 피격되지 않도록 하기 위함
		swordPos_.y = -10.0;
	}

	// 왼팔 스윙(베기)
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
		swordPos_.x = x + pow(0.35*sin(time*8.0* PERFORMANCE + 90), 2);	// 스윙할 때 좌표 변화
		swordPos_.y = y + pow(0.5*sin(time*8.0* PERFORMANCE), 2) - 0.22;
		// printf("%lf %lf\n", swordPos_.x, swordPos_.y);
	}

	// 오른팔 스윙(베기)
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

	// 왼팔 찌르기
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

	// 오른팔 찌르기
	void actionRight_2(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.065);
		my_canvas.drawFilledBox(color, 0.09, 0.02);
		my_canvas.endTransformation();
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
			if (player_)
			{
				// 왼팔 그리기
				if (my_canvas.isKeyPressed(GLFW_KEY_R)) actionLeft_1(time, RGBColors::black);
				else if (my_canvas.isKeyPressed(GLFW_KEY_T)) actionLeft_2(time, RGBColors::black);
				else drawLeftArm(time, RGBColors::black);

				// 오른팔 그리기
				if (my_canvas.isKeyPressed(GLFW_KEY_R)) actionRight_1(time, RGBColors::black);
				else if (my_canvas.isKeyPressed(GLFW_KEY_T)) actionRight_2(time, RGBColors::black);
				else drawRightArm(time, RGBColors::black);
			}
			else
			{
				// 왼팔 그리기
				if (my_canvas.isKeyPressed(GLFW_KEY_K)) actionLeft_1(time, RGBColors::black);
				else if (my_canvas.isKeyPressed(GLFW_KEY_L)) actionLeft_2(time, RGBColors::black);
				else drawLeftArm(time, RGBColors::black);

				// 오른팔 그리기
				if (my_canvas.isKeyPressed(GLFW_KEY_K)) actionRight_1(time, RGBColors::black);
				else if (my_canvas.isKeyPressed(GLFW_KEY_L)) actionRight_2(time, RGBColors::black);
				else drawRightArm(time, RGBColors::black);
			}
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

	// 앞으로 돌진 - 5초마다 한번 쓸 수 있는 워리어의 특수 스킬
	void dash(float& time)
	{
		if (dash_)		// dash를 쓸 수 있을 때
		{
			special_effect(time, RGBColors::red);	// dash가 활성상태인지를 알려주는 공모양 이펙트

			if (player_)	// warrior가 1P일 때
			{	// dash는 방향키 조작에 따라 방향이 달라짐
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
			else	// warrior가 2P일 때
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
		}		// ↓ dash의 쿨타임(재사용 대기 시간)을 5초로 두기 위함
		else if ((int)time % 5 == 0 && 0 < time - (int)time && time - (int)time < 0.01) dash_ = true;
	}

	// dash되는 위치
	void dash_to(double x, double y)
	{
		center_.x += x;
		center_.y += y;
	}

	// 타격당한 위치를 표시하는 함수 함수
	void attacked_position(vec3* _attackedPos)
	{
		attackedPos_ = *_attackedPos;
		my_canvas.beginTransformation();
		my_canvas.translate(_attackedPos->x, _attackedPos->y);
		my_canvas.drawFilledCircle(RGBColors::red, 0.01, 10);
		my_canvas.endTransformation();
	}
};