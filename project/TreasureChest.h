#pragma once
#ifndef _TreasureChest_H_
#define _TreasureChest_H_
#include "cocos2d.h"
#include"Entity.h"
#include"Coin.h"
#include"player.h"
#include"Weapon.h"
#include"ObjectTag.h"



USING_NS_CC;


class TreasureChest :public Entity {
public:
	CREATE_FUNC(TreasureChest);
	virtual bool init();
	static TreasureChest* create(int m_ID);

	bool onContactBegin(PhysicsContact& contact);

	void ifChestOpened(float dt);
	
private:
	bool ifOpened = false;
	int ID;
	
};
#endif
