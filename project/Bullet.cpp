#include"Bullet.h"

bool Bullet::init()
{
	
	//����Tag
	this->setTag(ObjectTag_Bullet);

	std::string bulletID = "Weapon/" + std::to_string(ID) + "Bullet.png";
	Sprite* bulletSprite = Sprite::create(bulletID);
	bulletSprite->setScale(static_cast<float>(0.5));
	this->bindSprite(bulletSprite);
	setInformation();



	//auto soundEffect = AudioEngine::play2d("Music/eatCoin.mp3", false);
	////��ȡ֮ǰ������
	//int volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
	//AudioEngine::setVolume(soundEffect, volumePercent / 100.f);

	auto body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
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
		this->runAction(RemoveSelf::create());
	}
}

bool Bullet::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	

	//����ײ���ӵ��͵���
	Bullet* bullet = dynamic_cast<Bullet*>(nodeA);
	Enemy* enemy = dynamic_cast<Enemy*>(nodeB);

	//��ֹ�ڵ�Ϊnullptr����
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//�ӵ���������
	if (nodeA->getTag() == ObjectTag_Bullet && nodeB->getTag() == ObjectTag_Enemy) {
		enemy->changeHP(-damage);

		//Ҫ���Ƴ�������壬�������ֶ���˺������
		bullet->removeAllComponents();
		bullet->runAction(RemoveSelf::create());
	}
	return true;
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
	/* ��ײ��� */
	EventListenerPhysicsContact* contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Bullet::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	my_map->addChild(this, 5);
	this->setPosition(point);
	

	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByTag(ObjectTag_Player));
	if (player->showSprite()->getScaleX() > 0) {
		this->setScaleX(1);
		this->setRotation(rotation);
		this->runAction(MoveBy::create(static_cast <float>(2), Vec2(4000 * cos(rotation / 180 * M_PI), -4000 * sin(rotation / 180 * M_PI))));
	}
	else {
		this->setScaleX(-1);
		this->setRotation(-rotation);
		this->runAction(MoveBy::create(static_cast <float>(2), Vec2(-4000 * cos(rotation / 180 * M_PI), -4000 * sin(rotation / 180 * M_PI))));
	}
	
	
	
	//�����ϰ���
	setBarrierLater();

	this->scheduleUpdate();
}

void Bullet::setBarrierLater() {
	barrier = my_map->getLayer("enemyBarrier");
}

Vec2 Bullet::tileCoordForPosition(Vec2 point) {
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
	y = static_cast<int>(mapTiledNum.height - y - 1);

	//����������㿪ʼ
	return Vec2(static_cast<float>(x), static_cast<float>(y));
}