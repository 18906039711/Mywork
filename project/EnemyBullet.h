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

	//���ӵ������ͼ��
	void putIntoMap(Vec2 point = Vec2::ZERO, float rotation = 0);

	//�ӵ�������һϵ�ж���
	void removeBullet();

private:
	int ID;
	int damage;

	TMXLayer* barrier;

	//��ȡ��ͼ���ϰ���
	void setBarrierLayer();

	//����������ת��Ϊ��ͼ��������
	Vec2 tileCoordForPosition(Vec2 point);
};
#endif


