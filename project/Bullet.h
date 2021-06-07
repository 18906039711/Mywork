#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_
#include "cocos2d.h"
#include"Entity.h"
#include"ObjectTag.h"
#include "AudioEngine.h"
#include"Enemy.h"
#include"player.h"


USING_NS_CC;


class Bullet :public Entity {
public:
	//CREATE_FUNC( Bullet);
	virtual bool init();
	static Bullet* Bullet::create(int m_ID);
	void update(float delta);

	bool onContactBegin(PhysicsContact& contact);
	void setInformation();


	void putIntoMap(Vec2 point, float rotation);

private:
	int ID;
	int damage;
	float firingRate;

	TMXLayer* barrier;

	//��ȡ��ͼ���ϰ���
	void setBarrierLater();

	//����������ת��Ϊ��ͼ��������
	Vec2 tileCoordForPosition(Vec2 point);
};
#endif


