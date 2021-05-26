#pragma once
#ifndef _Coin_H_
#define _Coin_H_
#include "cocos2d.h"
#include"Entity.h"
#include"ObjectTag.h"
#include "AudioEngine.h"

USING_NS_CC;


class Coin :public Entity {
public:
	//CREATE_FUNC(Coin);
	virtual bool init();
	static Coin* Coin::create();

private:
	bool ifdelete = false;
};
#endif


