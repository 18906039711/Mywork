#include"Entity.h"

void Entity::bindSprite(Sprite* sprite) {
	my_sprite = sprite;
	this->addChild(my_sprite);
}

void Entity::setController(ControllerListener* controller) {
	this->my_controller = controller;
	my_controller->setControllerListener(this);
}

