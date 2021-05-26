#pragma once
#ifndef _CoinUI_H_
#define _CoinUI_H_
#include "cocos2d.h"
#include"coin.h"

USING_NS_CC;


class Coin;

class CoinUI :public Node {
public:
	CREATE_FUNC(CoinUI);
	virtual bool init();
	void update(float delta);
	void setUI();
	void intializeCoin();

private:
	Sprite* CoinBackGround = Sprite::create("Coin/CoinBackGround.png");
	int coinNum;
};
#endif
