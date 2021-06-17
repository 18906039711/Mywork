#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "cocos2d.h"
#include"Entity.h"
#include"PlayerUI.h"
#include"Weapon.h"
#include"Potion.h"
#include"Enemy.h"
#include"ObjectTag.h"

USING_NS_CC;

extern int playerID;

class PlayerUI;
class Weapon;
class Potion;

class Player :public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();

	//վ���ܶ�����
	void StandAction();
	void RunningAction();
	void stopPlayerAllActions();

	//�����������
	void setPlayerAttribute();
	//��ʼ����������״̬�����������Ϣ���츳��
	void initializePlayer();

	//�ı��������
	void changeHP(int changeValue);
	void changeMP(int changeValue);
	void changeDefendce(int changeValue);

	//�����Ҹ�ֵ�仯
	void updatePlayerAttribute();

	void update(float delta);

	//����ƶ��й�
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;//��ż��̰�������
	void moving();
	void setSpeed(int speed);
	void putIntoMap(TMXTiledMap* map);

	//��ҩˮ
	void getPotion(Potion* potion);

	//�������
	void getWeapon(Weapon* weapon);
	void getWeapon();
	//���������Ƕ�
	void rotateWeapon(float rotation);

	//�жϷ�Χ���Ƿ��е���
	void searchEnemy();
	int searchingRadius = 700;

	//����
	void attack();

	//�����Զ��ظ�
	void recoverDefendce(float dt);

	//�ӽ��Ƿ�����ɫ
	bool playerFollowingMark = false;

	//HP���㣬��Ϸ����
	bool aliveMark = true;


private:


	int maxHP = 0;
	int maxMP = 0;
	int maxDefendce = 0;
	int HP = 0;
	int MP = 0;
	int Defendce = 0;
	int Speed = 7;
	int weaponID = 0;
	//��¼���׻ظ�ʱ��
	int recoverDefenceTime = 0;
	friend class PlayerUI;
	friend class Weapon;

private:
	//��Χ���е��˷�����
	bool enemyMark = false;
	TMXTiledMap* my_map;
	TMXLayer* barrier;

	//��ȡ��ͼ���ϰ���
	void setBarrierLayer();

	//����������ת��Ϊ��ͼ��������
	Vec2 tileCoordForPosition(Vec2 point);
};
#endif




