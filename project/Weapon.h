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

	//碰撞开关
	void fireSwitch(bool mark);

	bool onContactBegin(PhysicsContact& contact);
	bool onContactPreSolve(PhysicsContact& contact);
	bool onContactSeparate(PhysicsContact& contact);

	void showInfomation();
	void removeInfomation();

	//设置当前武器数值
	void setInformation();

	//攻击
	void attack(float dt);
	bool attackMark = false;
	bool attackMark1 = false;


	int ID;
	std::string name;
private:
	int damage;
	int MPConsumption;
	//攻击间隔/s
	float firingRate;

	/* 碰撞检测 */
	EventListenerPhysicsContact* contactListenerWeapon = EventListenerPhysicsContact::create();

};
#endif
