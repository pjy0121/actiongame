#pragma once
#include "MyHuman.h"

class MyBullet
{
public:
	vec3 center_;
	vec3 velocity_;

	// 총알 그리기
	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledCircle(RGBColors::gray, 0.01, 8);
		my_canvas.endTransformation();
	}

	// 총알이 날라가는 것을 연출하기 위해 위치 업데이트
	void update(const float& dt)
	{
		center_ += velocity_*dt;
	}
};