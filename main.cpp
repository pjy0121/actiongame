#pragma once

#include "MyHuman.h"
#include "MyWarrior.h"
#include "MyGunner.h"
#include "MyBullet.h"
#include "gl.h"

#define P1 true
#define P2 false

int is_attacked(vec3& a, vec3& b, vec3* attackedPos);	// 두 점의 좌표가 같은 지 판단하기(피격여부 판단) - Overloading
int is_attacked(vec3& a, vec3& b);	// 두 점의 좌표가 같은 지 판단하기(피격여부 판단)- Overloading
int is_close(vec3& a, vec3& b);		// 두 점이 가까이 있는지 판단
int menu();		// 게임 시작 메뉴
void guide();	// 게임 설명 출력

int main(int argc, char** argv)
{
	float time = 0.0;

	MyWarrior warrior(P1);
	MyGunner gunner(P2);
	MyBullet *bullet = nullptr;

	int select = 0;		// 캐릭터 설정 번호
	char bg[1];	// 배경 번호
	select = menu();

	if (select == 2)	// 2번을 선택 시 warrior가 2P, gunner가 1P
	{
		warrior.player_ = false; gunner.player_ = true;
	}

	/*
	cout << "배경을 선택해주세요.\n (1) 숲 (2) 바다 (3) 땅\n --> " << endl;
	cin >> bg;

	char* file_name = strcat(bg, ".bmp");
	*/

	my_canvas.show([&]
	{
		my_canvas.beginTransformation();
		my_canvas.translate(-1.35, -1.0);
		my_canvas.scale(0.008, 0.008);
		view_Display();
		my_canvas.scale(125, 125);
		my_canvas.translate(1.5, 1.0);
		my_canvas.endTransformation();

		/*
		// 배경 이미지 설정
		my_canvas.beginTransformation();
		my_canvas.translate(-0.83, -0.5);
		my_canvas.DoDisplay(file_name);
		my_canvas.translate(0.83, 0.5);
		my_canvas.endTransformation();
		*/

		warrior.keySettings();	// 키셋팅
		gunner.keySettings();	// 키셋팅
		
		// shoot a bullet
		if ((gunner.player_ && my_canvas.isKeyPressed(GLFW_KEY_R)) || (!gunner.player_ && my_canvas.isKeyPressed(GLFW_KEY_K)))
		{
			vec3 gp = gunner.gunPos_;
			vec3 wc = warrior.center_;
			if (gunner.player_)
				wc.x = -wc.x;
			else gp.x = -gp.x;
			if (is_close(gp, wc) && !gunner.dead_)	// 워리어와 근거리에 위치할 경우
			{
				double shot_damage = 0.1;
				if (warrior.defense_)	// 방어 모드
					shot_damage *= 0.2;
				if (bullet != nullptr) { vec3 bc = bullet->center_; warrior.bleeding(bc, time); }	// 피 흘리는 모션
				gunner.close_shot();	// 근거리 공격 후
				warrior.hp_ -= shot_damage;	// 워리어는 데미지를 입음
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
				bullet->update(1 / 80.0f);
			else bullet->update(-1 / 80.0f);
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

			if (!warrior.player_) wc.x = -wc.x; // warrior가 2P일 경우
			
			// gunner의 공격에 warrior가 피격
			if (is_attacked(wc, bullet->center_, attackedPos))
			{
				vec3 bc = bullet->center_;
				double bullet_damage = 20.0;
				bullet = nullptr;	// 맞으면 총알이 사라짐
				if (warrior.defense_) bullet_damage *= 0.2;	// 방어 모드 
				warrior.hp_ -= bullet_damage;	// hp감소

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
			double swing_damage = 0.5, pierce_damage = 0.1;
			// hp감소
			if (gunner.defense_)	// 방어 모드
			{
				swing_damage *= 0.2;
				pierce_damage *= 0.2;
			}
			if ((my_canvas.isKeyPressed(GLFW_KEY_T) && warrior.player_)|| (my_canvas.isKeyPressed(GLFW_KEY_L) && !warrior.player_))	// 찌르기일 경우
				gunner.hp_ -= pierce_damage;
			else gunner.hp_ -= swing_damage;	// 스윙(베기) 일 경우
			printf("거너 피격! - HP : %lf\n", gunner.hp_);
			gunner.bleeding(sp, time);	// 피를 흘리는 모션
		}

		if (warrior.hp_ <= 0) { warrior.dead_ = true; gunner.win(); }		// warrior 사망, gunner의 승리
		if (gunner.hp_ <= 0) { gunner.dead_ = true; warrior.win(); }	// gunner사망, warrior의 승리
		
		warrior.dash(time);		// warrior가 dash 스킬을 사용할 수 있도록 설정
		gunner.fadeaway(time);	// gunner가 fadeaway 스킬을 사용할 수 있도록 설정
		time += 1 / 150.0;
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
	if (b.x - 0.2 < a.x && a.x < b.x + 0.2 && b.y - 0.1 < a.y && a.y < b.y + 0.1)
		return 1;
	else return 0;
}

// 게임 시작 메뉴
int menu()
{
	int menu_select = 0, character_select = 0;
	cout << "(1) 게임 시작하기\t (2) 게임 가이드 보기" << endl;
	cout << ": ";
	cin >> menu_select;
	cout << "\n" << endl;

	if (menu_select == 2)
	{
		guide();
	}

	cout << "\n\n\n\n\n- 게임 시작 -\n" << endl;
	cout << "캐릭터 구성을 선택해주세요." << endl;
	cout << "(1) \t\t (2)" << endl;
	cout << "1P - 워리어 \t 1P - 거너" << endl;
	cout << "2P - 거너 \t 2P - 워리어" << endl;
	cout << ": ";
	cin >> character_select;

	return character_select;
}

// 게임 설명 출력
void guide()
{
	cout << "이 게임은 2인이 하나의 키보드를 가지고 플레이하는 대전게임입니다." << endl;
	cout << "각 플레이어는 워리어 / 거너 중 하나의 캐릭터로 플레이하게 됩니다." << endl;
	cout << "먼저 상대방의 HP 게이지를 없애는 플레이어가 승리합니다.\n\n" << endl;

	cout << "<캐릭터 조작>" << endl;
	cout << "● 1P의 키셋팅 \t| ○ 2P의 키셋팅" << endl;
	cout << "ㅁ 방향키 : " << endl;
	cout << "  w\t\t|  ↑" << endl;
	cout << "a s d  \t\t|←↓→\n" << endl;
	cout << "ㅁ 공격1 :\n  R\t\t| K" << endl;
	cout << "ㅁ 공격2 :\n  T\t\t| L\n" << endl;
	cout << "ㅁ 방어 : \n  CapsLock\t| Enter\n" << endl;
	cout << "ㅁ 특수 스킬 :\n  Left-Shift\t| Right-Shift\n\n" << endl;

	cout << "<기술>" << endl;
	cout << "ㅁ 워리어의 공격1 - 베기 : 칼을 위아래로 빠르게 휘두릅니다." << endl;
	cout << "ㅁ 워리어의 공격2 - 찌르기 : 칼을 창처럼 쥐고 돌진하여 찌릅니다.\n" << endl;
	cout << "ㅁ 거너의 공격1\n- 총알 발사(원거리) : 매우 빠른 총알 1발을 발사합니다." <<
		"\n- 밀어내기(근거리) : 적에게 데미지를 줌과 동시에 적과 자신을 약간 이격시킵니다." << endl;
	cout << "ㅁ 거너의 공격2 - 없음\n" << endl;
	cout << "ㅁ 방어(공통) : 방어 중에는 적의 공격에 20%의 데미지만을 입습니다.\n" << endl;

	cout << "캐릭터 위에 구슬이 돌고 있다면 특수 스킬 발동이 가능합니다." << endl;
	cout << "ㅁ 워리어의 특수 스킬(쿨타임 5초)\n- 대쉬(Dash) : 방향키와 함께 사용하여 해당 방향으로 빠르게 이동합니다." << endl;
	cout << "ㅁ 거너의 특수 스킬(쿨타임 7초)\n- 페이드어웨이(Fade-away) : 위 키 혹은 아래 키와 함께 사용하여 해당 방향 뒤쪽으로 멀리 도망칩니다.\n\n" << endl;

	cout << "<프로그램 제작자>" << endl;
	cout << "동국대학교 컴퓨터공학과 3학년 박지용" << endl;
	cout << "github주소\n: https://github.com/pjy0121/actiongame" << endl;
}