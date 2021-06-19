#pragma once
#ifndef _ENEMYLAYER_H_
#define _ENEMYLAYER_H_
#include "cocos2d.h"
#include "Entity.h"
#include "player.h"
#include "Enemy.h"
#include "PlayScene1.h"
#include "ObjectTag.h"


USING_NS_CC;

class EnemyLayer :public Entity {
public:
	CREATE_FUNC(EnemyLayer);
	virtual bool init();

	void update(float delta);

	void putIntoMap(TMXTiledMap* map);
	
	void createEnemies();

	//С���Ƿ�ȫ��
	bool cleanMark = false;
private:
	int enemyNum;
	int borderLength = 950;

	TMXTiledMap* my_map;
	TMXLayer* barrier;
	Player* player;

	Vector<Enemy*> enemyVec;

	//��ȡ��ͼ���ϰ���
	void setBarrierLayer();

	//����������ת��Ϊ��ͼ��������
	Vec2 tileCoordForPosition(Vec2 point);

};
#endif

