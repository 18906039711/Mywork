#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "cocos2d.h"
#include"Entity.h"
#include"player.h"
#include"ObjectTag.h"


USING_NS_CC;

class Enemy :public Entity {
public:
	CREATE_FUNC(Enemy);
	virtual bool init();
	static Enemy* create(int m_ID);

	void getMap(TMXTiledMap* map);
	void putIntoMap(TMXTiledMap* map, int tag = ObjectTag_Enemy);
	

	void setInformation();

	void randomMove(float delta);

	//进入角色搜索范围，被锁定成为攻击对象
	void locked();
	void unlocked();

	//扣血
	void changeHP(int changeValue);

	
	int ID;

	//敌人存活标记，用于地图障碍开关
	bool aliveMark = true;

private:
	int damage;
	int HP;
	int Speed;

	TMXTiledMap* my_map;
	TMXLayer* barrier;

	//获取地图的障碍层
	void setBarrierLayer();

	//将像素坐标转换为地图格子坐标
	Vec2 tileCoordForPosition(Vec2 point);

};
#endif
