#include"EnemyBullet.h"

bool EnemyBullet::init()
{
		//设置Tag
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

	//防止节点为nullptr闪退
	if (nodeA == nullptr || nodeB == nullptr) {
		CCLOG("node=nullptr");
		return true;
	}

	Node* EnemyBulletNode = NULL;    /* 宝箱对象 */
	Node* otherNode = NULL;

	//判断两个碰撞对象
	if (nodeA->getTag() == ObjectTag_EnemyBullet) {
		EnemyBulletNode = nodeA;
		otherNode = nodeB;
	}
	else {
		EnemyBulletNode = nodeB;
		otherNode = nodeA;
	}

	//绑定碰撞的子弹和敌人
	EnemyBullet* bullet = dynamic_cast<EnemyBullet*>(EnemyBulletNode);
	
	//子弹碰到玩家的近战武器
	if (EnemyBulletNode->getTag() == ObjectTag_EnemyBullet && otherNode->getTag() == ObjectTag_Bullet) {
		Bullet* swordBullet = dynamic_cast<Bullet*>(otherNode);
		//远程且碰到近战武器的子弹
		if ((bullet->ID < piggyEnemy || ID >= GoblinPriest) && swordBullet->getID() >= SwordID) {
			bullet->removeBullet();
		}
	}

	//子弹碰到玩家
	if (EnemyBulletNode->getTag() == ObjectTag_EnemyBullet && otherNode->getTag() == ObjectTag_Player) {
		Player* player = dynamic_cast<Player*>(otherNode);
		player->changeHP(-damage);
		//远程
		if (bullet->ID < piggyEnemy || ID >= GoblinPriest) {
			bullet->removeBullet();
		}
		else {
			//同角色子弹一样，否则会出现重复伤害，直接秒杀
			bullet->removeAllComponents();
		}
	}

	

	return true;
}

void EnemyBullet::removeBullet() {
	//要先移除物理刚体，否则会出现多次伤害的情况
	this->removeAllComponents();
	this->stopAllActions();
	this->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.15)), RemoveSelf::create(), NULL));

	//创建子弹撞击动画
	auto animation = Animation::create();

	for (int i = 1; i <= 3; i++) {
		char nameSize[100] = { 0 };
		sprintf(nameSize, "Enemy/%d/removeAction%d.png", ID, i);
		animation->addSpriteFrameWithFile(nameSize);
	}

	//设置两帧间的时间间隔
	animation->setDelayPerUnit(static_cast<float>(0.05));

	//执行动画
	my_sprite->runAction(Animate::create(animation));

}

void EnemyBullet::setInformation() {
	char damageStr[50];
	sprintf(damageStr, "%d_damage", ID);
	damage = UserDefault::getInstance()->getIntegerForKey(damageStr);
}

void EnemyBullet::putIntoMap(Vec2 point, float rotation) {
	/* 碰撞检测 */
	EventListenerPhysicsContact* bulletContactListener = EventListenerPhysicsContact::create();
	bulletContactListener->onContactBegin = CC_CALLBACK_1(EnemyBullet::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(bulletContactListener, this);

	//远程
	if (ID < piggyEnemy || ID >= GoblinPriest) {
		my_map->addChild(this, 20);
		this->setPosition(point);

		//设置障碍层
		setBarrierLayer();

		//监听判断障碍
		this->scheduleUpdate();
		this->setRotation(-rotation);
		this->runAction(MoveBy::create(static_cast <float>(5), Vec2(4000 * cos(rotation / 180 * M_PI), 4000 * sin(rotation / 180 * M_PI))));
	}
}

void EnemyBullet::setBarrierLayer() {
	barrier = my_map->getLayer("barrier");
}

Vec2 EnemyBullet::tileCoordForPosition(Vec2 point) {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 获取瓦片地图的尺寸(瓦片数量)
	auto mapTiledNum = my_map->getMapSize();
	//获取单个瓦片的尺寸(pixel)
	auto tiledSize = my_map->getTileSize();


	//x表示瓦片地图上第几个瓦片
	int x = int(point.x / (tiledSize.width * 2));
	int y = int(point.y / (tiledSize.height * 2));

	//cocos2dx与tiledmap坐标不同
	y = static_cast<int>(mapTiledNum.height - y - 1);

	if (x<0 || x>mapTiledNum.width) {
		x = 0;
	}
	if (y<0 || y>mapTiledNum.height) {
		y = 0;
	}

	//格子坐标从零开始
	return Vec2(static_cast<float>(x), static_cast<float>(y));
}