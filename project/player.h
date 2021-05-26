#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "cocos2d.h"
#include"Entity.h"
#include"PlayerUI.h"
#include"ObjectTag.h"

USING_NS_CC;

extern int player_num;

class PlayerUI;

class Player :public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();

	//站立跑动动画
	void StandAction();
	void RunningAction();
	void stopPlayerAllActions();

	//设置初始面板属性
	void setPlayerAttribute();

	//改变玩家属性
	void changeHP(int changeValue);
	void changeMP(int changeValue);
	void changeDefendce(int changeValue);

	//监控玩家各值变化
	//void update(float delta);
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
	void update(float delta);
	void setSpeed(int speed);
	void getMap(TMXTiledMap* map);
private:
	int maxHP = 0;
	int maxMP = 0;
	int maxDefendce = 0;
	int HP = 0;
	int MP = 0;
	int Defendce = 0;
	friend class PlayerUI;
private:
	int Speed;
	TMXTiledMap* my_map;
	TMXLayer* barrier;
	//获取地图的障碍层
	void setBarrierLater();

	//将像素坐标转换为地图格子坐标
	Vec2 tileCoordForPosition(Vec2 point);
};
#endif




