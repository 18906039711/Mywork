#pragma once

#ifndef _CHOOSE_SCENE_H_
#define _CHOOSE_SCENE_H_

#include "cocos2d.h"
#include "PlayScene1.h"
#include "SuspendScene.h"
#include "PlayerMoveController.h"
#include "player.h"
#include "PlayerUI.h"
#include "AudioEngine.h"

USING_NS_CC;

class Player;

class ChooseScene :public Scene
{
public:
	CREATE_FUNC(ChooseScene);

	virtual bool init();

	static cocos2d::Scene* createScene();

	void setButton();

	void suspendCallback(cocos2d::Ref* pSender);

	void setMusic();

	/*void update(float delta);

	void makeEddy();

	void enterEddy();

	void setButton();

	void suspendCallback(cocos2d::Ref* pSender);



	float pfh(float a, float b)
	{
		return a * a + b * b;
	}*/

private:
	Sprite* player_rangerwb = Sprite::create("character/rangerWB.png");
	Sprite* player_sorcererwb = Sprite::create("character/sorcererWB.png");
	TMXTiledMap* map = TMXTiledMap::create("map/chooseMap.tmx");
	void addPlayerUI(Player* player);
	void choosePlayer();
	void addPlayer();
	int backGroundMusic;
};


#endif //_CHOOSE_SCENE_H_