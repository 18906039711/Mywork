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

	float X = 0, Y = 0, px, py, width, height;

	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	px = my_player->getPosition().x;
	py = my_player->getPosition().y;

	//获得缩放后的长宽
	width = (my_player->showSprite())->getBoundingBox().size.width;
	height = (my_player->showSprite())->getBoundingBox().size.height;

	////获取上下左右四条边的中点
	//Vec2 tiledPos1 = tileCoordForPosition(Vec2(px + width / 2, py + height / 2));
	//Vec2 tiledPos2 = tileCoordForPosition(Vec2(px - width / 2, py - height / 2));
	//Vec2 tiledPos3 = tileCoordForPosition(Vec2(px - width / 2, py + height / 2));
	//Vec2 tiledPos4 = tileCoordForPosition(Vec2(px + width / 2, py - height / 2));

	////设置障碍层
	//setBarrierLater();

	////通过坐标获取TiledMap上格子的唯一标识
	//int tiledGid1 = barrier->getTileGIDAt(tiledPos1);
	//int tiledGid2 = barrier->getTileGIDAt(tiledPos2);
	//int tiledGid3 = barrier->getTileGIDAt(tiledPos3);
	//int tiledGid4 = barrier->getTileGIDAt(tiledPos4);

	////按键被按下，且精灵的边不在障碍层中
	//if (keyMap[up] && !tiledGid1 && !tiledGid3) {
	//	Y = Speed;
	//}
	//if (keyMap[left] && !tiledGid2 && !tiledGid3) {
	//	my_controllerListener->setScaleX(-1);
	//	X = -Speed;
	//}
	//if (keyMap[down] && !tiledGid2 && !tiledGid4) {
	//	Y = -Speed;
	//}
	//if (keyMap[right] && !tiledGid1 && !tiledGid4) {
	//	my_controllerListener->setScaleX(1);
	//	X = Speed;
	//}

	//Vec2 point = my_controllerListener->getPosition();
	//point.x += X;
	//point.y += Y;
	//my_controllerListener->setPosition(point.x, point.y);
	//获取上下左右四条边的中点
	Vec2 tiledPos_up = tileCoordForPosition(Vec2(px, py + height / 2));
	Vec2 tiledPos_down = tileCoordForPosition(Vec2(px, py - height / 2));
	Vec2 tiledPos_left = tileCoordForPosition(Vec2(px - width / 2, py));
	Vec2 tiledPos_right = tileCoordForPosition(Vec2(px + width / 2, py));

	//设置障碍层
	setBarrierLater();

	//通过坐标获取TiledMap上格子的唯一标识
	int tiledGid_up = barrier->getTileGIDAt(tiledPos_up);
	int tiledGid_down = barrier->getTileGIDAt(tiledPos_down);
	int tiledGid_left = barrier->getTileGIDAt(tiledPos_left);
	int tiledGid_right = barrier->getTileGIDAt(tiledPos_right);

	//按键被按下，且精灵的边不在障碍层中
	if (keyMap[up] && !tiledGid_up) {
		Y += Speed;
	}
	if (keyMap[left] && !tiledGid_left) {
		my_player->setScaleX(-1);
		X -= Speed;
	}
	if (keyMap[down] && !tiledGid_down) {
		Y -= Speed;
	}
	if (keyMap[right] && !tiledGid_right) {
		my_player->setScaleX(1);
		X += Speed;
	}

	Vec2 point = my_player->getPosition();
	point.x += X;
	point.y += Y;
	my_player->setPosition(point.x, point.y);
	
	//设置动作,判断按键是否改变
	/*char keyBoardNow[4] = { 0 };
	keyBoardNow[0] = keyMap[up];
	keyBoardNow[1] = keyMap[left];
	keyBoardNow[2] = keyMap[down];
	keyBoardNow[3] = keyMap[right];*/

	/*static char keyBoardOrgin[4] = { 0 };*/
	bool keyBoardNow = keyMap[up] + keyMap[left] + keyMap[down] + keyMap[right];
	static bool keyBoardOrgin = 0;

	if (keyBoardNow != keyBoardOrgin) {
		if (keyMap[up] || keyMap[left] || keyMap[down] || keyMap[right]) {
			my_player->stopPlayerAllActions();
			my_player->RunningAction();
		}
		else {
			my_player->stopPlayerAllActions();
			my_player->StandAction();
		}
	}
	keyBoardOrgin = keyBoardNow;
}

bool PlayerMoveController::ifKeyBoardChange(char keyBoardNow[4], char keyBoardOrgin[4]) {
	for (int i = 0; i < 4; i++) {
		if (keyBoardNow[i] != keyBoardOrgin[i]) {
			return true;
		}
	}
	return false;
}

void PlayerMoveController::setSpeed(int speed) {
	this->Speed = speed;
}

void PlayerMoveController::getMap(TMXTiledMap* map) {
	my_map = map;
}
void PlayerMoveController::setBarrierLater() {
	barrier = my_map->getLayer("barrier");
}

Vec2 PlayerMoveController::tileCoordForPosition(Vec2 point) {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 获取瓦片地图的尺寸(瓦片数量)
	auto mapTiledNum = my_map->getMapSize();
	///获取单个瓦片的尺寸(pixel)
	auto tiledSize = my_map->getTileSize();

	//x表示瓦片地图上第几个瓦片
	int x = int(point.x / (tiledSize.width * 2));
	int y = int(point.y / (tiledSize.height * 2));

	//cocos2dx与tiledmap坐标不同
	y = mapTiledNum.height - y - 1;

	//格子坐标从零开始
	return Vec2(x, y);
}

void PlayerMoveController::bindPlayer(Player* player) {
	my_player = player;
}