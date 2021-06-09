#pragma once
#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "cocos2d.h"
#include"Entity.h"
#include"player.h"
#include"ObjectTag.h"
#include"Bullet.h"
#include<string>


USING_NS_CC;
class Bullet;

class Weapon :public Entity {
public:
	//CREATE_FUNC(Weapon);
	virtual bool init();
	static Weapon* create(int m_ID);

	void update(float delta);

	//��ײ����
	void fireSwitch(bool mark);

	void putIntoMap(Vec2 point);

	void showInfomation();
	void removeInfomation();

	//���õ�ǰ������ֵ
	void setInformation();

	//����
	void attack(float dt);
	bool attackMark = false;
	bool attackMark1 = false;


	int ID;
	std::string name;
private:
	Sprite* fire;
	int damage;
	int MPConsumption;
	//�������/s
	float firingRate;


};
#endif
