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

	//������Ϣ
	void setInformation();

	void showInfomation();
	void removeInfomation();

	//��ʾ���
	void showPrice();
	//��¼�Ի��̶�
	bool priceMark = false;

	//���ײ��Ի�
	void deal();

	//���ɻָ�ҩˮ
	void givePotion();

	int ID;
	std::string name;

	//�����¼�����
	EventListenerKeyboard* buyThing = EventListenerKeyboard::create();
private:
	int price;

};
#endif
