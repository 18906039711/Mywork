#pragma once
#ifndef _POTION_H_
#define _POTION_H_
#include "cocos2d.h"
#include"Entity.h"
#include"player.h"
#include"ObjectTag.h"
#include<string>

USING_NS_CC;
class Player;

class Potion :public Entity {
public:
	//CREATE_FUNC(Potion);
	virtual bool init();
	static Potion* create(int m_ID);

	void update(float delta);

	void putIntoMap(TMXTiledMap* map);

	void showInfomation();
	void removeInfomation();

	//设置当前回复数值
	void setInformation();

	void recover(Player* player);

	int ID;
	std::string name;
private:
	int HPRecoveryVolume;
	int MPRecoveryVolume;

};
#endif
