#pragma once

#ifndef _PLAY_SCENE1_H_
#define _PLAY_SCENE1_H_

#include "cocos2d.h"
#include "ChooseScene.h"
#include "EnemyLayer.h"
#include"Merchant.h"
#include "AudioEngine.h"

USING_NS_CC;

class PlayScene1 :public Layer
{
public:
	CREATE_FUNC(PlayScene1);

	virtual bool init();

	static cocos2d::Scene* createScene();

	void update(float delta);

	void setMap();

	void setButton();

	void suspendCallback(cocos2d::Ref* pSender);

	void setMusic();

	void setTreasureChest();

	void displayCoinNum();

	void setFence();
	void removeFence();

	void setEnemyLayer();


	//void ifChestOpened(float dt);

	

private:
	TMXTiledMap* map = TMXTiledMap::create("map/PlayMap2.tmx");
	float mapWidth = (map->getMapSize().width) * map->getTileSize().width * 2;
	float mapHeight = (map->getMapSize().height) * map->getTileSize().height * 2;

	Vec2 mapPoint[9];

	Player* player;
	void addPlayer();
	int backGroundMusic1;

	//HP归零，游戏结束，返回选择界面
	void gameOver();
	void returnToChoose(float dt);
};

#endif _PLAY_SCENE1_H_
