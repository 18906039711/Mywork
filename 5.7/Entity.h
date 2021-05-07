#pragma once
#ifndef _Entity_H_
#define _Entity_H_
#include"cocos2d.h"
#include"ControllerListener.h"
USING_NS_CC;

class Entity : public ControllerListener {
public:
	//绑定精灵对象
	void bindSprite(Sprite* sprite);
	//设置控制器
	void setController(ControllerListener* controller);

protected:
	Sprite* my_sprite;
	ControllerListener* my_controller;

	TMXTiledMap* map;
	//检测碰撞的地图层
	TMXLayer* barrier;
	//将像素坐标转换为地图格子坐标
	Vec2 tileCoordForPosition(Vec2 point);
};

#endif
