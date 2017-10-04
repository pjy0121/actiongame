#include "DigitalCanvas2D.h"
#include "RGBColors.h"
#include <iostream>
#include <cmath>

using namespace std;
using namespace glm;

DigitalCanvas2D my_canvas("This is my digital canvas!", 1024, 768); // Canvas : (-1.0, -1.0) x (1.0, 1.0)

class Shooter
{
public:
	vec3 center_;
	vec3 direction_;

	void setValues(vec3 _center, vec3 _direction)
	{
		center_ = _center;
		direction_ = _direction;
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

	void drawLeftArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(sin(time*5.0) * 30.0);			// animation!
		my_canvas.translate(0.06, -0.03);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.15, 0.18);
		my_canvas.endTransformation();
	}

	void drawRightArm(const float & time, const vec3 & color)
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.rotate(sin(time*5.0) * 40.0);			// animation!
		my_canvas.translate(0.045, -0.039);
		my_canvas.scale(0.4, 0.1);
		my_canvas.drawFilledBox(color, 0.21, 0.18);
		my_canvas.translate(0.01, 0.0);
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
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledBox(RGBColors::green, 0.1, 0.1);
		my_canvas.endTransformation();
	}
};

class MyTank: public Shooter
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
};

class MyCharacter : public Shooter
{
public:
	void draw(const float& time)
	{
		drawHead(RGBColors::black);
		drawBody(RGBColors::black);
		drawLeftArm(time, RGBColors::black);
		drawRightArm(time, RGBColors::black);
		drawLeftLeg(time, RGBColors::black);
		drawRightLeg(time, RGBColors::black);
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

	MyTank tank;
	MyCharacter warrior;
	MyBullet *bullet = nullptr;
	MyRaser *raser = nullptr;

	my_canvas.show([&]
	{
		// move tank(left & right side) - πÊ«‚≈∞
		if (my_canvas.isKeyPressed(GLFW_KEY_LEFT)) {
			tank.center_.x -= 0.001; 
		}
		if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT)) {
			tank.center_.x += 0.001;
		}

		// move tank(up & down)
		if (my_canvas.isKeyPressed(GLFW_KEY_UP)) {
			tank.center_.y += 0.001;
		}
		if (my_canvas.isKeyPressed(GLFW_KEY_DOWN)) {
			tank.center_.y -= 0.001;
		}

		// move warrior(left & right side) - WSAD
		if (my_canvas.isKeyPressed(GLFW_KEY_A)) {
			warrior.center_.x -= 0.001;
		}
		if (my_canvas.isKeyPressed(GLFW_KEY_D)) {
			warrior.center_.x += 0.001;
		}

		// move warrior(up & down)
		if (my_canvas.isKeyPressed(GLFW_KEY_W)) {
			warrior.center_.y += 0.001;
		}
		if (my_canvas.isKeyPressed(GLFW_KEY_S)) {
			warrior.center_.y -= 0.001;
		}

		// shoot a cannon ball
		if (my_canvas.isKeyPressed(GLFW_KEY_SPACE))
		{
			for(int i=0; i<3; i++)
			{
				if (bullet != nullptr) { delete bullet; }
				bullet = new MyBullet;
				bullet->center_ = tank.center_;
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

		tank.draw();
		warrior.draw(time);

		if (bullet != nullptr) bullet->draw();
		if (raser != nullptr) raser->draw();

		time += 1 / 600.0;
	}
	);

	return 0;
}
