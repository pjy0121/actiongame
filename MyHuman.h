#pragma once
#include "DigitalCanvas2D.h"
#include "RGBColors.h"
#include <iostream>
#include <cmath>
#include <cstring>

#define PERFORMANCE 2.0		// 배경에 따라 느려지는 현상이 발생해서 속도 조정

using namespace std;
using namespace glm;

DigitalCanvas2D my_canvas("This is my digital canvas!", 1024, 768); // Canvas : (-1.0, -1.0) x (1.0, 1.0)

class MyHuman
{
public:
	bool defense_;	// 방어모드 On true, Off : false
	double hp_;		// 캐릭터의 남은 hp량
	double speed_;	// 캐릭터의 고유 스피드
	bool player_;	// 1P면 true, 2P면 false
	bool dead_;	// 죽었으면 true, 살아있으면 false

	vec3 center_;
	vec3 direction_;


	// 생성자
	MyHuman() : hp_(100), defense_(false), speed_(0.0006 * PERFORMANCE), dead_(false)
	{}

	// 캐릭터의 머리 그리기
	void drawHead(const vec3& color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledCircle(RGBColors::black, 0.04);
	}

	// 캐릭터의 몸통 그리기
	void drawBody(const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(0.0, -0.04);
		my_canvas.drawFilledBox(RGBColors::black, 0.04, 0.16);
		my_canvas.endTransformation();
	}

	// 캐릭터의 왼팔 그리기
	void drawLeftArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.065);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.translate(0.05, -0.06);
		my_canvas.endTransformation();
	}

	// 캐릭터의 오른팔 그리기
	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.05);
		my_canvas.rotate(20);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

	// 캐릭터의 왼다리 그리기
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

	// 캐릭터의 오른다리 그리기
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

	// 캐릭터 전체 그리기
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

	// 게임 시작 시 초기 위치 지정
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

	// 내부 값 업데이트
	void setValues(vec3 _center, vec3 _direction)
	{
		center_ = _center;
		direction_ = _direction;
	}

	// 기본적인 키셋팅
	bool keySettings()
	{
		if (dead_) return false;	// 죽은 캐릭터는 움직일 수 없음

		if (player_)	// 1P 키셋팅
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
		else		// 2P 키셋팅
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

	// 피격 시 뒤로 밀림(넉백)
	void knockback()
	{
		center_.x -= 0.002 * PERFORMANCE;
		center_.y -= 0.002 * PERFORMANCE;
	}

	// 승자의 표식으로 왕관을 씌워줌
	void win()
	{
		my_canvas.beginTransformation();
		if (player_) my_canvas.translate(center_.x, center_.y + 0.05);	// 승자가 1P일 경우
		else my_canvas.translate(-center_.x, center_.y + 0.05);		// 2P일 경우
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

	// 끝 그리기 + 게임 끝난 후 자동종료
	void drawOver()
	{
		static float timer = 0.0;	// 종료 타이머 - 이 함수 실행 후 약 3초 후 종료
									// ㄲ
		my_canvas.beginTransformation();
		my_canvas.translate(-0.4, 0.8);
		my_canvas.drawFilledBox(RGBColors::black, 0.35, 0.075);
		my_canvas.translate(0.45, 0.0);
		my_canvas.drawFilledBox(RGBColors::black, 0.35, 0.075);
		my_canvas.translate(-0.35, -0.25);
		my_canvas.drawFilledBox(RGBColors::black, 0.05, 0.3);
		my_canvas.translate(0.45, 0.0);
		my_canvas.drawFilledBox(RGBColors::black, 0.05, 0.3);

		// ㅡ
		my_canvas.translate(-0.15, -0.4);
		my_canvas.drawFilledBox(RGBColors::black, 1.6, 0.05);

		// ㅌ
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

	// hp바 그리기
	void drawHP()
	{
		my_canvas.translate(0.0, 0.45);
		if (!player_) my_canvas.rotate(180);	// 1P와 2P의 HP가 모두 오른쪽에서부터 닳게하기 위함
		my_canvas.drawFilledBox(RGBColors::red, 0.5, 0.05);
		my_canvas.translate(hp_ * 0.0025 - 0.25, 0.0);
		my_canvas.drawFilledBox(RGBColors::green, hp_ * 0.005, 0.05);
		if (!player_) my_canvas.rotate(-180);
	}

	// 플레이어가 1P인지 2P인지를 화살표로 표시
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

	// 피격에 따라 피를 흘리는 모션
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

	// 특수스킬의 활성 여부를 알려주는 공모양 이펙트
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