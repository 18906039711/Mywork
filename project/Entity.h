#pragma once
#ifndef _Entity_H_
#define _Entity_H_
#include"cocos2d.h"
#include"Controller.h"
USING_NS_CC;

class Entity : public ControllerListener {
public:
	//绑定精灵对象
	void bindSprite(Sprite* sprite);

	Sprite* showSprite() {
		return my_sprite;
	}

	//设置控制器
	void setController(ControllerListener* controller);

protected:
	Sprite* my_sprite;
	ControllerListener* my_controller;

};

#endif
