#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "cocos2d.h"
#include"Entity.h"
#include"PlayerUI.h"
#include"Weapon.h"
#include"ObjectTag.h"

USING_NS_CC;

extern int playerID;

class PlayerUI;
class Weapon;

class Player :public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();

	//站立跑动动画
	void StandAction();
	void RunningAction();
	void stopPlayerAllActions();

	//设置面板属性
	void setPlayerAttribute();
	//初始化，包括补状态，清空武器信息，天赋等
	void initializePlayer();

	//改变玩家属性
	void changeHP(int changeValue);
	void changeMP(int changeValue);
	void changeDefendce(int changeValue);

	//监控玩家各值变化
	void updatePlayerAttribute();

	void update(float delta);

	//玩家移动有关
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
	void moving();
	void setSpeed(int speed);
	void getMap(TMXTiledMap* map);

	//捡道具、武器
	void pickUp(Weapon* weapon);

	//佩戴武器
	void getWeapon(Weapon* weapon);
	void getWeapon();
private:
	int maxHP = 0;
	int maxMP = 0;
	int maxDefendce = 0;
	int HP = 0;
	int MP = 0;
	int Defendce = 0;
	int Speed = 7;
	int weaponID = 0;
	friend class PlayerUI;

private:
	TMXTiledMap* my_map;
	TMXLayer* barrier;

	//获取地图的障碍层
	void setBarrierLater();

	//将像素坐标转换为地图格子坐标
	Vec2 tileCoordForPosition(Vec2 point);
};
#endif




