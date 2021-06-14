#include"Bullet.h"

bool Bullet::init()
{
	
	//����Tag
	this->setTag(ObjectTag_Bullet);

	std::string bulletID = "Weapon/" + std::to_string(ID) + "/Bullet.png";
	Sprite* bulletSprite = Sprite::create(bulletID);
	bulletSprite->setScale(static_cast<float>(0.7));
	this->bindSprite(bulletSprite);
	setInformation();

	std::string bulletBackgroundID = "Weapon/" + std::to_string(ID) + "/BulletBackground.png";
	Sprite* bulletBackgroundSprite = Sprite::create(bulletBackgroundID);
	if (bulletBackgroundSprite != nullptr) {
		bulletBackgroundSprite->setOpacity(100);
		this->addChild(bulletBackgroundSprite,0, ObjectTag_BulletBackground);
	}

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
	Vec2 tiledPos = tileCoordForPosition(this->getPosition());
	if (barrier->getTileGIDAt(tiledPos)) {
		this->removeBullet();
	}
}

bool Bullet::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	
	//��ֹ�ڵ�Ϊnullptr����
	if (nodeA == nullptr || nodeB == nullptr) {
		CCLOG("node=nullptr");
		return true;
	}

	Node* BulletNode = NULL;    /* ������� */
	Node* EnemyNode = NULL;

	//�ж�������ײ����
	if (nodeA->getTag() == ObjectTag_Bullet) {
		BulletNode = nodeA;
		EnemyNode = nodeB;
	}
	else {
		BulletNode = nodeB;
		EnemyNode = nodeA;
	}

	//����ײ���ӵ��͵���
	Bullet* bullet = dynamic_cast<Bullet*>(BulletNode);
	Enemy* enemy = dynamic_cast<Enemy*>(EnemyNode);


	//�ӵ���������
	if (BulletNode->getTag() == ObjectTag_Bullet && (EnemyNode->getTag() >= ObjectTag_Enemy && EnemyNode->getTag() <= ObjectTag_Enemy + 10)) {
		enemy->changeHP(-damage);
		//�������
		if (bullet->ID < SwordID) {
			bullet->removeBullet();
		}
	}
	return true;
}

void Bullet::removeBullet() {
	//Ҫ���Ƴ�������壬�������ֶ���˺������
	this->removeAllComponents();
	this->my_sprite->setVisible(false);
	this->stopAllActions();
	this->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.15)), RemoveSelf::create(), NULL));

	//�����ӵ�ײ������
	auto animation = Animation::create();

	for (int i = 1; i <= 3; i++) {
		char nameSize[100] = { 0 };
		sprintf(nameSize, "Weapon/%d/removeAction%d.png", ID, i);
		animation->addSpriteFrameWithFile(nameSize);
	}

	//������֡���ʱ����
	animation->setDelayPerUnit(static_cast<float>(0.05));

	//ִ�ж���
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
	/* ��ײ��� */
	EventListenerPhysicsContact* bulletContactListener = EventListenerPhysicsContact::create();
	bulletContactListener->onContactBegin = CC_CALLBACK_1(Bullet::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(bulletContactListener, this);

	my_map->addChild(this, 20);
	this->setPosition(point);

	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByTag(ObjectTag_Player));
	
	//���������
	if (ID < SwordID) {
		//���ﳯ��
		if (player->showSprite()->getScaleX() > 0) {
			this->setScaleX(1);
			this->setRotation(rotation);
			this->runAction(MoveBy::create(static_cast <float>(2), Vec2(4000 * cos(rotation / 180 * M_PI), -4000 * sin(rotation / 180 * M_PI))));
		}
		//���ﳯ��
		else {
			this->setScaleX(-1);
			this->setRotation(-rotation);
			this->runAction(MoveBy::create(static_cast <float>(2), Vec2(-4000 * cos(rotation / 180 * M_PI), -4000 * sin(rotation / 180 * M_PI))));
		}
		//�����ϰ���
		setBarrierLayer();
		this->scheduleUpdate();
	}
	//��ս����
	else {
		//��ս��������������ж��ϰ�
		this->unscheduleUpdate();

		if (player->showSprite()->getScaleX() > 0) {
			//���ﳯ��
			this->setScaleX(1);
			this->setRotation(rotation);
		}
		else {
			//���ﳯ��
			this->setScaleX(-1);
			this->setRotation(-rotation);
		}

		//��������֡
		auto animation = Animation::create();
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "Weapon/%d/Bullet%d.png", ID, i);
			animation->addSpriteFrameWithFile(nameSize);
		}
		//������֡���ʱ����
		animation->setDelayPerUnit(static_cast<float>(0.1));
		animation->setLoops(1);

		//���Ŷ���
		my_sprite->runAction(Animate::create(animation));
		this->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.3)), RemoveSelf::create(), NULL));
	}
	
}

void Bullet::setBarrierLayer() {
	barrier = my_map->getLayer("barrier");
}

Vec2 Bullet::tileCoordForPosition(Vec2 point) {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// ��ȡ��Ƭ��ͼ�ĳߴ�(��Ƭ����)
	auto mapTiledNum = my_map->getMapSize();
	//��ȡ������Ƭ�ĳߴ�(pixel)
	auto tiledSize = my_map->getTileSize();


	//x��ʾ��Ƭ��ͼ�ϵڼ�����Ƭ
	int x = int(point.x / (tiledSize.width * 2));
	int y = int(point.y / (tiledSize.height * 2));

	//cocos2dx��tiledmap���겻ͬ
	y = static_cast<int>(mapTiledNum.height - y - 1);

	//����������㿪ʼ
	return Vec2(static_cast<float>(x), static_cast<float>(y));
}