#pragma once
#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "cocos2d.h"
#include"Entity.h"
#include"player.h"
#include"ObjectTag.h"

USING_NS_CC;

class Weapon :public Entity {
public:
	CREATE_FUNC(Weapon);
	virtual bool init();
	static Weapon* create(int m_ID);

	bool onContactBegin(PhysicsContact& contact);
	bool onContactPreSolve(PhysicsContact& contact);
	bool onContactSeparate(PhysicsContact& contact);

	void showInfomation();
	void removeInfomation();

	int ID;
private:
	int damage;
	float firingRate;
	int MPConsumption;

};
#endif
