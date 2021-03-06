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
	
	if (playerID == rangerID) {
		playerSprite = Sprite::create("character/ranger_right.png");
		playerSprite->setScale(static_cast<float>(0.2));
		//绑定精灵图像
		this->bindSprite(playerSprite);
	}
	else if (playerID == sorcererID) {
		playerSprite = Sprite::create("character/sorcerer_right.png");
		playerSprite->setScale(static_cast<float>(0.2));
		this->bindSprite(playerSprite);
	}
	//设置站立动画
	this->StandAction();
	//设置移动速度
	this->setSpeed(8);

	//设置物理刚体
	auto body = PhysicsBody::createBox(playerSprite->getBoundingBox().size);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setDynamic(false);
	this->setPhysicsBody(body);


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

	this->schedule(CC_SCHEDULE_SELECTOR(Player::recoverDefendce), static_cast<float>(1));
	return true;
}

//动画
void Player::StandAction() {
	//创建序列帧
	auto animation = Animation::create();

	if (playerID == rangerID) {
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/ranger%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	else if (playerID == sorcererID) {
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/sorcerer%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	//设置两帧间的时间间隔
	animation->setDelayPerUnit(static_cast<float>(0.3));
	//设置循环，-1无限循环
	animation->setLoops(-1);
	//在最后一帧播放完恢复到第一帧
	animation->setRestoreOriginalFrame(true);

	my_sprite->runAction(Animate::create(animation));
}

void Player::RunningAction() {
	auto animation = Animation::create();
	if (playerID == rangerID) {
		for (int i = 1; i <= 4; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/ranger/run%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	else if (playerID == sorcererID) {
		for (int i = 1; i <= 4; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/sorcerer/run%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	animation->setDelayPerUnit(static_cast<float>(0.3));
	animation->setLoops(-1);
	animation->setRestoreOriginalFrame(true);
	my_sprite->runAction(Animate::create(animation));
}

void Player::stopPlayerAllActions() {
	my_sprite->stopAllActions();
}

//设置面板属性
void Player::setPlayerAttribute() {
	if (playerID == rangerID) {
		maxHP = UserDefault::getInstance()->getIntegerForKey("RangerHP", 6);
		maxDefendce = UserDefault::getInstance()->getIntegerForKey("RangerDefendce", 3);
		maxMP = UserDefault::getInstance()->getIntegerForKey("RangerMP", 180);
	}
	else if (playerID == sorcererID) {
		maxHP = UserDefault::getInstance()->getIntegerForKey("SorcererHP", 3);
		maxDefendce = UserDefault::getInstance()->getIntegerForKey("SorcererDefendce", 5);
		maxMP = UserDefault::getInstance()->getIntegerForKey("SorcererMP", 240);
	}

	HP = UserDefault::getInstance()->getIntegerForKey("PlayerHP", maxHP);
	MP = UserDefault::getInstance()->getIntegerForKey("PlayerMP", maxMP);
	Defendce = UserDefault::getInstance()->getIntegerForKey("PlayerDefendce", maxDefendce);
}

void Player::changeHP(int changeValue) {
	//先扣护甲
	if (Defendce > 0&& changeValue < 0) {
		changeDefendce(changeValue);
		return;
	}

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
	//数字显示
	if (changeValue > 0) {
		std::string Changestr = "+" + std::to_string(changeValue);
		Label* HPLabel = Label::createWithTTF(Changestr, "fonts/arial.ttf", 40);
		HPLabel->runAction(Sequence::create(DelayTime::create(static_cast<float>(1)), RemoveSelf::create(), NULL));
		HPLabel->setTextColor(Color4B::RED);
		HPLabel->setPosition(Vec2(my_sprite->getBoundingBox().size.width / 2, my_sprite->getBoundingBox().size.height / 2));
		this->addChild(HPLabel);
	}
	else if(changeValue < 0) {
		//闪红
		my_sprite->runAction(Sequence::create(TintTo::create(static_cast<float>(0.1), Color3B::RED),
			TintTo::create(static_cast<float>(0.1), Color3B::WHITE), NULL));

		//有时候正好stopAction会一直变红，所以用函数强制恢复
		//之后的debuff也可以用这个函数
		this->scheduleOnce(CC_SCHEDULE_SELECTOR(Player::recover), 0.1);

		//设置护甲回复冷却时间,5s
		recoverDefenceTime = 5;
	}

	if (HP <= 0) {
		aliveMark = false; 
		my_sprite->removeAllChildren();
		this->removeAllComponents();
		this->unscheduleUpdate();
		this->setVisible(false);
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
	//数字显示
	if (changeValue > 0) {
		std::string Changestr = "+" + std::to_string(changeValue);
		Label* MPLabel = Label::createWithTTF(Changestr, "fonts/arial.ttf", 40);
		MPLabel->runAction(Sequence::create(DelayTime::create(static_cast<float>(1)), RemoveSelf::create(), NULL));
		MPLabel->setTextColor(Color4B::BLUE);
		MPLabel->setPosition(Vec2(my_sprite->getBoundingBox().size.width / 2, my_sprite->getBoundingBox().size.height / 2));
		this->addChild(MPLabel);
	}
}
void Player::changeDefendce(int changeValue) {
	if (Defendce + changeValue < 0) {
		int additionalDamage = Defendce + changeValue;
		UserDefault::getInstance()->setIntegerForKey("PlayerDefendce", 0);
		Defendce = 0;

		//多余伤害扣血
		changeHP(additionalDamage);
	}
	else if (Defendce + changeValue > maxDefendce) {
		UserDefault::getInstance()->setIntegerForKey("PlayerDefendce", maxDefendce);
		Defendce = maxDefendce;
	}
	else {
		UserDefault::getInstance()->setIntegerForKey("PlayerDefendce", Defendce + changeValue);
		Defendce += changeValue;
	}
	if (changeValue < 0) {
		//设置护甲回复冷却时间,5s
		recoverDefenceTime = 5;
	}
	
}

void Player::initializePlayer() {
	UserDefault::getInstance()->setIntegerForKey("PlayerHP", maxHP);
	HP = UserDefault::getInstance()->getIntegerForKey("PlayerHP");

	UserDefault::getInstance()->setIntegerForKey("PlayerDefendce", maxDefendce);
	Defendce = UserDefault::getInstance()->getIntegerForKey("PlayerDefendce");

	UserDefault::getInstance()->setIntegerForKey("PlayerMP", maxMP);
	MP = UserDefault::getInstance()->getIntegerForKey("PlayerMP");

	UserDefault::getInstance()->setIntegerForKey("weaponID", 0);
}


void Player::getPotion(Potion* potion) {
	potion->removeChildByTag(ObjectTag_Information);
	potion->recover(this);
	potion->runAction(RemoveSelf::create());
}

//武器
void Player::getWeapon(Weapon* weapon) {
	//如果手上有武器，先取下
	if (weaponID != 0) {
		//从玩家手中删除
		my_sprite->getChildByTag(ObjectTag_Weapon)->removeFromParentAndCleanup(true);

		//创建一个放在地图上
		Weapon* nowWeapon = Weapon::create(weaponID);
		nowWeapon->my_map = dynamic_cast<TMXTiledMap*>(this->getParent());
		nowWeapon->putIntoMap(this->getPosition());
	}

	this->getParent()->getParent()->removeChildByTag(ObjectTag_Information);
	weapon->removeChildByTag(ObjectTag_WeaponArrow);
	weapon->retain();
	weapon->removeFromParent();
	//如果是近战武器
	if (weapon->ID >= SwordID) {
		weapon->showSprite()->setAnchorPoint(Vec2(0, 0.5));
	}
	weapon->setPosition(Vec2(my_sprite->getContentSize().width / 5 * 4, my_sprite->getContentSize().height / 4));
	weapon->setScale(5);
	UserDefault::getInstance()->setIntegerForKey("weaponID", weapon->ID);
	weaponID = weapon->ID;
	my_sprite->addChild(weapon, 0, ObjectTag_Weapon);
	weapon->fireSwitch(true);
	//将玩家所在地图传给武器，以便传给子弹
	weapon->my_map = this->my_map;
}

void Player::getWeapon() {
	//如果携带武器，创建角色时直接加上武器
	int id = UserDefault::getInstance()->getIntegerForKey("weaponID");
	if (id != 0) {
		Weapon* weapon = Weapon::create(id);
		weapon->setPosition(Vec2(my_sprite->getContentSize().width / 5 * 4, my_sprite->getContentSize().height / 4));
		weapon->setScale(5);
		weaponID = weapon->ID;
		//如果是近战武器
		if (weapon->ID >= SwordID) {
			weapon->showSprite()->setAnchorPoint(Vec2(0, 0.5));
		}
		my_sprite->addChild(weapon, 0, ObjectTag_Weapon);
		weapon->fireSwitch(true);
		//将玩家所在地图传给武器，以便传给子弹
		weapon->my_map = this->my_map;
	}
}

void Player::rotateWeapon(float rotation) {
	Weapon* weapon = dynamic_cast<Weapon*>(my_sprite->getChildByTag(ObjectTag_Weapon));
	weapon->setRotation(rotation);
}

void Player::update(float delta) {
	//玩家移动
	moving();
	//搜索锁定敌人
	searchEnemy();
	//攻击
	if (weaponID != 0) {
		attack();
	}
	//技能
	if (skillCD == 0) {
		skill();
		CDTimer->setPercentage(100);
	}
	else {
		skillCD--;
		CDTimer->setPercentage(static_cast<float> (100 - skillCD / 3));
	}

}

void Player::searchEnemy() {
	int enemyNumber = 0;
	float distance, minDistance = pow(searchingRadius * 5, 2);
	for (int i = 1; i <= 10; i++) {
		Enemy* enemy = dynamic_cast<Enemy*>(my_map->getChildByTag(ObjectTag_Enemy + i - 1));
		//有敌人
		if (enemy != nullptr) {
			Vec2 enemyPosition = enemy->getPosition();
			Vec2 playerPosition = this->getPosition();
			//可以直接平方比较，无需开方
			distance = static_cast<float>(pow(playerPosition.x - enemyPosition.x, 2) + pow(playerPosition.y - enemyPosition.y, 2));

			//寻找最近的敌人
			if (distance < minDistance) {
				minDistance = distance;
				enemyNumber = i;
			}
		}
	}

	//没有敌人
	if (enemyNumber == 0) {
		if (weaponID != 0) {
			rotateWeapon(0);
			enemyMark = false;
		}
		return;
	}
	
	Enemy* enemy = dynamic_cast<Enemy*>(my_map->getChildByTag(ObjectTag_Enemy + enemyNumber - 1));
	Vec2 enemyPosition = enemy->getPosition();
	Vec2 playerPosition = this->getPosition();

	distance = minDistance;
	float rotation = atan((playerPosition.y - enemyPosition.y) / (playerPosition.x - enemyPosition.x)) * 180 / M_PI;

	if (distance < pow(searchingRadius, 2)) {
		//锁定最近的敌人
		enemy->locked();
		enemyMark = true;
		//如果范围内有敌人，始终面向敌人
		if (playerPosition > enemyPosition) {
			my_sprite->setScaleX(-static_cast<float>(0.2));
			//如果有武器，旋转武器
			if (weaponID != 0) {
				rotateWeapon(rotation);
			}
		}
		else {
			my_sprite->setScaleX(static_cast<float>(0.2));
			//如果有武器，旋转武器
			if (weaponID != 0) {
				rotateWeapon(-rotation);
			}
		}
		//对其余敌人解除锁定
		for (int i = 1; i <= 10; i++) {
			Enemy* enemy = dynamic_cast<Enemy*>(my_map->getChildByTag(ObjectTag_Enemy + i - 1));
			if (enemy != nullptr && i != enemyNumber) {
				enemy->unlocked();
			}
		}
	}
	else {
		//超出索敌范围
		enemy->unlocked();
		enemyMark = false;
		if (weaponID != 0) {
			rotateWeapon(0);
		}
	}
}

void Player::attack() {
	Weapon* weapon = dynamic_cast<Weapon*>(my_sprite->getChildByTag(ObjectTag_Weapon));
	if (keyMap[EventKeyboard::KeyCode::KEY_J]) {
		weapon->attackMark = true;
	}
	else {
		weapon->attackMark = false;
	}
}

void Player::showSkillCD(){
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto CD = Sprite::create("character/skillSprite1.png");
	CD->setScale(visibleSize.width / CD->getContentSize().width / 10);
	CD->setOpacity(150);
	CD->setPosition(visibleSize.width - CD->getBoundingBox().size.width, CD->getBoundingBox().size.height);
	my_map->getParent()->addChild(CD);

	CDTimer->setType(ProgressTimer::Type::BAR);
	CDTimer->setBarChangeRate(Vec2(0, 1));
	CDTimer->setMidpoint(Vec2(0, 0));
	CDTimer->setScale(visibleSize.width / CD->getContentSize().width / 10);
	CDTimer->setOpacity(150);
	CDTimer->setPosition(CD->getPosition());
	my_map->getParent()->addChild(CDTimer);
}

void Player::skill() {
	if (keyMap[EventKeyboard::KeyCode::KEY_K]) {
		if (playerID == rangerID) {
			skillCD = 300;//5s
			rangerSkill();
		}
		if (playerID == sorcererID) {
			skillCD = 300;//5s
			sorcererSkill();
		}
		CDTimer->setPercentage(0);
	}
}

void Player::rangerSkill() {
	setSpeed(14);
	my_sprite->setOpacity(150);
	//短暂无敌
	this->removeComponent(this->getPhysicsBody());
	//持续两秒
	this->scheduleOnce(CC_SCHEDULE_SELECTOR(Player::rangerSkillOver), 1);
}
void Player::rangerSkillOver(float dt) {
	setSpeed(7);
	my_sprite->setOpacity(255);


	//重新设置物理刚体
	auto body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setDynamic(false);
	this->setPhysicsBody(body);
}

void Player::sorcererSkill() {
	Sprite* magicCircle = Sprite::create("MagicCircle.png");
	magicCircle->setScale(static_cast<float>(0.01));
	auto amplify = ScaleTo::create(static_cast<float>(0.5), my_sprite->getBoundingBox().size.width / magicCircle->getContentSize().width*2);
	magicCircle->runAction(Sequence::create(amplify, DelayTime::create(static_cast<float>(1)), FadeOut::create(static_cast<float>(0.5)), NULL));
	magicCircle->setPosition(0, -my_sprite->getBoundingBox().size.height / 2);
	this->addChild(magicCircle, -1);

	for (int i = 1; i <= 10; i++) {
		Enemy* enemy = dynamic_cast<Enemy*>(my_map->getChildByTag(ObjectTag_Enemy + i - 1));
		//遍历敌人
		if (enemy != nullptr) {
			sorcererThunder(enemy);
		}
	}
}

void Player::sorcererThunder(Enemy* enemy) {
	auto thunder = Sprite::create("thunderAnimation/1.png");

	auto animation = Animation::create();

	for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "thunderAnimation/%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	//设置两帧间的时间间隔
	animation->setDelayPerUnit(static_cast<float>(0.3));
	thunder->runAction(Animate::create(animation));
	thunder->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.6)), RemoveSelf::create(), NULL));
	thunder->setScale(static_cast<float>(0.6));
	thunder->setPosition(0, thunder->getBoundingBox().size.height / 2);
	enemy->addChild(thunder);
	enemy->changeHP(-10);
}

void Player::recoverDefendce(float dt) {
	if (recoverDefenceTime != 0) {
		recoverDefenceTime--;
	}
	else {
		changeDefendce(1);
	}
}

void Player::recover(float dt) {
	setSpeed(7);
	my_sprite->setColor(Color3B::WHITE);
}

//玩家移动
void Player::moving() {
	if (!playerID) {  //如果未选择无法移动
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
	setBarrierLayer();

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
		//如果范围内没敌人，正常移动
		if (!enemyMark) {
			my_sprite->setScaleX(-static_cast<float>(0.2));
		}
		X -= Speed;
	}
	if (keyMap[down] && !tiledGid_down1 && !tiledGid_down2) {
		Y -= Speed;
	}
	if (keyMap[right] && !tiledGid_right1 && !tiledGid_right2) {
		if (!enemyMark) {
			my_sprite->setScaleX(static_cast<float>(0.2));
		}
		X += Speed;
	}

	//各向同速
	if (X != 0 && Y != 0) {
		X /= static_cast<float>(sqrt(2));
		Y /= static_cast<float>(sqrt(2));
	}

	Vec2 point = this->getPosition();
	point.x += X;
	point.y += Y;
	this->setPosition(point);
	if (playerFollowingMark) {
		my_map->setPosition(visibleSize.width / 2 - point.x, visibleSize.height / 2 - point.y);
	}

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

void Player::setSpeed(int speed) {
	this->Speed = speed;
}

void Player::putIntoMap(TMXTiledMap* map) {
	my_map = map;
	map->addChild(this, map->getLayer("player")->getLocalZOrder(), ObjectTag_Player);
	showSkillCD();
}

void Player::setBarrierLayer() {
	barrier = my_map->getLayer("barrier");
}

Vec2 Player::tileCoordForPosition(Vec2 point) {
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

