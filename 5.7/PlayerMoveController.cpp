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

	float X = 0, Y = 0, px, py;

	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	px = my_controllerListener->getPosition().x;
	py = my_controllerListener->getPosition().y;

	//Vec2 tiledPos = tileCoordForPosition(Vec2(px, py));

	///* ��ȡ��ͼ���ӵ�Ψһ��ʶ */
	//int tiledGid = barrier->getTileGIDAt(tiledPos);

	///* ��Ϊ0���������������� */
	//if (tiledGid != 0) {
	//	/*
	//		��ȡ�õ�ͼ���ӵ��������ԣ�Ŀǰ����ֻ��һ��Collidable����,
	//		����������meta��ģ���ͬʱҲ������������ͼ�ģ������ڻ�ȡ���ӵ���������ʱ��
	//		ͨ������Ψһ��ʶ�ڵ�ͼ��ȡ�á�
	//	*/
	//	auto properties = map->getPropertiesForGID(tiledGid).asValueMap();
	//	/* 
	//	    ȡ�ø��ӵ�Collidable����ֵ 
	//	    �ж�Collidable�����Ƿ�Ϊtrue,�ǵĻ�����������ƶ� 
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