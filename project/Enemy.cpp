#include"Enemy.h"

bool Enemy::init()
{

	this->scheduleUpdate();

	//设置Tag
	this->setTag(ObjectTag_Enemy);

	std::string EnemyID = "Enemy/" + std::to_string(ID) + "/Normal.png";
	Sprite* EnemySprite = Sprite::create(EnemyID);
	EnemySprite->setScale(static_cast<float>(0.9));
	this->bindSprite(EnemySprite);
	setInformation();

	auto body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	this->addComponent(body);

	appearAction();
	standAction();

	return true;
}

Enemy* Enemy::create(int m_ID) {
	auto enemy = new Enemy();
	enemy->ID = m_ID;

	std::srand((unsigned)time(0));
	int null = static_cast<int>(rand_0_1());
	if (enemy->ID < piggyEnemy) {
		enemy->schedule(CC_SCHEDULE_SELECTOR(Enemy::randomMove), static_cast<float>(2));
	}
	else {
		enemy->schedule(CC_SCHEDULE_SELECTOR(Enemy::towardsPlayerMove), static_cast<float>(2));
	}


	if (enemy && enemy->init()) {
		enemy->autorelease();
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return nullptr;
}

void Enemy::update(float delta) {
	Player* player= dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player));
	if (player == nullptr) {
		return;
	}
	Vec2 enemyPosition = this->getPosition();
	Vec2 playerPosition = player->getPosition();
	if (enemyPosition.x > playerPosition.x) {
		my_sprite->setScaleX(-static_cast<float>(0.9));
	}
	else {
		my_sprite->setScaleX(static_cast<float>(0.9));
	}

}

void Enemy::getMap(TMXTiledMap* map) {
	my_map = map;
}
void Enemy::putIntoMap(TMXTiledMap* map, int tag) {
	getMap(map);
	map->addChild(this, map->getLayer("player")->getLocalZOrder(), tag);
}

void Enemy::setInformation() {
	char damageStr[50];
	sprintf(damageStr, "%d_damage", ID);
	damage = UserDefault::getInstance()->getIntegerForKey(damageStr);
	char HPStr[50];
	sprintf(HPStr, "%d_HP", ID);
	HP = UserDefault::getInstance()->getIntegerForKey(HPStr);
	char SpeedStr[50];
	sprintf(SpeedStr, "%d_Speed", ID);
	Speed = UserDefault::getInstance()->getFloatForKey(SpeedStr);

	enemyWidth = my_sprite->getBoundingBox().size.width; 
	enemyHeight = my_sprite->getBoundingBox().size.height;
}

void Enemy::randomMove(float delta) {
	if (ID == Dummy) { //无法移动的敌人就无需计算 
		return;
	}

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
	Vec2 tiledPos_up = tileCoordForPosition(Vec2(px, py + height / 2));
	Vec2 tiledPos_down = tileCoordForPosition(Vec2(px, py - height / 2));
	Vec2 tiledPos_left = tileCoordForPosition(Vec2(px - width / 2, py));
	Vec2 tiledPos_right = tileCoordForPosition(Vec2(px + width / 2, py));

	//设置障碍层
	setBarrierLayer();

	//通过坐标获取TiledMap上格子的唯一标识
	int tiledGid_up = barrier->getTileGIDAt(tiledPos_up);
	int tiledGid_down = barrier->getTileGIDAt(tiledPos_down);
	int tiledGid_left = barrier->getTileGIDAt(tiledPos_left);
	int tiledGid_right = barrier->getTileGIDAt(tiledPos_right);


	//按键被按下，且精灵的边不在障碍层中
	if (!tiledGid_up) {
		Y += rand_0_1();
	}
	if (!tiledGid_down) {
		Y -= rand_0_1();
	}
	if (!tiledGid_left) {
		//my_sprite->setScaleX(-static_cast<float>(0.2));
		X -= rand_0_1();
	}
	if (!tiledGid_right) {
		//my_sprite->setScaleX(static_cast<float>(0.2));
		X += rand_0_1();
	}

	//转化成单位向量
	float e = sqrt(pow(X, 2) + pow(Y, 2));
	Vec2 destination = Vec2(px + Speed * X / e * 30, py + Speed * Y / e * 30);

	if (!barrier->getTileGIDAt(tileCoordForPosition(destination))) {
		if (rand_0_1() < 0.7) {
			this->runAction(MoveTo::create(static_cast<float>(1), destination));
		}
		else {
			if (!hardMark) {
				attack();
			}
		}
	}
	else {
		if (!hardMark) {
			attack();
		}
	}
	//困难模式下，攻击频率变高
	if (hardMark) {
		attack();
	}
}

void Enemy::towardsPlayerMove(float delta) {
	if (ID == Dummy) { //无法移动的敌人就无需计算 
		return;
	}

	float X = 0, Y = 0, px, py, width, height;

	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Player* player = dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player));
	if (player == nullptr) {
		return;
	}
	px = player->getPosition().x;
	py = player->getPosition().y;

	//获得缩放后的长宽
	width = (this->showSprite())->getBoundingBox().size.width;
	height = (this->showSprite())->getBoundingBox().size.height;

	//获取上下左右四条边的点
	Vec2 tiledPos_up = tileCoordForPosition(Vec2(px, py + height / 2));
	Vec2 tiledPos_down = tileCoordForPosition(Vec2(px, py - height / 2));
	Vec2 tiledPos_left = tileCoordForPosition(Vec2(px - width / 2, py));
	Vec2 tiledPos_right = tileCoordForPosition(Vec2(px + width / 2, py));

	//设置障碍层
	setBarrierLayer();

	//通过坐标获取TiledMap上格子的唯一标识
	int tiledGid_up = barrier->getTileGIDAt(tiledPos_up);
	int tiledGid_down = barrier->getTileGIDAt(tiledPos_down);
	int tiledGid_left = barrier->getTileGIDAt(tiledPos_left);
	int tiledGid_right = barrier->getTileGIDAt(tiledPos_right);


	//按键被按下，且精灵的边不在障碍层中
	if (!tiledGid_up) {
		Y += rand_0_1();
	}
	if (!tiledGid_down) {
		Y -= rand_0_1();
	}
	if (!tiledGid_left) {
		//my_sprite->setScaleX(-static_cast<float>(0.2));
		X -= rand_0_1();
	}
	if (!tiledGid_right) {
		//my_sprite->setScaleX(static_cast<float>(0.2));
		X += rand_0_1();
	}

	//计算距离
	float distance = pow(this->getPosition().x - px, 2) + pow(this->getPosition().y - py, 2);
	Vec2 destination;

	//如果角色不在攻击范围内随机移动
	if (distance > pow(700, 2)) {
		//转化成单位向量
		float e = sqrt(pow(X, 2) + pow(Y, 2));
		destination = Vec2(this->getPosition().x + Speed * X / e * 30, this->getPosition().y + Speed * Y / e * 30);
		CCLOG("%f,%f", destination.x, destination.y);
	}
	else {
		destination = Vec2(px + 15 * X, py + 15 * Y);
	}
	
	distance = pow(this->getPosition().x - destination.x, 2) + pow(this->getPosition().y - destination.y, 2);

	if (!barrier->getTileGIDAt(tileCoordForPosition(destination))) {
		if (rand_0_1() < 0.7) {
			this->runAction(MoveTo::create(static_cast<float>(sqrt(distance) / (Speed * 30)), destination));
		}
		//可移动时攻击
	    //困难模式下，攻击频率变高
		if (!hardMark) {
			if (rand_0_1() < 0.7) {
				attack();
			}
		}
		else {
			attack();
		}
	}
}

void Enemy::appearAction() {

	auto appearSprite = Sprite::create("appearAction/appear1.png");
	//创建序列帧
	auto animation = Animation::create();

	if (ID != Dummy) {
		for (int i = 1; i <= 5; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "appearAction/appear%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	//设置两帧间的时间间隔
	animation->setDelayPerUnit(static_cast<float>(0.1));
	//设置循环1次
	animation->setLoops(1);
	appearSprite->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), NULL));

	//出场动画
	my_sprite->setOpacity(0);
	this->addChild(appearSprite);
	my_sprite->runAction(FadeIn::create(static_cast<float>(0.5)));
}

void Enemy::standAction() {
	//创建序列帧
	auto animation = Animation::create();

	if (ID != Dummy) {
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "Enemy/%d/stand%d.png", ID, i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}

	//设置两帧间的时间间隔
	animation->setDelayPerUnit(static_cast<float>(0.3));
	//设置循环，-1无限循环
	animation->setLoops(-1);
	//在最后一帧播放完恢复到第一帧
	animation->setRestoreOriginalFrame(true);

	my_sprite->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.5)), Animate::create(animation), NULL));
}

void Enemy::attackAction() {
	my_sprite->stopAllActions();
	//创建序列帧
	auto animation = Animation::create();

	if (ID != Dummy) {
		for (int i = 1; i <= 4; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "Enemy/%d/attack%d.png", ID, i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}

	//设置两帧间的时间间隔
	animation->setDelayPerUnit(static_cast<float>(0.1));
	animation->setLoops(1);

	my_sprite->runAction(Animate::create(animation));
}

void Enemy::attack() {
	attackAction();
	standAction();

	Player* player = dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player));
	//如果没有玩家,不攻击
	if (player == nullptr) {
		return;
	}

	Vec2 enemyPosition = this->getPosition();
	Vec2 playerPosition = player->getPosition();

	float rotation = atan((enemyPosition.y - playerPosition.y) / (enemyPosition.x - playerPosition.x)) * 180 / M_PI;

	Vec2 point = this->getPosition();

	//发射三个子弹
	if (ID ==longRangeEnemy1) {
		for (int i = -1; i <= 1; i++) {
			auto bullet = EnemyBullet::create(ID);
			bullet->my_map = this->my_map;
			if (my_sprite->getScaleX() > 0) {
				bullet->putIntoMap(point, rotation + i * 30);
			}
			else {
				bullet->putIntoMap(point, rotation + 180 + i * 30);
			}
		}
		return;
	}

	auto bullet = EnemyBullet::create(ID);
	bullet->my_map = this->my_map;
	//远程
	if (ID < piggyEnemy) {
		if (my_sprite->getScaleX() > 0) {
			bullet->putIntoMap(point, rotation);
		}
		else {
			bullet->putIntoMap(point, rotation + 180);
		}
	}
	//近战，攻击效果直接加在图像上，由于怪物tag各不同，不好将其放在地图上
	else {
		this->addChild(bullet);
		if (my_sprite->getScaleX() > 0) {
			bullet->setScaleX(1);
			bullet->setRotation(-rotation);
			bullet->setPosition(enemyWidth / 2 * cos(rotation / 180 * M_PI), enemyHeight / 2 * sin(rotation / 180 * M_PI));
		}
		else {
			bullet->setScaleX(-1);
			bullet->setRotation(-rotation);
			bullet->setPosition(-enemyWidth / 2 * cos(rotation / 180 * M_PI), -enemyHeight / 2 * sin(rotation / 180 * M_PI));
		}
		//目的是为了获取监听，跟地点角度无关
		bullet->putIntoMap();
		
		bullet->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.5)), RemoveSelf::create(), NULL));
	}
	
}

void Enemy::locked() {
	if (this->getChildByTag(ObjectTag_lockingCircle) == nullptr) {
		Sprite* lockingCircle = Sprite::create("Enemy/lockingCircle.png");
		lockingCircle->setScale(enemyWidth / lockingCircle->getContentSize().width);
		lockingCircle->setPosition(0, -enemyHeight / 2); 
		this->addChild(lockingCircle, -1, ObjectTag_lockingCircle);
	}
}

void Enemy::unlocked() {
	this->removeChildByTag(ObjectTag_lockingCircle);
}

void Enemy::changeHP(int changeValue) {
	HP += changeValue;

	if (HP <= 0) {
		aliveMark = false;
		//死亡图像
		std::string deadStr = "Enemy/" + std::to_string(ID) + "/dead.png";
		auto deadSprite = Sprite::create(deadStr);
		if (deadSprite != nullptr) {
			deadSprite->setPosition(this->getPosition());
			my_map->addChild(deadSprite, my_map->getLayer("player")->getLocalZOrder() - 2);
			if (my_sprite->getScaleX() < 0) {
				deadSprite->setScaleX(-1);
			}
		}

		//随机生成0-1个coin，以及0-1个MpFactor
		int CoinNum = static_cast<int>(rand_0_1() * 2);
		int MpFactorNum = static_cast<int>(rand_0_1() * 2);
		for (int i = 0; i < CoinNum; i++) {
			Coin* coin = Coin::create();
			coin->setPosition(this->getPosition());
			coin->runAction(Sequence::create(MoveBy::create(static_cast<float>(0.2), Vec2(rand_0_1() * 400 - 200, rand_0_1() * 400 - 200)), NULL));
			my_map->addChild(coin, my_map->getLayer("player")->getLocalZOrder());
		}
		for (int i = 0; i < MpFactorNum; i++) {
			Sprite* MpFactor = Sprite::create("Coin/MPFactor.png");
			MpFactor->setScale(static_cast<float>(0.3));
			MpFactor->setPosition(this->getPosition());
			MpFactor->runAction(Sequence::create(MoveBy::create(static_cast<float>(0.2), Vec2(rand_0_1() * 400 - 200, rand_0_1() * 400 - 200)),
				DelayTime::create(static_cast<float>(0.5)), FadeOut::create(static_cast<float>(0.5)), NULL));
			my_map->addChild(MpFactor, my_map->getLayer("player")->getLocalZOrder());
		}
		dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player))->changeMP(5 * MpFactorNum);

		this->runAction(RemoveSelf::create());
	}

	if (changeValue < 0) {
		std::string Changestr = std::to_string(changeValue);
		Label* HPLabel = Label::createWithTTF(Changestr, "fonts/arial.ttf", 40);
		HPLabel->runAction(Sequence::create(MoveBy::create(static_cast<float>(0.2), Vec2(0, 30)),
			DelayTime::create(static_cast<float>(0.5)), RemoveSelf::create(), NULL));
		HPLabel->setTextColor(Color4B::RED);
		HPLabel->setPosition(Vec2(my_sprite->getBoundingBox().size.width / 2, my_sprite->getBoundingBox().size.height / 2 - 30));
		this->addChild(HPLabel);
	}

	std::string HPstr = std::to_string(HP);
	Label* HPLabel = Label::createWithTTF(HPstr, "fonts/arial.ttf", 40);
	HPLabel->setTextColor(Color4B::RED);
	HPLabel->setPosition(Vec2(0, my_sprite->getBoundingBox().size.height / 2));
	this->removeChildByTag(0);
	this->addChild(HPLabel, 1, 0);

}

void Enemy::setBarrierLayer() {
	barrier = my_map->getLayer("enemyBarrier");
}

Vec2 Enemy::tileCoordForPosition(Vec2 point) {
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