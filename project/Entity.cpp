#include"Entity.h"

void Entity::bindSprite(Sprite* sprite) {
	this->removeChildByTag(1);
	my_sprite = sprite;
	this->addChild(my_sprite, 0, 1);
}




