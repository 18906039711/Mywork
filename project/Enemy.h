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
	void putIntoMap(TMXTiledMap* map);
	void setInformation();

	void randomMove(float delta);

	//�����ɫ������Χ����������Ϊ��������
	void locked();
	void unlocked();

	//��Ѫ
	void changeHP(int changeValue);

	
	int ID;
private:
	int damage;
	int HP;
	int Speed;

	TMXTiledMap* my_map;
	TMXLayer* barrier;

	//��ȡ��ͼ���ϰ���
	void setBarrierLater();

	//����������ת��Ϊ��ͼ��������
	Vec2 tileCoordForPosition(Vec2 point);

};
#endif