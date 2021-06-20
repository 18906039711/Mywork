#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "cocos2d.h"
#include"Entity.h"
#include"player.h"
#include"EnemyBullet.h"
#include"Coin.h"
#include"ObjectTag.h"


USING_NS_CC;

class Enemy :public Entity {
public:
	CREATE_FUNC(Enemy);
	virtual bool init();
	static Enemy* create(int m_ID);

	//用于搜索角色
	void update(float delta);
	int searchingRadius = 1000;

	void getMap(TMXTiledMap* map);
	void putIntoMap(TMXTiledMap* map, int tag = ObjectTag_Enemy);
	
	void setInformation();

	//出场站立攻击动画
	void appearAction();
	void standAction();
	void attackAction();

	//随机移动
	void randomMove(float delta);
	//朝玩家移动
	void towardsPlayerMove(float delta);
	//攻击
	void attack();
	void bossAttack();
	void thunderAttack(float dt);

	//进入角色搜索范围，被锁定成为攻击对象
	void locked();
	void unlocked();

	//扣血
	void changeHP(int changeValue);
	int getHP();

	int ID;

	//敌人存活标记，用于地图障碍开关
	bool aliveMark = true;
	//困难标记
	bool hardMark = UserDefault::getInstance()->getBoolForKey("hardMark", false);\

protected:
	int damage;
	int HP;
	int Speed;

	//原始宽度，用来设置标记圆圈的大小
	float enemyWidth;
	float enemyHeight;

	TMXTiledMap* my_map;
	TMXLayer* barrier;

	//获取地图的障碍层
	void setBarrierLayer();

	//将像素坐标转换为地图格子坐标
	Vec2 tileCoordForPosition(Vec2 point);

};
#endif
