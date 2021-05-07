#pragma once
#ifndef _Entity_H_
#define _Entity_H_
#include"cocos2d.h"
#include"ControllerListener.h"
USING_NS_CC;

class Entity : public ControllerListener {
public:
	//�󶨾������
	void bindSprite(Sprite* sprite);
	//���ÿ�����
	void setController(ControllerListener* controller);

protected:
	Sprite* my_sprite;
	ControllerListener* my_controller;

	TMXTiledMap* map;
	//�����ײ�ĵ�ͼ��
	TMXLayer* barrier;
	//����������ת��Ϊ��ͼ��������
	Vec2 tileCoordForPosition(Vec2 point);
};

#endif
