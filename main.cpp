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
	

	// ������
	Human() : hp_(100), defense_(10), speed_(0.0006), dead_(false)
	{}

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

	void setValues(vec3 _center, vec3 _direction)
	{
		center_ = _center;
		direction_ = _direction;
	}

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
			return true;
		}
	}

	// �ǰ� �� �ڷ� �и�(�˹�)
	void knockback()
	{
		center_.x -= 0.003;
		center_.y -= 0.003;
	}

	// ������ ǥ������ �հ��� ������
	void win()
	{
		my_canvas.beginTransformation();
		if(player_) my_canvas.translate(center_.x, center_.y + 0.05);	// ���ڰ� 1P�� ���
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

	// Ư����ų�� Ȱ�� ���θ� �˷��ִ� ����� ����Ʈ
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

	// ������
	MyWarrior(bool _player) : dash_(true)
	{
		player_ = _player;
		defense_ = 20;
		speed_ = 0.0009;
		swordPos_ = center_;
		
		initPosition();	// ���� ��ġ ����
	}

	// �׾��� ���� ���
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

	// ������ ���� - 8�ʸ��� �ѹ� �� �� �ִ� �������� Ư�� ��ų
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
		else if ((int)time % 8 == 0 && 0 < time - (int)time && time - (int)time < 0.01) dash_ = true;
	}

	// dash�Ǵ� ��ġ
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
		swordPos_.x = -10.0;	// ���� ������ ��� �ִ� ������ ���� �ǰݵ��� �ʵ��� �ϱ� ����
		swordPos_.y = -10.0;
	}

	// ������ ����(����)
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

	// ���� ����(����)
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
		swordPos_.x = x + pow(0.35*sin(time*8.0+90), 2);	// ������ �� ��ǥ ��ȭ
		swordPos_.y = y + pow(0.5*sin(time*8.0), 2) - 0.22;
		// printf("%lf %lf\n", swordPos_.x, swordPos_.y);
	}

	// ������ ���
	void actionRight_2(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x + 0.04, center_.y - 0.065);
		my_canvas.drawFilledBox(color, 0.09, 0.02);
		my_canvas.endTransformation();
	}

	// ���� ���
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

	// Ÿ�ݴ��� ��ġ�� ǥ���ϴ� �Լ� �Լ�
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

		if(!player_)	// 1P�� �ƴ϶�� �¿����
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

		initPosition();	// ���� ��ġ ����
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

	// �׾��� ���� ���
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

	// �ڷ� ������ �������� - 11�ʸ��� �ѹ� �� �� �ִ� gunner�� Ư�� ��ų
	void fadeaway(float& time)
	{
		if (fadeaway_)		// fadeaway�� �� �� ���� ��
		{
			special_effect(time, RGBColors::blue);	// fadeaway�� Ȱ������������ �˷��ִ� ����� ����Ʈ
			if (player_)	// gunner�� 1P�� ��
			{	
				if(my_canvas.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && my_canvas.isKeyPressed(GLFW_KEY_W))		// �������� fadeaway
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
		else if ((int)time % 7 == 0 && 0 < time - (int)time && time - (int)time < 0.01) fadeaway_ = true;
	}

	// fadeaway �Ǵ� ��ġ
	void fade_to(double x, double y)
	{
		center_.x += x;
		center_.y += y;
	}

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
	int bg = 0;	// ��� ��ȣ
	cout << "����� �������ּ���. (1) �� (2) �ٴ� (3) ��\n --> " << endl;
	cin >> bg;

	int select = 0;
	cout << "ĳ���͸� �������ּ���." << endl;
	cout << "(1) ������(1P) vs �ų�(2P) (2) �ų�(1P) vs ������(2P)\n --> " << endl;
	cin >> select;

	if (select == 2)	// 2���� ���� �� warrior�� 2P, gunner�� 1P
	{
		warrior.player_ = false; gunner.player_ = true;
	}
	*/
	my_canvas.show([&]
	{
	/*
		 // ��� ����
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

		warrior.keySettings();	// Ű����
		gunner.keySettings();	// Ű����
		
		// shoot a bullet
		if ((gunner.player_ && my_canvas.isKeyPressed(GLFW_KEY_T)) || (!gunner.player_ && my_canvas.isKeyPressed(GLFW_KEY_K)))
		{
			vec3 gp = gunner.gunPos_;
			vec3 wc = warrior.center_;
			if (gunner.player_)
				wc.x = -wc.x;
			else gp.x = -gp.x;
			if (is_close(gp, wc) && !gunner.dead_)	// ������� �ٰŸ��� ��ġ�� ���
			{
				if (bullet != nullptr) { vec3 bc = bullet->center_; warrior.bleeding(bc, time); }	// �� �긮�� ���
				gunner.close_shot();	// �ٰŸ� ���� ��
				warrior.hp_ -= 0.01;	// ������� �������� ����
				printf("������ �ǰ�! - HP : %lf\n", warrior.hp_);
				warrior.knockback();	// ������ �˹�
				gunner.knockback();	// �ڷ� ������
			}

			else
			{
				for (int i = 0; i < 3; i++)
				{
					if (bullet != nullptr) { delete bullet; }
					bullet = new MyBullet;
					bullet->center_.y = gunner.center_.y - 0.05;

					if (gunner.player_)	// Gunner�� 1P�� ���
						bullet->center_.x = gunner.center_.x + 0.2;
					else
						bullet->center_.x = -gunner.center_.x - 0.2;	// Gunner�� 2P�� ���
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

		if (gunner.dead_ && bullet != nullptr) bullet = nullptr;	// gunner ��� �� �Ѿ��� �߻��� �� ������ ����
		if (bullet != nullptr) bullet->draw();

		vec3* attackedPos = &(bullet->center_);
		if (bullet != nullptr)
		{
			vec3 wc = warrior.center_;

			if (!warrior.player_) wc.x = -wc.x; // warrir�� 2P�� ���
			
			// gunner�� ���ݿ� warrior�� �ǰ�
			if (is_attacked(wc, bullet->center_, attackedPos))
			{
				vec3 bc = bullet->center_;
				bullet = nullptr;	// ������ �Ѿ��� �����
				warrior.hp_ -= 20.0;	// hp����

				printf("������ �ǰ�! - HP : %lf\n", warrior.hp_);
				warrior.knockback();

				warrior.bleeding(bc, time);
			}
		}

		// warrior�� ���ݿ� gunner�� �ǰ�
		vec3 sp = warrior.swordPos_;
		vec3 gc = gunner.center_;
		if (warrior.player_)	// warrior�� 1P�� ���
			gc.x = -gc.x;	// gunner�� x��ǥ�� y���� �߽����� ȸ���Ǿ��� ������ ����
		else sp.x = -sp.x;

		if (is_attacked(sp, gc))
		{
			// hp����
			if ((my_canvas.isKeyPressed(GLFW_KEY_T) && warrior.player_)|| (my_canvas.isKeyPressed(GLFW_KEY_L) && !warrior.player_))	// ����� ���
				gunner.hp_ -= 0.1;
			else gunner.hp_ -= 0.5;	// ����(����) �� ���
			printf("�ų� �ǰ�! - HP : %lf\n", gunner.hp_);
			gunner.bleeding(sp, time);	// �Ǹ� �긮�� ���
		}

		if (warrior.hp_ <= 0) { warrior.dead_ = true; gunner.win(); }		// warrior ���, gunner�� �¸�
		if (gunner.hp_ <= 0) { gunner.dead_ = true; warrior.win(); }	// gunner���, warrior�� �¸�
		
		warrior.dash(time);		// warrior�� dash ��ų�� ����� �� �ֵ��� ����
		gunner.fadeaway(time);	// gunner�� fadeaway ��ų�� ����� �� �ֵ��� ����
		time += 1 / 600.0;
	}
	);
	return 0;
}

// �� ���� ��ǥ�� ���� �� �Ǵ��ϱ�(�ǰݿ��� �Ǵ�)
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

// �� ���� ��ǥ�� ���� �� �Ǵ��ϱ�(�ǰݿ��� �Ǵ�)
int is_attacked(vec3& a, vec3& b)
{
	if (b.x - 0.03 < a.x && a.x < b.x + 0.02 &&  b.y - 0.15 < a.y && a.y < b.y + 0.05)
		return 1;
	else return 0;
}

// �� ���� ������ �ִ��� �Ǵ�
int is_close(vec3& a, vec3& b)
{
	if (b.x - 0.17 < a.x && a.x < b.x + 0.17 && b.y - 0.1 < a.y && a.y < b.y + 0.1)
		return 1;
	else return 0;
}