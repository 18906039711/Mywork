#pragma once
#ifndef _PlayerMoveController_H_
#define _PlayerMoveController_H_
#include"cocos2d.h"
#include"Controller.h"
#include"player.h"
USING_NS_CC;

class Player;

//角色移动控制器
class PlayerMoveController :public ControllerListener, public Node {
public:
	CREATE_FUNC(PlayerMoveController);
	virtual bool init();
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
	void update(float delta);
	void setSpeed(int speed);
	void getMap(TMXTiledMap* map);
	//绑定精灵对象
	void bindPlayer(Player* player);

private:
	int Speed;

	TMXTiledMap* my_map;
	TMXLayer* barrier;
	Player* my_player;

	//获取地图的障碍层
	void setBarrierLater();

	//将像素坐标转换为地图格子坐标
	Vec2 tileCoordForPosition(Vec2 point);
};

#endif

