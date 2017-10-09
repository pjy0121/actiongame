#include "DigitalCanvas2D.h"
#include "RGBColors.h"
#include <iostream>
#include <cmath>

#define P1 true
#define P2 false

using namespace std;
using namespace glm;

DigitalCanvas2D my_canvas("This is my digital canvas!", 1024, 768); // Canvas : (-1.0, -1.0) x (1.0, 1.0)
int is_attacked(vec3& a, vec3& b, vec3* attackedPos);
int is_attacked(vec3& a, vec3& b);
int is_close(vec3& a, vec3& b);

class Human
{
public:
	int defense_;
	double hp_;
	double speed_;
	bool player_;
	bool dead_;

	vec3 center_;
	vec3 direction_;
	

	// 생성자
	Human() : hp_(100), defense_(10), speed_(0.0006), dead_(false)
	{}

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

	void setValues(vec3 _center, vec3 _direction)
	{
		center_ = _center;
		direction_ = _direction;
	}

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
			return true;
		}
	}

	// 피격 시 뒤로 밀림(넉백)
	void knockback()
	{
		center_.x -= 0.003;
		center_.y -= 0.003;
	}

	// 승자의 표식으로 왕관을 씌워줌
	void win()
	{
		my_canvas.beginTransformation();
		if(player_) my_canvas.translate(center_.x, center_.y + 0.05);	// 승자가 1P일 경우
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
		my_canvas.rotate(sin(time*30.0) * 270);
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
		my_canvas.rotate(time * 90.0);
		my_canvas.translate(-0.05, 0.0);
		my_canvas.drawFilledCircle(color, 0.02, 100);
		if (!player_)
		{
			my_canvas.scale(-1.0, 1.0);
		}
		my_canvas.endTransformation();
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
		my_canvas.endTransformation();
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

class MyWarrior : public Human
{
public:
	vec3 attackedPos_;
	vec3 swordPos_;
	bool dash_;

	// 생성자
	MyWarrior(bool _player) : dash_(true)
	{
		player_ = _player;
		defense_ = 20;
		speed_ = 0.0009;
		swordPos_ = center_;
		
		initPosition();	// 시작 위치 설정
	}

	// 죽었을 때의 모션
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

	// 앞으로 돌진 - 8초마다 한번 쓸 수 있는 워리어의 특수 스킬
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
		else if ((int)time % 8 == 0 && 0 < time - (int)time && time - (int)time < 0.01) dash_ = true;
	}

	// dash되는 위치
	void dash_to(double x, double y)
	{
		center_.x += x;
		center_.y += y;
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
		my_canvas.translate(0.0, 0.95);
		my_canvas.drawFilledTriangle(color, 0.05, 0.3);
		my_canvas.translate(0.0, -1.3);
		my_canvas.drawFilledBox(color, 0.1, 0.2);
		swordPos_.x = -10.0;	// 검을 가만히 들고 있는 상태일 때는 피격되지 않도록 하기 위함
		swordPos_.y = -10.0;
	}

	// 오른팔 스윙(베기)
	void actionRight_1(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(-90 + pow(sin(time*7.0), 2) * 105.0);			// animation!
		my_canvas.translate(0.04, -0.02);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.18, 0.18);
		my_canvas.endTransformation();
	}

	// 왼팔 스윙(베기)
	void actionLeft_1(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(-90 + pow(sin(time*7.0), 2) * 105.0);			// animation!
		my_canvas.translate(0.03, -0.025);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.18, 0.18);
		my_canvas.translate(0.02, 0.0);
		drawSword(time, RGBColors::red);
		my_canvas.scale(2.5, 10);
		my_canvas.endTransformation();

		double x = center_.x + 0.06;
		double y = center_.y + 0.12;
		swordPos_.x = x + pow(0.35*sin(time*8.0+90), 2);	// 스윙할 때 좌표 변화
		swordPos_.y = y + pow(0.5*sin(time*8.0), 2) - 0.22;
		// printf("%lf %lf\n", swordPos_.x, swordPos_.y);
	}

	// 오른팔 찌르기
	void actionRight_2(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.065);
		my_canvas.drawFilledBox(color, 0.09, 0.02);
		my_canvas.endTransformation();
	}

	// 왼팔 찌르기
	void actionLeft_2(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.05);
		my_canvas.drawFilledBox(color, 0.09, 0.02);
		my_canvas.translate(0.02, 0.01);
		my_canvas.rotate(-90);
		my_canvas.scale(0.4, 0.1);
		drawSword(time, RGBColors::red);
		my_canvas.rotate(90);
		my_canvas.endTransformation();

		swordPos_.x = center_.x + 0.25;
		swordPos_.y = center_.y - 0.02;
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

		if(!player_)	// 1P가 아니라면 좌우반전
		{
			my_canvas.scale(-1.0, 1.0);
		}
	}
};

class MyGunner : public Human
{
public:
	vec3 gunPos_;
	bool fadeaway_;

	MyGunner(bool _player)	:fadeaway_(true)
	{
		gunPos_ = center_;
		player_ = _player;

		initPosition();	// 시작 위치 설정
	}

	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.05);
		my_canvas.rotate(-15);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.2, 0.18);
		my_canvas.endTransformation();
	}

	// 죽었을 때의 모션
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

	// 뒤로 빠르게 물러서기 - 11초마다 한번 쓸 수 있는 gunner의 특수 스킬
	void fadeaway(float& time)
	{
		if (fadeaway_)		// fadeaway를 쓸 수 있을 때
		{
			special_effect(time, RGBColors::blue);	// fadeaway가 활성상태인지를 알려주는 공모양 이펙트
			if (player_)	// gunner가 1P일 때
			{	
				if(my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_W))		// 위쪽으로 fadeaway
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
		else if ((int)time % 7 == 0 && 0 < time - (int)time && time - (int)time < 0.01) fadeaway_ = true;
	}

	// fadeaway 되는 위치
	void fade_to(double x, double y)
	{
		center_.x += x;
		center_.y += y;
	}

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
		my_canvas.drawFilledCircle(RGBColors::gray, 0.01, 8);
		my_canvas.endTransformation();
	}

	void update(const float& dt)
	{
		center_ += velocity_*dt;
	}
};

int main(void)
{
	float time = 0.0;

	MyWarrior warrior(P2);
	MyGunner gunner(P1);
	MyBullet *bullet = nullptr;

	/*
	int bg = 0;	// 배경 번호
	cout << "배경을 설정해주세요. (1) 숲 (2) 바다 (3) 땅\n --> " << endl;
	cin >> bg;

	int select = 0;
	cout << "캐릭터를 선택해주세요." << endl;
	cout << "(1) 워리어(1P) vs 거너(2P) (2) 거너(1P) vs 워리어(2P)\n --> " << endl;
	cin >> select;

	if (select == 2)	// 2번을 선택 시 warrior가 2P, gunner가 1P
	{
		warrior.player_ = false; gunner.player_ = true;
	}
	*/
	my_canvas.show([&]
	{
	/*
		 // 배경 설정
		my_canvas.beginTransformation();
		my_canvas.translate(-0.83, -0.5);
		my_canvas.DoDisplay();
		my_canvas.translate(0.83, 0.5);
		my_canvas.endTransformation();
	*/

		for (int i = -10; i < 10; i++)
		{
			my_canvas.beginTransformation();
			my_canvas.translate(0.0 + 0.1*i, 0.0);
			my_canvas.drawFilledCircle(RGBColors::green, 0.01, 30);
			my_canvas.endTransformation();
		}

		warrior.keySettings();	// 키셋팅
		gunner.keySettings();	// 키셋팅
		
		// shoot a bullet
		if ((gunner.player_ && my_canvas.isKeyPressed(GLFW_KEY_T)) || (!gunner.player_ && my_canvas.isKeyPressed(GLFW_KEY_K)))
		{
			vec3 gp = gunner.gunPos_;
			vec3 wc = warrior.center_;
			if (gunner.player_)
				wc.x = -wc.x;
			else gp.x = -gp.x;
			if (is_close(gp, wc) && !gunner.dead_)	// 워리어와 근거리에 위치할 경우
			{
				if (bullet != nullptr) { vec3 bc = bullet->center_; warrior.bleeding(bc, time); }	// 피 흘리는 모션
				gunner.close_shot();	// 근거리 공격 후
				warrior.hp_ -= 0.01;	// 워리어는 데미지를 입음
				printf("워리어 피격! - HP : %lf\n", warrior.hp_);
				warrior.knockback();	// 워리어 넉백
				gunner.knockback();	// 뒤로 물러섬
			}

			else
			{
				for (int i = 0; i < 3; i++)
				{
					if (bullet != nullptr) { delete bullet; }
					bullet = new MyBullet;
					bullet->center_.y = gunner.center_.y - 0.05;

					if (gunner.player_)	// Gunner가 1P일 경우
						bullet->center_.x = gunner.center_.x + 0.2;
					else
						bullet->center_.x = -gunner.center_.x - 0.2;	// Gunner가 2P일 경우
					bullet->velocity_ = vec3(0.5, 0.0, 0.0);
				}
			}
		}
		
		if (bullet != nullptr)
		{
			if(gunner.player_)
				bullet->update(1 / 95.0f);
			else bullet->update(-1 / 95.0f);
		}

		// rendering
		warrior.draw(time);
		gunner.draw(time);

		if (gunner.dead_ && bullet != nullptr) bullet = nullptr;	// gunner 사망 시 총알을 발사할 수 없도록 설정
		if (bullet != nullptr) bullet->draw();

		vec3* attackedPos = &(bullet->center_);
		if (bullet != nullptr)
		{
			vec3 wc = warrior.center_;

			if (!warrior.player_) wc.x = -wc.x; // warrir가 2P일 경우
			
			// gunner의 공격에 warrior가 피격
			if (is_attacked(wc, bullet->center_, attackedPos))
			{
				vec3 bc = bullet->center_;
				bullet = nullptr;	// 맞으면 총알이 사라짐
				warrior.hp_ -= 20.0;	// hp감소

				printf("워리어 피격! - HP : %lf\n", warrior.hp_);
				warrior.knockback();

				warrior.bleeding(bc, time);
			}
		}

		// warrior의 공격에 gunner가 피격
		vec3 sp = warrior.swordPos_;
		vec3 gc = gunner.center_;
		if (warrior.player_)	// warrior가 1P일 경우
			gc.x = -gc.x;	// gunner의 x좌표는 y축을 중심으로 회전되었기 때문에 반전
		else sp.x = -sp.x;

		if (is_attacked(sp, gc))
		{
			// hp감소
			if ((my_canvas.isKeyPressed(GLFW_KEY_T) && warrior.player_)|| (my_canvas.isKeyPressed(GLFW_KEY_L) && !warrior.player_))	// 찌르기일 경우
				gunner.hp_ -= 0.1;
			else gunner.hp_ -= 0.5;	// 스윙(베기) 일 경우
			printf("거너 피격! - HP : %lf\n", gunner.hp_);
			gunner.bleeding(sp, time);	// 피를 흘리는 모션
		}

		if (warrior.hp_ <= 0) { warrior.dead_ = true; gunner.win(); }		// warrior 사망, gunner의 승리
		if (gunner.hp_ <= 0) { gunner.dead_ = true; warrior.win(); }	// gunner사망, warrior의 승리
		
		warrior.dash(time);		// warrior가 dash 스킬을 사용할 수 있도록 설정
		gunner.fadeaway(time);	// gunner가 fadeaway 스킬을 사용할 수 있도록 설정
		time += 1 / 600.0;
	}
	);
	return 0;
}

// 두 점의 좌표가 같은 지 판단하기(피격여부 판단)
int is_attacked(vec3& a, vec3& b, vec3* attackedPos)
{
	if (b.x - 0.002 < a.x && a.x < b.x + 0.003 &&  b.y - 0.04 < a.y && a.y < b.y + 0.16)
	{
		attackedPos->x = b.x;
		attackedPos->y = b.y;
		return 1;
	}
	else return 0;
}

// 두 점의 좌표가 같은 지 판단하기(피격여부 판단)
int is_attacked(vec3& a, vec3& b)
{
	if (b.x - 0.03 < a.x && a.x < b.x + 0.02 &&  b.y - 0.15 < a.y && a.y < b.y + 0.05)
		return 1;
	else return 0;
}

// 두 점이 가까이 있는지 판단
int is_close(vec3& a, vec3& b)
{
	if (b.x - 0.17 < a.x && a.x < b.x + 0.17 && b.y - 0.1 < a.y && a.y < b.y + 0.1)
		return 1;
	else return 0;
}