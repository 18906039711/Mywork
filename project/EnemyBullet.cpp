#include"EnemyBullet.h"

bool EnemyBullet::init()
{
		//����Tag
	this->setTag(ObjectTag_EnemyBullet);

	std::string bulletID = "Enemy/" + std::to_string(ID) + "/Bullet.png";
	Sprite* bulletSprite = Sprite::create(bulletID);
	bulletSprite->setScale(static_cast<float>(0.7));
	this->bindSprite(bulletSprite);
	setInformation();


	PhysicsBody* body;
	if (ID < piggyEnemy) {
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

EnemyBullet* EnemyBullet::create(int m_ID) {
	auto bullet = new EnemyBullet();
	bullet->ID = m_ID;

	if (bullet && bullet->init()) {
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}
void EnemyBullet::update(float delta) {
	Vec2 tiledPos = tileCoordForPosition(this->getPosition());
	if (barrier == nullptr) {
		return;
	}
	if (barrier->getTileGIDAt(tiledPos)) {
		this->removeBullet();
	}
}

bool EnemyBullet::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	//��ֹ�ڵ�Ϊnullptr����
	if (nodeA == nullptr || nodeB == nullptr) {
		CCLOG("node=nullptr");
		return true;
	}

	Node* EnemyBulletNode = NULL;    /* ������� */
	Node* otherNode = NULL;

	//�ж�������ײ����
	if (nodeA->getTag() == ObjectTag_EnemyBullet) {
		EnemyBulletNode = nodeA;
		otherNode = nodeB;
	}
	else {
		EnemyBulletNode = nodeB;
		otherNode = nodeA;
	}

	//����ײ���ӵ��͵���
	EnemyBullet* bullet = dynamic_cast<EnemyBullet*>(EnemyBulletNode);
	
	//�ӵ�������ҵĽ�ս����
	if (EnemyBulletNode->getTag() == ObjectTag_EnemyBullet && otherNode->getTag() == ObjectTag_Bullet) {
		Bullet* swordBullet = dynamic_cast<Bullet*>(otherNode);
		//Զ����������ս�������ӵ�
		if ((bullet->ID < piggyEnemy || ID >= GoblinPriest) && swordBullet->getID() >= SwordID) {
			bullet->removeBullet();
		}
	}

	//�ӵ��������
	if (EnemyBulletNode->getTag() == ObjectTag_EnemyBullet && otherNode->getTag() == ObjectTag_Player) {
		Player* player = dynamic_cast<Player*>(otherNode);
		player->changeHP(-damage);
		//Զ��
		if (bullet->ID < piggyEnemy || ID >= GoblinPriest) {
			bullet->removeBullet();
		}
		else {
			//ͬ��ɫ�ӵ�һ�������������ظ��˺���ֱ����ɱ
			bullet->removeAllComponents();
		}
	}

	

	return true;
}

void EnemyBullet::removeBullet() {
	//Ҫ���Ƴ�������壬�������ֶ���˺������
	this->removeAllComponents();
	this->stopAllActions();
	this->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.15)), RemoveSelf::create(), NULL));

	//�����ӵ�ײ������
	auto animation = Animation::create();

	for (int i = 1; i <= 3; i++) {
		char nameSize[100] = { 0 };
		sprintf(nameSize, "Enemy/%d/removeAction%d.png", ID, i);
		animation->addSpriteFrameWithFile(nameSize);
	}

	//������֡���ʱ����
	animation->setDelayPerUnit(static_cast<float>(0.05));

	//ִ�ж���
	my_sprite->runAction(Animate::create(animation));

}

void EnemyBullet::setInformation() {
	char damageStr[50];
	sprintf(damageStr, "%d_damage", ID);
	damage = UserDefault::getInstance()->getIntegerForKey(damageStr);
}

void EnemyBullet::putIntoMap(Vec2 point, float rotation) {
	/* ��ײ��� */
	EventListenerPhysicsContact* bulletContactListener = EventListenerPhysicsContact::create();
	bulletContactListener->onContactBegin = CC_CALLBACK_1(EnemyBullet::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(bulletContactListener, this);

	//Զ��
	if (ID < piggyEnemy || ID >= GoblinPriest) {
		my_map->addChild(this, 20);
		this->setPosition(point);

		//�����ϰ���
		setBarrierLayer();

		//�����ж��ϰ�
		this->scheduleUpdate();
		this->setRotation(-rotation);
		this->runAction(MoveBy::create(static_cast <float>(5), Vec2(4000 * cos(rotation / 180 * M_PI), 4000 * sin(rotation / 180 * M_PI))));
	}
}

void EnemyBullet::setBarrierLayer() {
	barrier = my_map->getLayer("barrier");
}

Vec2 EnemyBullet::tileCoordForPosition(Vec2 point) {
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

	if (x<0 || x>mapTiledNum.width) {
		x = 0;
	}
	if (y<0 || y>mapTiledNum.height) {
		y = 0;
	}

	//����������㿪ʼ
	return Vec2(static_cast<float>(x), static_cast<float>(y));
}