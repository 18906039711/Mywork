#include"player.h"

bool Player::init()
{
	this->scheduleUpdate();

	//设置Tag
	this->setTag(ObjectTag_Player);

	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite* playerSprite = Sprite::create();
	
	if (player_num == 1) {
		playerSprite = Sprite::create("character/ranger_right.png");
		playerSprite->setScale(0.2f);
		//绑定精灵图像
		this->bindSprite(playerSprite);
		this->setPosition(Vec2(visibleSize.width / 4 * 3, visibleSize.height / 3 + origin.y));
	}
	else if (player_num == 2) {
		 playerSprite = Sprite::create("character/sorcerer_right.png");
		playerSprite->setScale(0.2f);
		this->bindSprite(playerSprite);
		this->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 3 + origin.y));
	}
	//设置站立动画
	this->StandAction();
	//设置初始面板属性
	this->setPlayerAttribute();
	//设置移动速度
	this->setSpeed(8);

	//设置物理刚体
	auto body = PhysicsBody::createBox(playerSprite->getBoundingBox().size);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setDynamic(false);
	this->addComponent(body);

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

void Player::StandAction() {
	//创建序列帧
	auto animation = Animation::create();

	if (player_num == 1) {
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/ranger%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	else if (player_num == 2) {
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/sorcerer%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	//设置两帧间的时间间隔
	animation->setDelayPerUnit(0.3f);
	//设置循环，-1无限循环
	animation->setLoops(-1);
	//在最后一帧播放完恢复到第一帧
	animation->setRestoreOriginalFrame(true);

	my_sprite->runAction(Animate::create(animation));
}

void Player::RunningAction() {
	auto animation = Animation::create();
	if (player_num == 1) {
		for (int i = 1; i <= 4; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/ranger/run%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	animation->setDelayPerUnit(0.3f);
	animation->setLoops(-1);
	animation->setRestoreOriginalFrame(true);
	my_sprite->runAction(Animate::create(animation));
}

void Player::stopPlayerAllActions() {
	my_sprite->stopAllActions();
}

//设置初始面板属性
void Player::setPlayerAttribute() {
	if (player_num == 1) {
		maxHP = UserDefault::getInstance()->getIntegerForKey("RangerHP", 6);
		maxDefendce = UserDefault::getInstance()->getIntegerForKey("RangerDefendce", 3);
		maxMP = UserDefault::getInstance()->getIntegerForKey("RangerMP", 180);
	}
	else if (player_num == 2) {
		maxHP = UserDefault::getInstance()->getIntegerForKey("sorcererHP", 3);
		maxDefendce = UserDefault::getInstance()->getIntegerForKey("sorcererDefendce", 5);
		maxMP = UserDefault::getInstance()->getIntegerForKey("sorcererMP", 240);
	}
	UserDefault::getInstance()->setIntegerForKey("PlayerHP", maxHP);
	UserDefault::getInstance()->setIntegerForKey("PlayerDefendce", maxDefendce);
	UserDefault::getInstance()->setIntegerForKey("PlayerMP", maxMP);

	HP = UserDefault::getInstance()->getIntegerForKey("PlayerHP");
	MP = UserDefault::getInstance()->getIntegerForKey("PlayerMP");
	Defendce = UserDefault::getInstance()->getIntegerForKey("PlayerDefendce");
}

void Player::changeHP(int changeValue) {
	if (HP + changeValue < 0) {
		UserDefault::getInstance()->setIntegerForKey("PlayerHP", 0);
		HP = 0;
	}
	else if (HP + changeValue > maxHP) {
		UserDefault::getInstance()->setIntegerForKey("PlayerHP", maxHP);
		HP = maxHP;
	}
	else {
		UserDefault::getInstance()->setIntegerForKey("PlayerHP", HP + changeValue);
		HP += changeValue;
	}
}
void Player::changeMP(int changeValue) {
	if (MP + changeValue < 0) {
		UserDefault::getInstance()->setIntegerForKey("PlayerMP", 0);
		MP = 0;
	}
	else if (MP + changeValue > maxMP) {
		UserDefault::getInstance()->setIntegerForKey("PlayerMP", maxMP);
		MP = maxMP;
	}
	else {
		UserDefault::getInstance()->setIntegerForKey("PlayerMP", MP + changeValue);
		MP += changeValue;
	}

}
void Player::changeDefendce(int changeValue) {
	
	if (Defendce + changeValue < 0) {
		UserDefault::getInstance()->setIntegerForKey("Player Defendce", 0);
		Defendce = 0;
	}
	else if (Defendce + changeValue > maxHP) {
		UserDefault::getInstance()->setIntegerForKey("Player Defendce", maxDefendce);
		Defendce = maxDefendce;
	}
	else {
		UserDefault::getInstance()->setIntegerForKey("PlayerDefendce", Defendce + changeValue);
		Defendce += changeValue;
	}
	
}

void  Player::update(float delta) {

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
	px = this->getPosition().x;
	py = this->getPosition().y;

	//获得缩放后的长宽
	width = (this->showSprite())->getBoundingBox().size.width;
	height = (this->showSprite())->getBoundingBox().size.height;

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
	int tiledGid_left2 = barrier->getTileGIDAt(tiledPos_left2);
	int tiledGid_right1 = barrier->getTileGIDAt(tiledPos_right1);
	int tiledGid_right2 = barrier->getTileGIDAt(tiledPos_right2);

	//按键被按下，且精灵的边不在障碍层中
	if (keyMap[up] && !tiledGid_up1 && !tiledGid_up2) {
		Y += Speed;
	}
	if (keyMap[left] && !tiledGid_left1 && !tiledGid_left2) {
		this->setScaleX(-1);
		X -= Speed;
	}
	if (keyMap[down] && !tiledGid_down1 && !tiledGid_down2) {
		Y -= Speed;
	}
	if (keyMap[right] && !tiledGid_right1 && !tiledGid_right2) {
		this->setScaleX(1);
		X += Speed;
	}

	//各向同速
	if (X != 0 && Y != 0) {
		X /= sqrt(2);
		Y /= sqrt(2);
	}

	Vec2 point = this->getPosition();
	point.x += X;
	point.y += Y;
	this->setPosition(point.x, point.y);

	bool keyBoardNow = keyMap[up] + keyMap[left] + keyMap[down] + keyMap[right];
	static bool keyBoardOrgin = 0;

	if (keyBoardNow != keyBoardOrgin) {
		if (keyMap[up] || keyMap[left] || keyMap[down] || keyMap[right]) {
			this->stopPlayerAllActions();
			this->RunningAction();
		}
		else {
			this->stopPlayerAllActions();
			this->StandAction();
		}
	}
	keyBoardOrgin = keyBoardNow;
}

void  Player::setSpeed(int speed) {
	this->Speed = speed;
}

void Player::getMap(TMXTiledMap* map) {
	my_map = map;
}

void Player::setBarrierLater() {
	barrier = my_map->getLayer("barrier");
}

Vec2  Player::tileCoordForPosition(Vec2 point) {
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