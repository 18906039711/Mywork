#include"PlayerMoveController.h"
#include<string>
extern int player_num;

bool PlayerMoveController::init()
{
	this->Speed = 0;
	this->scheduleUpdate();
	//键盘移动事件监听
	EventListenerKeyboard* playerMove = EventListenerKeyboard::create();
	playerMove->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event)
	{
		keyMap[code] = true;
	};
	playerMove->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
		keyMap[code] = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(playerMove, this);
	return true;
}
void PlayerMoveController::update(float delta) {

	if (!player_num) {  //如果未选择无法移动
		return;
	}
	auto up = EventKeyboard::KeyCode::KEY_W;
	auto left = EventKeyboard::KeyCode::KEY_A;
	auto down = EventKeyboard::KeyCode::KEY_S;
	auto right = EventKeyboard::KeyCode::KEY_D;

	float X = 0, Y = 0, px, py;

	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	px = my_controllerListener->getPosition().x;
	py = my_controllerListener->getPosition().y;

	//Vec2 tiledPos = tileCoordForPosition(Vec2(px, py));

	///* 获取地图格子的唯一标识 */
	//int tiledGid = barrier->getTileGIDAt(tiledPos);

	///* 不为0，代表存在这个格子 */
	//if (tiledGid != 0) {
	//	/*
	//		获取该地图格子的所有属性，目前我们只有一个Collidable属性,
	//		格子是属于meta层的，但同时也是属于整个地图的，所以在获取格子的所有属性时，
	//		通过格子唯一标识在地图中取得。
	//	*/
	//	auto properties = map->getPropertiesForGID(tiledGid).asValueMap();
	//	/* 
	//	    取得格子的Collidable属性值 
	//	    判断Collidable属性是否为true,是的话，不让玩家移动 
	//	*/
	//	if (properties["Collidable"].asInt() == true) {
	//		return;
	//	}
	//}

	if (keyMap[up]) {
		Y = Speed;
	}
	if (keyMap[left]) {
		moving_mark = 1;
		my_controllerListener->setScaleX(-1);
		X = -Speed;
	}
	if (keyMap[down]) {
		Y = -Speed;
	}
	if (keyMap[right]) {
		moving_mark = 0;
		my_controllerListener->setScaleX(1);
		X = Speed;
	}
	Vec2 point = my_controllerListener->getPosition();
	point.x += X;
	point.y += Y;
	my_controllerListener->setPosition(point.x, point.y);
}

void PlayerMoveController::setSpeed(int speed) {
	this->Speed = speed;
}