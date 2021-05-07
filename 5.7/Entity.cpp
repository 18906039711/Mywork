#include"Entity.h"

void Entity::bindSprite(Sprite* sprite) {
	my_sprite = sprite;
	this->addChild(my_sprite);
}

void Entity::setController(ControllerListener* controller) {
	this->my_controller = controller;
	my_controller->setControllerListener(this);
}

Vec2 Entity::tileCoordForPosition(Vec2 point) {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// ��ȡ��Ƭ��ͼ�ĳߴ�(��Ƭ����)
	auto mapTiledNum = map->getMapSize();
	///��ȡ������Ƭ�ĳߴ�(pixel)
	auto tiledSize = map->getTileSize();

	//x��ʾ��Ƭ��ͼ�ϵڼ�����Ƭ
	int x = point.x / tiledSize.width;
	int y = point.y / tiledSize.height;

	//cocos2dx��tiledmap���겻ͬ
	y = mapTiledNum.height - y;

	//����������㿪ʼ
	return Vec2(x, y);
}