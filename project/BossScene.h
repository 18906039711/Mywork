#pragma once

#ifndef _BOSSSCENE_H_
#define _BOSSSCENE_H_

#include "cocos2d.h"
#include "ChooseScene.h"
#include "PlayScene1.h"
#include "Enemy.h"
#include "ObjectTag.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"


USING_NS_CC;

class Player;
class Enemy;

class BossScene :public Layer
{
public:
	CREATE_FUNC(BossScene);

	virtual bool init();

	static cocos2d::Scene* createScene();

	void update(float delta);

	void setMap();

	void setButton();

	void suspendCallback(cocos2d::Ref* pSender);

	void setMusic();

	void displayCoinNum();

	void createBoss();
	void bossAppearance();

	void showBossHP();
	//void ifChestOpened(float dt);



private:
	TMXTiledMap* map = TMXTiledMap::create("map/BossMap.tmx");
	float mapWidth = (map->getMapSize().width) * map->getTileSize().width * 2;
	float mapHeight = (map->getMapSize().height) * map->getTileSize().height * 2;

	ui::LoadingBar* BossHP = ui::LoadingBar::create("UI/BossHpUI.png");
	Enemy* boss = Enemy::create(GoblinPriest);
	Player* player;
	void addPlayer(float delta);
	int backGroundMusic1;

	//HP归零，游戏结束，返回选择界面
	void gameOver();
	void returnToChoose(float dt);
};

#endif 
