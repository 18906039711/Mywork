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
	if (enemy->ID < piggyEnemy || enemy->ID >= GoblinPriest) {
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

	//boss等待出场动画
	static int waitAction = 1;
	if (ID >= GoblinPriest) {
		if (waitAction) {
			waitAction--;
			return;
		}
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
			if (!(hardMark || ID == GoblinPriest)) {
				attack();
			}
		}
	}
	else {
		if (!(hardMark || ID == GoblinPriest)) {
			attack();
		}
	}
	//困难模式下或boos，攻击频率变高
	if (hardMark || ID == GoblinPriest) {
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
	}
	else {
		destination = Vec2(px + 15 * X, py + 15 * Y);
	}
	
	CCLOG("%d,%d", tileCoordForPosition(destination).x, tileCoordForPosition(destination).y);
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
	//如果是boss，有独特的攻击模式
	if (ID >= GoblinPriest) {
		bossAttack();
		return;
	}
	Player* player = dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player));
	//如果没有玩家,不攻击
	if (player == nullptr) {
		return;
	}

	attackAction();
	standAction();

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

void Enemy::bossAttack() {
	static int timeCounter = 0;
	timeCounter++;
	Player* player = dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player));
	//如果没有玩家,不攻击
	if (player == nullptr) {
		return;
	}

	Vec2 enemyPosition = this->getPosition();
	Vec2 playerPosition = player->getPosition();

	float rotation = atan((enemyPosition.y - playerPosition.y) / (enemyPosition.x - playerPosition.x)) * 180 / M_PI;

	Vec2 point = this->getPosition();

	//攻击模式的切换，以及小怪的召唤
	//竖直雷电攻击
	if (timeCounter == 4|| timeCounter == 7) {
		/*Boss召唤动画*/
		my_sprite->stopAllActions();
		//创建序列帧
		auto summon = Animation::create();
		for (int i = 5; i <= 6; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "Enemy/%d/attack%d.png", ID, i);
			summon->addSpriteFrameWithFile(nameSize);
		}
		//设置两帧间的时间间隔
		summon->setDelayPerUnit(static_cast<float>(1));
		summon->setLoops(1);
		my_sprite->runAction(Animate::create(summon));
		/*Boss召唤动画*/

		/*范围提示*/
		Sprite* strikingRange = Sprite::create("thunderAnimation/strikingRange.png");
		strikingRange->setOpacity(100);
		strikingRange->setScale(static_cast<float>(0.1));
		strikingRange->setPosition(player->getPosition());
		my_map->addChild(strikingRange, my_map->getLayer("floor")->getLocalZOrder() + 1, ObjectTag_DamageRange);
		strikingRange->runAction(Spawn::create(FadeTo::create(static_cast<float>(1), 200), ScaleTo::create(static_cast<float>(1), 1), NULL));
		strikingRange->runAction(Sequence::create(DelayTime::create(static_cast<float>(1.5)), RemoveSelf::create(), NULL));
		/*范围提示*/

		/*痕迹*/
		Sprite* vestige = Sprite::create("thunderAnimation/vestige.png");
		vestige->setPosition(player->getPosition());
		vestige->setOpacity(0);
		my_map->addChild(vestige, my_map->getLayer("floor")->getLocalZOrder() + 1);
		vestige->runAction(Sequence::create(DelayTime::create(static_cast<float>(1.5)),FadeIn::create(static_cast<float>(0.5)),NULL));
		/*痕迹*/

		/*雷电动画*/
		auto thunder = Sprite::create("thunderAnimation/1.png");
		auto thunderAnimation = Animation::create();
		for (int i = 1; i <= 10; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "thunderAnimation/%d.png", i);
			thunderAnimation->addSpriteFrameWithFile(nameSize);
		}
		//设置两帧间的时间间隔
		thunderAnimation->setDelayPerUnit(static_cast<float>(0.1));
		thunderAnimation->setLoops(1);
		thunder->setOpacity(0);
		thunder->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.99)), FadeIn::create(static_cast<float>(0.01)), NULL));
		thunder->runAction(Sequence::create(DelayTime::create(static_cast<float>(1)), Animate::create(thunderAnimation), NULL));
		thunder->runAction(Sequence::create(DelayTime::create(static_cast<float>(2)), RemoveSelf::create(), NULL));
		thunder->setPosition(player->getPosition().x, player->getPosition().y + thunder->getBoundingBox().size.height / 4);
		my_map->addChild(thunder, 20);
		/*雷电动画*/

		/*雷电音效*/
		auto soundEffect = AudioEngine::play2d("Music/thunder.mp3", false);
		//读取之前的音量
		int volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
		AudioEngine::setVolume(soundEffect, volumePercent / 100.f);
		/*雷电音效*/

		/*伤害判定*/
		this->scheduleOnce(CC_SCHEDULE_SELECTOR(Enemy::thunderAttack), 1.3);
	}
	//召唤三只小怪
	else if (timeCounter == 10|| timeCounter==5) {
		//攻击循环
		if (timeCounter == 10) {
			timeCounter = 0;
		}

		//召唤动画
		my_sprite->stopAllActions();
		//创建序列帧
		auto summon = Animation::create();
		for (int i = 5; i <= 6; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "Enemy/%d/attack%d.png", ID, i);
			summon->addSpriteFrameWithFile(nameSize);
		}
		//设置两帧间的时间间隔
		summon->setDelayPerUnit(static_cast<float>(0.3));
		summon->setLoops(1);
		my_sprite->runAction(Animate::create(summon));
		standAction();

		int enemyType = rand_0_1() * 4;
		while (enemyType == 2) {
			enemyType = rand_0_1() * 4;
		}
		Enemy* enemy = Enemy::create(longRangeEnemy1 + enemyType);
		enemy->setPosition(this->getPosition().x + rand_0_1() * 500 - 500 / 2,
			this->getPosition().y + rand_0_1() * 500 - 500 / 2);
		//如果生成在障碍中，重新生成
		while (barrier->getTileGIDAt(tileCoordForPosition(enemy->getPosition()))) {
			enemy->setPosition(this->getPosition().x + rand_0_1() * 500 - 500 / 2,
				this->getPosition().y + rand_0_1() * 500 - 500 / 2);
		}
		//计算场上小怪数
		int Num = 1;
		while (my_map->getChildByTag(ObjectTag_Enemy + Num) != nullptr) {
			Num++;
		}
		enemy->putIntoMap(my_map, ObjectTag_Enemy + Num);
	}
	//普通攻击，半血时增强
	else {
		attackAction();
		standAction();
		char HPStr[50];
		sprintf(HPStr, "%d_HP", ID);
		int MaxHP = UserDefault::getInstance()->getIntegerForKey(HPStr);
		if (HP > MaxHP / 2) {
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
		}
		else {
			for (int i = -3; i <= 3; i++) {
				auto bullet = EnemyBullet::create(ID);
				bullet->my_map = this->my_map;
				if (my_sprite->getScaleX() > 0) {
					bullet->putIntoMap(point, rotation + i * 15);
				}
				else {
					bullet->putIntoMap(point, rotation + 180 + i * 15);
				}
			}
		}
	}
}

void Enemy::thunderAttack(float dt) {
	auto player = dynamic_cast<Player*> (my_map->getChildByTag(ObjectTag_Player));
	auto damageRange = my_map->getChildByTag(ObjectTag_DamageRange);
	float distance = pow(player->getPosition().x - damageRange->getPosition().x, 2) + pow(player->getPosition().y - damageRange->getPosition().y, 2);
	if (distance <= pow(damageRange->getBoundingBox().size.height / 2 + player->showSprite()->getBoundingBox().size.height / 2, 2)) {
		player->changeHP(-damage * 2);
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
	if (this->getChildByTag(ObjectTag_lockingCircle) != nullptr) {
		this->removeChildByTag(ObjectTag_lockingCircle);
	}
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

		//boss掉落较多
		if (ID >= GoblinPriest) {
			CoinNum = static_cast<int>(rand_0_1() * 5) + 30;
			MpFactorNum = static_cast<int>(rand_0_1() * 5) + 25;
		}
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
				DelayTime::create(static_cast<float>(1)), FadeOut::create(static_cast<float>(0.5)), NULL));
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

	//显示血量
	//std::string HPstr = std::to_string(HP);
	//Label* HPLabel = Label::createWithTTF(HPstr, "fonts/arial.ttf", 40);
	//HPLabel->setTextColor(Color4B::RED);
	//HPLabel->setPosition(Vec2(0, my_sprite->getBoundingBox().size.height / 2));
	//this->removeChildByTag(0);
	//this->addChild(HPLabel, 1, 0);

}

int Enemy::getHP() {
	return this->HP;
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

	if (x<0 || x>mapTiledNum.width) {
		x = 0;
	}
	if (y<0 || y>mapTiledNum.height) {
		y = 0;
	}

	//格子坐标从零开始
	return Vec2(static_cast<float>(x), static_cast<float>(y));
}