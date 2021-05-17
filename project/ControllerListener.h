#pragma once
#ifndef _Controller_Listener_H_
#define _Controller_Listener_H_
#include"cocos2d.h"
USING_NS_CC;

//¿ØÖÆÆ÷
class ControllerListener  {
public:
	virtual void setSimplePosition(int x, int y) = 0;
	virtual Vec2 getCurPosition() = 0;
};
#endif