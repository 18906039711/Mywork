#pragma once
#ifndef _PlayerMoveController_H_
#define _PlayerMoveController_H_
#include"cocos2d.h"
#include"ControllerListener.h"
USING_NS_CC;

//½ÇÉ«ÒÆ¶¯¿ØÖÆÆ÷
class PlayerMoveController :public ControllerListener {
public:
	CREATE_FUNC(PlayerMoveController);
	virtual bool init();
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
	void update(float delta);
	void setSpeed(int speed);
private:
	int Speed;
	int moving_mark = 0;
};

#endif

