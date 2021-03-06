#pragma once

#ifndef _CHOOSE_SCENE_H_
#define _CHOOSE_SCENE_H_

#include "cocos2d.h"
#include "PlayScene1.h"
#include "SuspendScene.h"
#include "player.h"
#include "PlayerUI.h"
#include "CoinUI.h"
#include"TreasureChest.h"
#include"Weapon.h"
#include"Enemy.h"
#include "ObjectTag.h"
#include "AudioEngine.h"

USING_NS_CC;

class Player;

class ChooseScene :public Layer
{
public:
	CREATE_FUNC(ChooseScene);

	virtual bool init();

	static cocos2d::Scene* createScene();

	void setMap();

	void setButton();

	void suspendCallback(cocos2d::Ref* pSender);

	void setMusic();

	void setTreasureChest();

	void displayCoinNum();

	void setEnemy();

	void setDoor();

	void update(float delta);

	/*void makeEddy();

	void enterEddy();

	*/

private:
	Sprite* player_rangerwb = Sprite::create("character/rangerWB.png");
	Sprite* player_sorcererwb = Sprite::create("character/sorcererWB.png");
	TMXTiledMap* map = TMXTiledMap::create("map/ChooseMap.tmx"); 
	float mapWidth = (map->getMapSize().width) * map->getTileSize().width * 2;
	float mapHeight = (map->getMapSize().height) * map->getTileSize().height * 2;

	Player* player;

	void choosePlayer();
	void addPlayer();
	int backGroundMusic;

	//HP归零，游戏结束，返回选择界面
	void gameOver();
	void returnToChoose(float dt);
};


#endif //_CHOOSE_SCENE_H_