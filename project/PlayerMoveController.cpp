#include"PlayerMoveController.h"
#include<string>
extern int player_num;

bool PlayerMoveController::init()
{
	this->Speed = 0;
	this->scheduleUpdate();
	//�����ƶ��¼�����
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

	if (!player_num) {  //���δѡ���޷��ƶ�
		return;
	}
	auto up = EventKeyboard::KeyCode::KEY_W;
	auto left = EventKeyboard::KeyCode::KEY_A;
	auto down = EventKeyboard::KeyCode::KEY_S;
	auto right = EventKeyboard::KeyCode::KEY_D;

	float X = 0, Y = 0, px, py, width, height;

	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	px = my_player->getPosition().x;
	py = my_player->getPosition().y;

	//������ź�ĳ���
	width = (my_player->showSprite())->getBoundingBox().size.width;
	height = (my_player->showSprite())->getBoundingBox().size.height;

	//��ȡ�������������ߵĵ�
	Vec2 tiledPos_up1 = tileCoordForPosition(Vec2(px - width / 2 + 10, py + height / 2));
	Vec2 tiledPos_up2 = tileCoordForPosition(Vec2(px + width / 2 - 10, py + height / 2));
	Vec2 tiledPos_down1 = tileCoordForPosition(Vec2(px - width / 2 + 10, py - height / 2));
	Vec2 tiledPos_down2 = tileCoordForPosition(Vec2(px + width / 2 - 10, py - height / 2));
	Vec2 tiledPos_left1 = tileCoordForPosition(Vec2(px - width / 2, py - height / 2 + 10));
	Vec2 tiledPos_left2 = tileCoordForPosition(Vec2(px - width / 2, py + height / 2 - 10));
	Vec2 tiledPos_right1 = tileCoordForPosition(Vec2(px + width / 2, py - height / 2 + 10));
	Vec2 tiledPos_right2 = tileCoordForPosition(Vec2(px + width / 2, py + height / 2 - 10));

	//�����ϰ���
	setBarrierLater();

	//ͨ�������ȡTiledMap�ϸ��ӵ�Ψһ��ʶ
	int tiledGid_up1 = barrier->getTileGIDAt(tiledPos_up1);
	int tiledGid_up2 = barrier->getTileGIDAt(tiledPos_up2);
	int tiledGid_down1 = barrier->getTileGIDAt(tiledPos_down1);
	int tiledGid_down2 = barrier->getTileGIDAt(tiledPos_down2);
	int tiledGid_left1 = barrier->getTileGIDAt(tiledPos_left1);
	int tiledGid_left2= barrier->getTileGIDAt(tiledPos_left2);
	int tiledGid_right1 = barrier->getTileGIDAt(tiledPos_right1);
	int tiledGid_right2 = barrier->getTileGIDAt(tiledPos_right2);

	//���������£��Ҿ���ı߲����ϰ�����
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

	//����ͬ��
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
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// ��ȡ��Ƭ��ͼ�ĳߴ�(��Ƭ����)
	auto mapTiledNum = my_map->getMapSize();
	///��ȡ������Ƭ�ĳߴ�(pixel)
	auto tiledSize = my_map->getTileSize();

	//x��ʾ��Ƭ��ͼ�ϵڼ�����Ƭ
	int x = int(point.x / (tiledSize.width * 2));
	int y = int(point.y / (tiledSize.height * 2));

	//cocos2dx��tiledmap���겻ͬ
	y = mapTiledNum.height - y - 1;

	//����������㿪ʼ
	return Vec2(x, y);
}

void PlayerMoveController::bindPlayer(Player* player) {
	my_player = player;
}