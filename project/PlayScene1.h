#pragma once

#ifndef _PLAY_SCENE1_H_
#define _PLAY_SCENE1_H_

#include "cocos2d.h"
#include"SuspendScene.h"

USING_NS_CC;

class PlayScene1 :public Scene
{
public:
	CREATE_FUNC(PlayScene1);

	virtual bool init();

	static cocos2d::Scene* createScene();

	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;

	void update(float delta);

	void setPlayer();

	void setButton();

	void suspendCallback(cocos2d::Ref* pSender);

	void MusicCallback(cocos2d::Ref* pSender);

private:
	Sprite* player_right;
	Sprite* player_left;
};

#endif _PLAY_SCENE1_H_
