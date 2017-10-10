#pragma once

#include "MyHuman.h"
#include "MyWarrior.h"
#include "MyGunner.h"
#include "MyBullet.h"
#include "gl.h"

#define P1 true
#define P2 false

int is_attacked(vec3& a, vec3& b, vec3* attackedPos);	// �� ���� ��ǥ�� ���� �� �Ǵ��ϱ�(�ǰݿ��� �Ǵ�) - Overloading
int is_attacked(vec3& a, vec3& b);	// �� ���� ��ǥ�� ���� �� �Ǵ��ϱ�(�ǰݿ��� �Ǵ�)- Overloading
int is_close(vec3& a, vec3& b);		// �� ���� ������ �ִ��� �Ǵ�
int menu();		// ���� ���� �޴�
void guide();	// ���� ���� ���

int main(int argc, char** argv)
{
	float time = 0.0;

	MyWarrior warrior(P1);
	MyGunner gunner(P2);
	MyBullet *bullet = nullptr;

	int select = 0;		// ĳ���� ���� ��ȣ
	char bg[1];	// ��� ��ȣ
	select = menu();

	if (select == 2)	// 2���� ���� �� warrior�� 2P, gunner�� 1P
	{
		warrior.player_ = false; gunner.player_ = true;
	}

	/*
	cout << "����� �������ּ���.\n (1) �� (2) �ٴ� (3) ��\n --> " << endl;
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
		// ��� �̹��� ����
		my_canvas.beginTransformation();
		my_canvas.translate(-0.83, -0.5);
		my_canvas.DoDisplay(file_name);
		my_canvas.translate(0.83, 0.5);
		my_canvas.endTransformation();
		*/

		warrior.keySettings();	// Ű����
		gunner.keySettings();	// Ű����
		
		// shoot a bullet
		if ((gunner.player_ && my_canvas.isKeyPressed(GLFW_KEY_R)) || (!gunner.player_ && my_canvas.isKeyPressed(GLFW_KEY_K)))
		{
			vec3 gp = gunner.gunPos_;
			vec3 wc = warrior.center_;
			if (gunner.player_)
				wc.x = -wc.x;
			else gp.x = -gp.x;
			if (is_close(gp, wc) && !gunner.dead_)	// ������� �ٰŸ��� ��ġ�� ���
			{
				double shot_damage = 0.1;
				if (warrior.defense_)	// ��� ���
					shot_damage *= 0.2;
				if (bullet != nullptr) { vec3 bc = bullet->center_; warrior.bleeding(bc, time); }	// �� �긮�� ���
				gunner.close_shot();	// �ٰŸ� ���� ��
				warrior.hp_ -= shot_damage;	// ������� �������� ����
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
				bullet->update(1 / 80.0f);
			else bullet->update(-1 / 80.0f);
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

			if (!warrior.player_) wc.x = -wc.x; // warrior�� 2P�� ���
			
			// gunner�� ���ݿ� warrior�� �ǰ�
			if (is_attacked(wc, bullet->center_, attackedPos))
			{
				vec3 bc = bullet->center_;
				double bullet_damage = 20.0;
				bullet = nullptr;	// ������ �Ѿ��� �����
				if (warrior.defense_) bullet_damage *= 0.2;	// ��� ��� 
				warrior.hp_ -= bullet_damage;	// hp����

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
			double swing_damage = 0.5, pierce_damage = 0.1;
			// hp����
			if (gunner.defense_)	// ��� ���
			{
				swing_damage *= 0.2;
				pierce_damage *= 0.2;
			}
			if ((my_canvas.isKeyPressed(GLFW_KEY_T) && warrior.player_)|| (my_canvas.isKeyPressed(GLFW_KEY_L) && !warrior.player_))	// ����� ���
				gunner.hp_ -= pierce_damage;
			else gunner.hp_ -= swing_damage;	// ����(����) �� ���
			printf("�ų� �ǰ�! - HP : %lf\n", gunner.hp_);
			gunner.bleeding(sp, time);	// �Ǹ� �긮�� ���
		}

		if (warrior.hp_ <= 0) { warrior.dead_ = true; gunner.win(); }		// warrior ���, gunner�� �¸�
		if (gunner.hp_ <= 0) { gunner.dead_ = true; warrior.win(); }	// gunner���, warrior�� �¸�
		
		warrior.dash(time);		// warrior�� dash ��ų�� ����� �� �ֵ��� ����
		gunner.fadeaway(time);	// gunner�� fadeaway ��ų�� ����� �� �ֵ��� ����
		time += 1 / 150.0;
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
	if (b.x - 0.2 < a.x && a.x < b.x + 0.2 && b.y - 0.1 < a.y && a.y < b.y + 0.1)
		return 1;
	else return 0;
}

// ���� ���� �޴�
int menu()
{
	int menu_select = 0, character_select = 0;
	cout << "(1) ���� �����ϱ�\t (2) ���� ���̵� ����" << endl;
	cout << ": ";
	cin >> menu_select;
	cout << "\n" << endl;

	if (menu_select == 2)
	{
		guide();
	}

	cout << "\n\n\n\n\n- ���� ���� -\n" << endl;
	cout << "ĳ���� ������ �������ּ���." << endl;
	cout << "(1) \t\t (2)" << endl;
	cout << "1P - ������ \t 1P - �ų�" << endl;
	cout << "2P - �ų� \t 2P - ������" << endl;
	cout << ": ";
	cin >> character_select;

	return character_select;
}

// ���� ���� ���
void guide()
{
	cout << "�� ������ 2���� �ϳ��� Ű���带 ������ �÷����ϴ� ���������Դϴ�." << endl;
	cout << "�� �÷��̾�� ������ / �ų� �� �ϳ��� ĳ���ͷ� �÷����ϰ� �˴ϴ�." << endl;
	cout << "���� ������ HP �������� ���ִ� �÷��̾ �¸��մϴ�.\n\n" << endl;

	cout << "<ĳ���� ����>" << endl;
	cout << "�� 1P�� Ű���� \t| �� 2P�� Ű����" << endl;
	cout << "�� ����Ű : " << endl;
	cout << "  w\t\t|  ��" << endl;
	cout << "a s d  \t\t|����\n" << endl;
	cout << "�� ����1 :\n  R\t\t| K" << endl;
	cout << "�� ����2 :\n  T\t\t| L\n" << endl;
	cout << "�� ��� : \n  CapsLock\t| Enter\n" << endl;
	cout << "�� Ư�� ��ų :\n  Left-Shift\t| Right-Shift\n\n" << endl;

	cout << "<���>" << endl;
	cout << "�� �������� ����1 - ���� : Į�� ���Ʒ��� ������ �ֵθ��ϴ�." << endl;
	cout << "�� �������� ����2 - ��� : Į�� âó�� ��� �����Ͽ� ��ϴ�.\n" << endl;
	cout << "�� �ų��� ����1\n- �Ѿ� �߻�(���Ÿ�) : �ſ� ���� �Ѿ� 1���� �߻��մϴ�." <<
		"\n- �о��(�ٰŸ�) : ������ �������� �ܰ� ���ÿ� ���� �ڽ��� �ణ �̰ݽ�ŵ�ϴ�." << endl;
	cout << "�� �ų��� ����2 - ����\n" << endl;
	cout << "�� ���(����) : ��� �߿��� ���� ���ݿ� 20%�� ���������� �Խ��ϴ�.\n" << endl;

	cout << "ĳ���� ���� ������ ���� �ִٸ� Ư�� ��ų �ߵ��� �����մϴ�." << endl;
	cout << "�� �������� Ư�� ��ų(��Ÿ�� 5��)\n- �뽬(Dash) : ����Ű�� �Բ� ����Ͽ� �ش� �������� ������ �̵��մϴ�." << endl;
	cout << "�� �ų��� Ư�� ��ų(��Ÿ�� 7��)\n- ���̵�����(Fade-away) : �� Ű Ȥ�� �Ʒ� Ű�� �Բ� ����Ͽ� �ش� ���� �������� �ָ� ����Ĩ�ϴ�.\n\n" << endl;

	cout << "<���α׷� ������>" << endl;
	cout << "�������б� ��ǻ�Ͱ��а� 3�г� ������" << endl;
	cout << "github�ּ�\n: https://github.com/pjy0121/actiongame" << endl;
}