#pragma once
#ifndef _Player_H_
#define _Player_H_
#include "cocos2d.h"
#include"Entity.h"
#include"ChooseScene.h"
#include"PlayerUI.h"

USING_NS_CC;

extern int player_num;

class PlayerUI;

class Player :public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();

	static Player* setPlayer();
	void StandAction();
	void RunningAction();
	void stopPlayerAllActions();
	void setPlayerAttribute();

private:
	int HP = 0;
	int MP = 0;
	int Defendce = 0;
	friend class PlayerUI;
};
#endif




