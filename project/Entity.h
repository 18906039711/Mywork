#pragma once
#ifndef _Entity_H_
#define _Entity_H_
#include"cocos2d.h"

USING_NS_CC;

class Entity : public Node {
public:

	//�󶨾������
	void bindSprite(Sprite* sprite);

	Sprite* showSprite() {
		return my_sprite;
	}

	void putIntoMap();
	TMXTiledMap* my_map;

protected:
	Sprite* my_sprite;

};

#endif
