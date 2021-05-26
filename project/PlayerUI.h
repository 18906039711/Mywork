#pragma once
#ifndef _PlayerUI_H_
#define _PlayerUI_H_
#include "cocos2d.h"
#include"player.h"
#include"ui/CocosGUI.h"

USING_NS_CC;

extern int player_num;

class Player;

class PlayerUI :public Node{
public:
	CREATE_FUNC(PlayerUI);
	virtual bool init();
	void update(float delta);
	void bindPlayer(Player* player);
	void setUI();
	void updateUI();

private:
	Sprite* BottomFrameUI = Sprite::create("UI/BottomFrameUI.png");
	ui::LoadingBar* PlayerHPUI = ui::LoadingBar::create("UI/PlayerHPUI.png");
	ui::LoadingBar* PlayerDefendceUI = ui::LoadingBar::create("UI/PlayerDefendceUI.png");
	ui::LoadingBar* PlayerMPUI = ui::LoadingBar::create("UI/PlayerMPUI.png");

	Player* my_player;
	int HP = 0;
	int MP = 0;
	int Defendce = 0;
	int MaxHP = 0;
	int MaxMP = 0;
	int MaxDefendce = 0;
};
#endif
