#pragma once
#include "MyHuman.h"

class MyBullet
{
public:
	vec3 center_;
	vec3 velocity_;

	// �Ѿ� �׸���
	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledCircle(RGBColors::gray, 0.01, 8);
		my_canvas.endTransformation();
	}

	// �Ѿ��� ���󰡴� ���� �����ϱ� ���� ��ġ ������Ʈ
	void update(const float& dt)
	{
		center_ += velocity_*dt;
	}
};