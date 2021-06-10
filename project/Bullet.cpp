#include"Bullet.h"

bool Bullet::init()
{
	
	//设置Tag
	this->setTag(ObjectTag_Bullet);

	std::string bulletID = "Weapon/" + std::to_string(ID) + "/Bullet.png";
	Sprite* bulletSprite = Sprite::create(bulletID);
	bulletSprite->setScale(static_cast<float>(0.5));
	this->bindSprite(bulletSprite);
	setInformation();

	std::string bulletBackgroundID = "Weapon/" + std::to_string(ID) + "/BulletBackground.png";
	Sprite* bulletBackgroundSprite = Sprite::create(bulletBackgroundID);
	if (bulletBackgroundSprite != nullptr) {
		bulletBackgroundSprite->setOpacity(100);
		this->addChild(bulletBackgroundSprite,0, ObjectTag_BulletBackground);
	}



	//auto soundEffect = AudioEngine::play2d("Music/eatCoin.mp3", false);
	////读取之前的音量
	//int volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
	//AudioEngine::setVolume(soundEffect, volumePercent / 100.f);
	PhysicsBody* body;
	if (ID < SwordID) {
		body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
	}
	else {
		Vec2 points[5] = { Vec2(-my_sprite->getBoundingBox().size.width / 2, my_sprite->getBoundingBox().size.height / 2),
		Vec2(0, my_sprite->getBoundingBox().size.height / 2),
		Vec2(my_sprite->getBoundingBox().size.width / 2,  0),
		Vec2(0, -my_sprite->getBoundingBox().size.height / 2),
		Vec2(-my_sprite->getBoundingBox().size.width / 2, -my_sprite->getBoundingBox().size.height / 2) };
		body = PhysicsBody::createPolygon(points, 5);
	}
	
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	this->addComponent(body);


	

	return true;
}

Bullet* Bullet::create(int m_ID) {
	auto bullet = new Bullet();
	bullet->ID = m_ID;

	if (bullet && bullet->init()) {
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}
void Bullet::update(float delta) {
	Enemy* enemy = dynamic_cast<Enemy*>(my_map->getChildByTag(ObjectTag_Enemy));
	Vec2 tiledPos = tileCoordForPosition(this->getPosition());
	if (barrier->getTileGIDAt(tiledPos)) {
		this->removeBullet();
	}
}

bool Bullet::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	

	//绑定碰撞的子弹和敌人
	Bullet* bullet = dynamic_cast<Bullet*>(nodeA);
	Enemy* enemy = dynamic_cast<Enemy*>(nodeB);

	//防止节点为nullptr闪退
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//子弹碰到敌人
	if (nodeA->getTag() == ObjectTag_Bullet && nodeB->getTag() == ObjectTag_Enemy) {
		enemy->changeHP(-damage);
		//射击武器
		if (bullet->ID < SwordID) {
			bullet->removeBullet();
		}
	}
	return true;
}

void Bullet::removeBullet() {
	//要先移除物理刚体，否则会出现多次伤害的情况
	this->removeAllComponents();
	this->my_sprite->setVisible(false);
	this->stopAllActions();
	this->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.15)), RemoveSelf::create(), NULL));

	//创建子弹撞击动画
	auto animation = Animation::create();

	for (int i = 1; i <= 3; i++) {
		char nameSize[100] = { 0 };
		sprintf(nameSize, "Weapon/%d/removeAction%d.png", ID, i);
		animation->addSpriteFrameWithFile(nameSize);
	}

	//设置两帧间的时间间隔
	animation->setDelayPerUnit(static_cast<float>(0.05));

	//执行动画
	this->getChildByTag(ObjectTag_BulletBackground)->runAction(Animate::create(animation));
	this->getChildByTag(ObjectTag_BulletBackground)->setOpacity(255);
	
}

void Bullet::setInformation() {
	char damageStr[50];
	sprintf(damageStr, "%d_damage", ID);
	damage = UserDefault::getInstance()->getIntegerForKey(damageStr);
	char firingRateStr[50];
	sprintf(firingRateStr, "%d_firingRate", ID);
	firingRate = UserDefault::getInstance()->getFloatForKey(firingRateStr);
}

void Bullet::putIntoMap(Vec2 point, float rotation) {
	/* 碰撞检测 */
	EventListenerPhysicsContact* contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Bullet::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	my_map->addChild(this, 5);
	this->setPosition(point);

	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByTag(ObjectTag_Player));
	
	//射击类武器
	if (ID < SwordID) {
		//人物朝右
		if (player->showSprite()->getScaleX() > 0) {
			this->setScaleX(1);
			this->setRotation(rotation);
			this->runAction(MoveBy::create(static_cast <float>(2), Vec2(4000 * cos(rotation / 180 * M_PI), -4000 * sin(rotation / 180 * M_PI))));
		}
		//人物朝左
		else {
			this->setScaleX(-1);
			this->setRotation(-rotation);
			this->runAction(MoveBy::create(static_cast <float>(2), Vec2(-4000 * cos(rotation / 180 * M_PI), -4000 * sin(rotation / 180 * M_PI))));
		}
		//设置障碍层
		setBarrierLater();
		this->scheduleUpdate();
	}
	//近战武器
	else {
		//近战武器无需监听，判断障碍
		this->unscheduleUpdate();

		if (player->showSprite()->getScaleX() > 0) {
			//人物朝右
			this->setScaleX(1);
			this->setRotation(rotation);
		}
		else {
			//人物朝左
			this->setScaleX(-1);
			this->setRotation(-rotation);
		}

		//创建序列帧
		auto animation = Animation::create();
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "Weapon/%d/Bullet%d.png", ID, i);
			animation->addSpriteFrameWithFile(nameSize);
		}
		//设置两帧间的时间间隔
		animation->setDelayPerUnit(static_cast<float>(0.1));
		animation->setLoops(1);

		//播放动画
		my_sprite->runAction(Animate::create(animation));
		this->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.3)), RemoveSelf::create(), NULL));
	}
	
}

void Bullet::setBarrierLater() {
	barrier = my_map->getLayer("enemyBarrier");
}

Vec2 Bullet::tileCoordForPosition(Vec2 point) {
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
	y = static_cast<int>(mapTiledNum.height - y - 1);

	//格子坐标从零开始
	return Vec2(static_cast<float>(x), static_cast<float>(y));
}