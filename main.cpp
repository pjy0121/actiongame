#include "DigitalCanvas2D.h"
#include "RGBColors.h"
#include <iostream>
#include <cmath>

#define P1 true
#define P2 false

using namespace std;
using namespace glm;

DigitalCanvas2D my_canvas("This is my digital canvas!", 1024, 768); // Canvas : (-1.0, -1.0) x (1.0, 1.0)

class Human
{
protected:
	int attack_;
	int defense_;
	bool player_;

public:
	vec3 center_;
	vec3 direction_;
	int hp_;
	

	// 생성자
	Human() : hp_(100), attack_(10), defense_(10)
	{}

	// 게임 시작 시 초기 위치 지정
	void initPosition()
	{
		if (player_)
		{
			center_.x = -1;
			center_.y = rand() % -1 - 0.5;
		}
		else
		{
			center_.x = -1;
			center_.y = rand() % 1 + 0.5;
		}
	}

	void setValues(vec3 _center, vec3 _direction)
	{
		center_ = _center;
		direction_ = _direction;
	}

	bool keySettings()
	{
		if (player_)
		{
			// move left & right side
			if (my_canvas.isKeyPressed(GLFW_KEY_LEFT)) {
				center_.x -= 0.0004;
			}
			if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT)) {
				center_.x += 0.0004;
			}

			// move up & down
			if (my_canvas.isKeyPressed(GLFW_KEY_UP)) {
				center_.y += 0.0004;
			}
			if (my_canvas.isKeyPressed(GLFW_KEY_DOWN)) {
				center_.y -= 0.0004;
			}
			return P1;
		}
		else
		{
			// move left & right side
			if (my_canvas.isKeyPressed(GLFW_KEY_A)) {
				center_.x += 0.0004;
			}
			if (my_canvas.isKeyPressed(GLFW_KEY_D)) {
				center_.x -= 0.0004;
			}

			// move up & down
			if (my_canvas.isKeyPressed(GLFW_KEY_W)) {
				center_.y += 0.0004;
			}
			if (my_canvas.isKeyPressed(GLFW_KEY_S)) {
				center_.y -= 0.0004;
			}
			return P2;
		}
	}

	void drawHead(const vec3& color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledCircle(RGBColors::black, 0.04);
	}

	void drawBody(const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(0.0, -0.04);
		my_canvas.drawFilledBox(RGBColors::black, 0.04, 0.16);
		my_canvas.endTransformation();
	}

	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.05);
		my_canvas.rotate(20);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

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

	void drawSword(const float & time, const vec3 & color)
	{
		my_canvas.translate(0.045, 0.8);
		my_canvas.drawFilledBox(color, 0.05, 1.6);
	}

	void drawLeftLeg(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x-0.01, center_.y-0.13);
		my_canvas.drawFilledBox(RGBColors::black, 0.018, 0.03);
		my_canvas.translate(0.0, -0.02);
		my_canvas.rotate(20 * sqrt(sin(time * 60) + 1));
		my_canvas.drawFilledBox(RGBColors::black, 0.015, 0.02);
		my_canvas.translate(0.0, -0.01);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.01);
		my_canvas.endTransformation();
	}

	void drawRightLeg(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.01, center_.y - 0.13);
		my_canvas.drawFilledBox(RGBColors::black, 0.018, 0.03);
		my_canvas.translate(0.0, -0.02);
		my_canvas.rotate(20*sqrt(sin(time*60)+1));
		my_canvas.drawFilledBox(RGBColors::black, 0.015, 0.02);
		my_canvas.translate(0.0, -0.01);
		my_canvas.drawFilledBox(RGBColors::black, 0.02, 0.01);
		my_canvas.endTransformation();
	}

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
};

class MyTank: public Human
{
public:
	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledBox(RGBColors::green, 0.25, 0.1);
		my_canvas.translate(-0.02, 0.1);
		my_canvas.drawFilledBox(RGBColors::blue, 0.15, 0.09);
		my_canvas.translate(0.15, 0.0);
		my_canvas.drawFilledBox(RGBColors::red, 0.15, 0.03);
		my_canvas.endTransformation();
	}

	// 좌우대칭 함수
	void reverseDraw()
	{
		my_canvas.scale(-1.0, 1.0);
		draw();
		my_canvas.scale(-1.0, 1.0);
	}
};

class MyWarrior : public Human
{
public:
	// 생성자
	MyWarrior(bool _player)
	{
		player_ = _player;
		attack_ = 20;
		defense_ = 20;

		initPosition();	// 시작 위치 설정
	}

	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.05);
		my_canvas.rotate(20);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

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

	// 오른손 스윙
	void actionRight_1(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(sin(time*5.0) * 50.0);			// animation!
		my_canvas.translate(0.04, -0.03);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.18, 0.18);
		my_canvas.endTransformation();
	}

	// 왼손 스윙
	void actionLeft_1(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(sin(time*5.0) * 60.0);			// animation!
		my_canvas.translate(0.03, -0.039);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.18, 0.18);
		my_canvas.translate(0.02, 0.0);
		drawSword(time, RGBColors::red);
		my_canvas.endTransformation();
	}

	void draw(const float& time)
	{
		if (!player_)	// 1P가 아니라면 좌우반전
		{
			my_canvas.scale(-1.0, 1.0);
		}
		drawHead(RGBColors::black);
		drawBody(RGBColors::black);
		if (player_)
		{
			if (my_canvas.isKeyPressed(GLFW_KEY_K)) actionLeft_1(time, RGBColors::black); else drawLeftArm(time, RGBColors::black);
			if (my_canvas.isKeyPressed(GLFW_KEY_K)) actionRight_1(time, RGBColors::black); else drawRightArm(time, RGBColors::black);
		}
		else
		{
			if (my_canvas.isKeyPressed(GLFW_KEY_R)) actionLeft_1(time, RGBColors::black); else drawLeftArm(time, RGBColors::black);
			if (my_canvas.isKeyPressed(GLFW_KEY_R)) actionRight_1(time, RGBColors::black); else drawRightArm(time, RGBColors::black);
		}
		drawLeftLeg(time, RGBColors::black);
		drawRightLeg(time, RGBColors::black);
		
		if(!player_)	// 1P가 아니라면 좌우반전
		{
			my_canvas.scale(-1.0, 1.0);
		}
	}
};

class MyGunner : public Human
{
public:
	MyGunner(bool _player)
	{
		player_ = _player;

		initPosition();	// 시작 위치 설정
	}

	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.02, center_.y - 0.05);
		my_canvas.rotate(20);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

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

	// 오른손 스윙
	void actionRight_1(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(sin(time*5.0) * 50.0);			// animation!
		my_canvas.translate(0.04, -0.03);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.18, 0.18);
		my_canvas.endTransformation();
	}

	// 왼손 스윙
	void actionLeft_1(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(sin(time*5.0) * 60.0);			// animation!
		my_canvas.translate(0.03, -0.039);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.18, 0.18);
		my_canvas.translate(0.02, 0.0);
		drawSword(time, RGBColors::red);
		my_canvas.endTransformation();
	}

	void draw(const float& time)
	{
		if (!player_)	// 1P가 아니라면 좌우반전
		{
			my_canvas.scale(-1.0, 1.0);
		}
		drawHead(RGBColors::black);
		drawBody(RGBColors::black);
		if (player_)
		{
			if (my_canvas.isKeyPressed(GLFW_KEY_K)) actionLeft_1(time, RGBColors::black); else drawLeftArm(time, RGBColors::black);
			if (my_canvas.isKeyPressed(GLFW_KEY_K)) actionRight_1(time, RGBColors::black); else drawRightArm(time, RGBColors::black);
		}
		else
		{
			if (my_canvas.isKeyPressed(GLFW_KEY_R)) actionLeft_1(time, RGBColors::black); else drawLeftArm(time, RGBColors::black);
			if (my_canvas.isKeyPressed(GLFW_KEY_R)) actionRight_1(time, RGBColors::black); else drawRightArm(time, RGBColors::black);
		}
		drawLeftLeg(time, RGBColors::black);
		drawRightLeg(time, RGBColors::black);

		if (!player_)	// 1P가 아니라면 좌우반전
		{
			my_canvas.scale(-1.0, 1.0);
		}
	}
};

class MyBullet
{
public:
	vec3 center_;
	vec3 velocity_;

	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledCircle(RGBColors::yellow, 0.02, 8);
		my_canvas.endTransformation();
	}

	void update(const float& dt)
	{
		center_ += velocity_*dt;
	}
};

class MyRaser: public MyBullet
{
public:
	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.6,  center_.y);
		my_canvas.drawFilledBox(RGBColors::red, 2, 0.05);
		my_canvas.endTransformation();
	}
};

int main(void)
{
	float time = 0.0;

	MyWarrior warrior(P1);
	MyGunner gunner(P2);
	MyBullet *bullet = nullptr;
	MyRaser *raser = nullptr;
	
	my_canvas.show([&]
	{
		warrior.keySettings();	// 키셋팅
		gunner.keySettings();	// 키셋팅

		// shoot a cannon ball
		if (my_canvas.isKeyPressed(GLFW_KEY_SPACE))
		{
			for(int i=0; i<3; i++)
			{
				if (bullet != nullptr) { delete bullet; }
				bullet = new MyBullet;
				bullet->center_ = gunner.center_;
				bullet->center_.x += 0.2;
				bullet->center_.y += 0.1;
				bullet->velocity_ = vec3(2.0, 0.0, 0.0);
			}
		}

		if (my_canvas.isKeyPressed(GLFW_KEY_ENTER))
		{
			if (raser != nullptr) delete raser;
			raser = new MyRaser;
			raser->center_ = warrior.center_;
			raser->center_.x += 0.2;
			raser->center_.y += 0.1;
			raser->velocity_ = vec3(3.0, 0.0, 0.0);
		}
		
		if (bullet != nullptr) bullet->update(1/60.0f);
		if (raser != nullptr) raser->update(1 / 10.0f);

		// rendering
		warrior.draw(time);
		gunner.draw(time);

		if (bullet != nullptr) bullet->draw();
		if (raser != nullptr) raser->draw();

		time += 1 / 600.0;
	}
	);

	return 0;
}
