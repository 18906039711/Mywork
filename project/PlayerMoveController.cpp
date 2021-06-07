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

	//获取上下左右四条边的点
	Vec2 tiledPos_up1 = tileCoordForPosition(Vec2(px - width / 2 + 10, py + height / 2));
	Vec2 tiledPos_up2 = tileCoordForPosition(Vec2(px + width / 2 - 10, py + height / 2));
	Vec2 tiledPos_down1 = tileCoordForPosition(Vec2(px - width / 2 + 10, py - height / 2));
	Vec2 tiledPos_down2 = tileCoordForPosition(Vec2(px + width / 2 - 10, py - height / 2));
	Vec2 tiledPos_left1 = tileCoordForPosition(Vec2(px - width / 2, py - height / 2 + 10));
	Vec2 tiledPos_left2 = tileCoordForPosition(Vec2(px - width / 2, py + height / 2 - 10));
	Vec2 tiledPos_right1 = tileCoordForPosition(Vec2(px + width / 2, py - height / 2 + 10));
	Vec2 tiledPos_right2 = tileCoordForPosition(Vec2(px + width / 2, py + height / 2 - 10));

	//设置障碍层
	setBarrierLater();

	//通过坐标获取TiledMap上格子的唯一标识
	int tiledGid_up1 = barrier->getTileGIDAt(tiledPos_up1);
	int tiledGid_up2 = barrier->getTileGIDAt(tiledPos_up2);
	int tiledGid_down1 = barrier->getTileGIDAt(tiledPos_down1);
	int tiledGid_down2 = barrier->getTileGIDAt(tiledPos_down2);
	int tiledGid_left1 = barrier->getTileGIDAt(tiledPos_left1);
	int tiledGid_left2= barrier->getTileGIDAt(tiledPos_left2);
	int tiledGid_right1 = barrier->getTileGIDAt(tiledPos_right1);
	int tiledGid_right2 = barrier->getTileGIDAt(tiledPos_right2);

	//按键被按下，且精灵的边不在障碍层中
	if (keyMap[up] && !tiledGid_up1 && !tiledGid_up2) {
		Y += Speed;
	}
	if (keyMap[left] && !tiledGid_left1 && !tiledGid_left2) {
		my_player->setScaleX(-1);
		X -= Speed;
	}
	if (keyMap[down] && !tiledGid_down1 && !tiledGid_down2) {
		Y -= Speed;
	}
	if (keyMap[right] && !tiledGid_right1 && !tiledGid_right2) {
		my_player->setScaleX(1);
		X += Speed;
	}

	//各向同速
	if (X != 0 && Y != 0) {
		X /= sqrt(2);
		Y /= sqrt(2);
	}

	Vec2 point = my_player->getPosition();
	point.x += X;
	point.y += Y;
	my_player->setPosition(point.x, point.y);
	
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