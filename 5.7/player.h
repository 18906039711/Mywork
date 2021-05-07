#pragma once
#ifndef _Player_H_
#define _Player_H_
#include "cocos2d.h"
#include"Entity.h"
USING_NS_CC;

class Player :public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();
	static Player* setRanger();
	static Player* setSorcerer();
	

private:
	

};
#endif




