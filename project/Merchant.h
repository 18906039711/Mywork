#pragma once
#ifndef _MERCHANT_H_
#define _MERCHANT_H_
#include "cocos2d.h"
#include"Entity.h"
#include"player.h"
#include"ObjectTag.h"
#include<string>

USING_NS_CC;
class Player;

class Merchant :public Entity {
public:
	//CREATE_FUNC(Merchant);
	virtual bool init();
	static Merchant* create(int m_ID);

	void update(float delta);

	void putIntoMap(TMXTiledMap* map);

	//设置信息
	void setInformation();

	void showInfomation();
	void removeInfomation();

	//显示金额
	void showPrice();
	//记录对话程度
	bool priceMark = false;

	//交易并对话
	void deal();

	//生成恢复药水
	void givePotion();

	int ID;
	std::string name;

	//键盘事件监听
	EventListenerKeyboard* buyThing = EventListenerKeyboard::create();
private:
	int price;

};
#endif
