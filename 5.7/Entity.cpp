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
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 获取瓦片地图的尺寸(瓦片数量)
	auto mapTiledNum = map->getMapSize();
	///获取单个瓦片的尺寸(pixel)
	auto tiledSize = map->getTileSize();

	//x表示瓦片地图上第几个瓦片
	int x = point.x / tiledSize.width;
	int y = point.y / tiledSize.height;

	//cocos2dx与tiledmap坐标不同
	y = mapTiledNum.height - y;

	//格子坐标从零开始
	return Vec2(x, y);
}