#pragma once
#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_
#include "cocos2d.h"
#include"Entity.h"
#include"ObjectTag.h"
#include "AudioEngine.h"
#include"Enemy.h"
#include"player.h"


USING_NS_CC;


class EnemyBullet :public Entity {
public:
	//CREATE_FUNC(Bullet);
	virtual bool init();
	static EnemyBullet* EnemyBullet::create(int m_ID);
	void update(float delta);

	bool onContactBegin(PhysicsContact& contact);
	void setInformation();

	//将子弹放入地图中
	void putIntoMap(Vec2 point = Vec2::ZERO, float rotation = 0);

	//子弹消除的一系列动作
	void removeBullet();

private:
	int ID;
	int damage;

	TMXLayer* barrier;

	//获取地图的障碍层
	void setBarrierLayer();

	//将像素坐标转换为地图格子坐标
	Vec2 tileCoordForPosition(Vec2 point);
};
#endif


