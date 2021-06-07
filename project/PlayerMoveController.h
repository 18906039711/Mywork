#pragma once
#ifndef _PlayerMoveController_H_
#define _PlayerMoveController_H_
#include"cocos2d.h"
#include"Controller.h"
#include"player.h"
USING_NS_CC;

class Player;

//��ɫ�ƶ�������
class PlayerMoveController :public ControllerListener, public Node {
public:
	CREATE_FUNC(PlayerMoveController);
	virtual bool init();
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
	void update(float delta);
	void setSpeed(int speed);
	void getMap(TMXTiledMap* map);
	//�󶨾������
	void bindPlayer(Player* player);

private:
	int Speed;

	TMXTiledMap* my_map;
	TMXLayer* barrier;
	Player* my_player;

	//��ȡ��ͼ���ϰ���
	void setBarrierLater();

	//����������ת��Ϊ��ͼ��������
	Vec2 tileCoordForPosition(Vec2 point);
};

#endif

