#pragma once

#ifndef _PLAY_SCENE1_H_
#define _PLAY_SCENE1_H_

#include "cocos2d.h"
#include "SuspendScene.h"
#include "player.h"
#include "PlayerUI.h"
#include "CoinUI.h"
#include"TreasureChest.h"
#include "AudioEngine.h"

USING_NS_CC;

class PlayScene1 :public Layer
{
public:
	CREATE_FUNC(PlayScene1);

	virtual bool init();

	static cocos2d::Scene* createScene();

	void setButton();

	void suspendCallback(cocos2d::Ref* pSender);

	void setMusic();

	void setTreasureChest();

	void displayCoinNum();

	//void ifChestOpened(float dt);

	//void update(float delta);

private:
	TMXTiledMap* map = TMXTiledMap::create("map/chooseMap.tmx");

	Player* player;
	void addPlayer();
	int backGroundMusic1;
};

#endif _PLAY_SCENE1_H_
