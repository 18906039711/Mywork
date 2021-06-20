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

	//����������ɫ
	void update(float delta);
	int searchingRadius = 1000;

	void getMap(TMXTiledMap* map);
	void putIntoMap(TMXTiledMap* map, int tag = ObjectTag_Enemy);
	
	void setInformation();

	//����վ����������
	void appearAction();
	void standAction();
	void attackAction();

	//����ƶ�
	void randomMove(float delta);
	//������ƶ�
	void towardsPlayerMove(float delta);
	//����
	void attack();
	void bossAttack();
	void thunderAttack(float dt);

	//�����ɫ������Χ����������Ϊ��������
	void locked();
	void unlocked();

	//��Ѫ
	void changeHP(int changeValue);
	int getHP();

	int ID;

	//���˴���ǣ����ڵ�ͼ�ϰ�����
	bool aliveMark = true;
	//���ѱ��
	bool hardMark = UserDefault::getInstance()->getBoolForKey("hardMark", false);\

protected:
	int damage;
	int HP;
	int Speed;

	//ԭʼ��ȣ��������ñ��ԲȦ�Ĵ�С
	float enemyWidth;
	float enemyHeight;

	TMXTiledMap* my_map;
	TMXLayer* barrier;

	//��ȡ��ͼ���ϰ���
	void setBarrierLayer();

	//����������ת��Ϊ��ͼ��������
	Vec2 tileCoordForPosition(Vec2 point);

};
#endif
