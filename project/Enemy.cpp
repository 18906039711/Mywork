#include"Enemy.h"

bool Enemy::init()
{

	//this->scheduleUpdate();

	//设置Tag
	this->setTag(ObjectTag_Enemy);

	std::string EnemyID = "Enemy/" + std::to_string(ID) + ".png";
	Sprite* EnemySprite = Sprite::create(EnemyID);
	this->bindSprite(EnemySprite);
	setInformation();

	auto body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	this->addComponent(body);

	

	return true;
}

Enemy* Enemy::create(int m_ID) {
	auto enemy = new Enemy();

	std::srand((unsigned)time(0));
	int null = static_cast<int>(rand_0_1());
	//类似宝箱，每个对象分别判定，所以不能放在init里
	enemy->schedule(CC_SCHEDULE_SELECTOR(Enemy::randomMove), static_cast<float>(2));

	enemy->ID = m_ID;
	if (enemy && enemy->init()) {
		enemy->autorelease();
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return nullptr;
}

void Enemy::putIntoMap(TMXTiledMap* map) {
	my_map = map;
	map->addChild(this, 4, ObjectTag_Enemy);
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
}

void Enemy::randomMove(float delta) {

	if (!ID && Speed) {  //ID不存在无法移动，无法移动的敌人就无需计算 
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
	setBarrierLater();

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

	Vec2 destination = Vec2(px + Speed * X / e * 40, py + Speed * Y / e * 40);

	if (!barrier->getTileGIDAt(tileCoordForPosition(destination))) {
		//一半概率移动
		if (rand_0_1() < 0.5) {
			this->runAction(MoveTo::create(static_cast<float>(2), destination));
		}
	}

	
}

void Enemy::setBarrierLater() {
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

void Enemy::locked() {
	if (this->getChildByTag(ObjectTag_lockingCircle) == nullptr) {
		Sprite* lockingCircle = Sprite::create("Enemy/lockingCircle.png");
		lockingCircle->setScale(my_sprite->getContentSize().width / lockingCircle->getContentSize().width);
		lockingCircle->setPosition(0, -my_sprite->getContentSize().height / 2);
		this->addChild(lockingCircle, -1, ObjectTag_lockingCircle);
	}
}

void Enemy::unlocked() {
	this->removeChildByTag(ObjectTag_lockingCircle);
}

void Enemy::changeHP(int changeValue) {
	HP += changeValue;
	if (changeValue < 0) {
		std::string Changestr = std::to_string(changeValue);
		Label* HPLabel = Label::createWithTTF(Changestr, "fonts/arial.ttf", 40);
		HPLabel->runAction(Sequence::create(MoveBy::create(static_cast<float>(0.2), Vec2(0, 30)),
			DelayTime::create(static_cast<float>(0.5)), RemoveSelf::create(), NULL));
		HPLabel->setTextColor(Color4B::RED);
		HPLabel->setPosition(Vec2(my_sprite->getBoundingBox().size.width / 2, my_sprite->getBoundingBox().size.height / 2 - 30));
		this->addChild(HPLabel);
	}
	if (HP <= 0) {
		this->runAction(RemoveSelf::create());
	}
	std::string HPstr = std::to_string(HP);
	Label* HPLabel = Label::createWithTTF(HPstr, "fonts/arial.ttf", 40);
	HPLabel->setTextColor(Color4B::RED);
	HPLabel->setPosition(Vec2(0, my_sprite->getBoundingBox().size.height / 2));
	this->removeChildByTag(0);
	this->addChild(HPLabel, 1, 0);
	

}

