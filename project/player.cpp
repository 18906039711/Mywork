#include"player.h"

bool Player::init()
{
	this->scheduleUpdate();
	//����Tag
	this->setTag(ObjectTag_Player);

	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite* playerSprite = Sprite::create();
	
	if (playerID == rangerID) {
		playerSprite = Sprite::create("character/ranger_right.png");
		playerSprite->setScale(static_cast<float>(0.2));
		//�󶨾���ͼ��
		this->bindSprite(playerSprite);
	}
	else if (playerID == sorcererID) {
		playerSprite = Sprite::create("character/sorcerer_right.png");
		playerSprite->setScale(static_cast<float>(0.2));
		this->bindSprite(playerSprite);
	}
	//����վ������
	this->StandAction();
	//�����ƶ��ٶ�
	this->setSpeed(8);

	//�����������
	auto body = PhysicsBody::createBox(playerSprite->getBoundingBox().size);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setDynamic(false);
	this->setPhysicsBody(body);


	this->scheduleUpdate();

	//�����ƶ��¼�����
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

//����
void Player::StandAction() {
	//��������֡
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
	//������֡���ʱ����
	animation->setDelayPerUnit(static_cast<float>(0.3));
	//����ѭ����-1����ѭ��
	animation->setLoops(-1);
	//�����һ֡������ָ�����һ֡
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

//�����������
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
	//�ȿۻ���
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
	//������ʾ
	if (changeValue > 0) {
		std::string Changestr = "+" + std::to_string(changeValue);
		Label* HPLabel = Label::createWithTTF(Changestr, "fonts/arial.ttf", 40);
		HPLabel->runAction(Sequence::create(DelayTime::create(static_cast<float>(1)), RemoveSelf::create(), NULL));
		HPLabel->setTextColor(Color4B::RED);
		HPLabel->setPosition(Vec2(my_sprite->getBoundingBox().size.width / 2, my_sprite->getBoundingBox().size.height / 2));
		this->addChild(HPLabel);
	}
	else if(changeValue < 0) {
		//����
		my_sprite->runAction(Sequence::create(TintTo::create(static_cast<float>(0.1), Color3B::RED),
			TintTo::create(static_cast<float>(0.1), Color3B::WHITE), NULL));

		//��ʱ������stopAction��һֱ��죬�����ú���ǿ�ƻָ�
		//֮���debuffҲ�������������
		this->scheduleOnce(CC_SCHEDULE_SELECTOR(Player::recover), 0.1);

		//���û��׻ظ���ȴʱ��,5s
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
	//������ʾ
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

		//�����˺���Ѫ
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
		//���û��׻ظ���ȴʱ��,5s
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

//����
void Player::getWeapon(Weapon* weapon) {
	//�����������������ȡ��
	if (weaponID != 0) {
		//���������ɾ��
		my_sprite->getChildByTag(ObjectTag_Weapon)->removeFromParentAndCleanup(true);

		//����һ�����ڵ�ͼ��
		Weapon* nowWeapon = Weapon::create(weaponID);
		nowWeapon->my_map = dynamic_cast<TMXTiledMap*>(this->getParent());
		nowWeapon->putIntoMap(this->getPosition());
	}

	this->getParent()->getParent()->removeChildByTag(ObjectTag_Information);
	weapon->removeChildByTag(ObjectTag_WeaponArrow);
	weapon->retain();
	weapon->removeFromParent();
	//����ǽ�ս����
	if (weapon->ID >= SwordID) {
		weapon->showSprite()->setAnchorPoint(Vec2(0, 0.5));
	}
	weapon->setPosition(Vec2(my_sprite->getContentSize().width / 5 * 4, my_sprite->getContentSize().height / 4));
	weapon->setScale(5);
	UserDefault::getInstance()->setIntegerForKey("weaponID", weapon->ID);
	weaponID = weapon->ID;
	my_sprite->addChild(weapon, 0, ObjectTag_Weapon);
	weapon->fireSwitch(true);
	//��������ڵ�ͼ�����������Ա㴫���ӵ�
	weapon->my_map = this->my_map;
}

void Player::getWeapon() {
	//���Я��������������ɫʱֱ�Ӽ�������
	int id = UserDefault::getInstance()->getIntegerForKey("weaponID");
	if (id != 0) {
		Weapon* weapon = Weapon::create(id);
		weapon->setPosition(Vec2(my_sprite->getContentSize().width / 5 * 4, my_sprite->getContentSize().height / 4));
		weapon->setScale(5);
		weaponID = weapon->ID;
		//����ǽ�ս����
		if (weapon->ID >= SwordID) {
			weapon->showSprite()->setAnchorPoint(Vec2(0, 0.5));
		}
		my_sprite->addChild(weapon, 0, ObjectTag_Weapon);
		weapon->fireSwitch(true);
		//��������ڵ�ͼ�����������Ա㴫���ӵ�
		weapon->my_map = this->my_map;
	}
}

void Player::rotateWeapon(float rotation) {
	Weapon* weapon = dynamic_cast<Weapon*>(my_sprite->getChildByTag(ObjectTag_Weapon));
	weapon->setRotation(rotation);
}

void Player::update(float delta) {
	//����ƶ�
	moving();
	//������������
	searchEnemy();
	//����
	if (weaponID != 0) {
		attack();
	}
	//����
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
		//�е���
		if (enemy != nullptr) {
			Vec2 enemyPosition = enemy->getPosition();
			Vec2 playerPosition = this->getPosition();
			//����ֱ��ƽ���Ƚϣ����迪��
			distance = static_cast<float>(pow(playerPosition.x - enemyPosition.x, 2) + pow(playerPosition.y - enemyPosition.y, 2));

			//Ѱ������ĵ���
			if (distance < minDistance) {
				minDistance = distance;
				enemyNumber = i;
			}
		}
	}

	//û�е���
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
		//��������ĵ���
		enemy->locked();
		enemyMark = true;
		//�����Χ���е��ˣ�ʼ���������
		if (playerPosition > enemyPosition) {
			my_sprite->setScaleX(-static_cast<float>(0.2));
			//�������������ת����
			if (weaponID != 0) {
				rotateWeapon(rotation);
			}
		}
		else {
			my_sprite->setScaleX(static_cast<float>(0.2));
			//�������������ת����
			if (weaponID != 0) {
				rotateWeapon(-rotation);
			}
		}
		//��������˽������
		for (int i = 1; i <= 10; i++) {
			Enemy* enemy = dynamic_cast<Enemy*>(my_map->getChildByTag(ObjectTag_Enemy + i - 1));
			if (enemy != nullptr && i != enemyNumber) {
				enemy->unlocked();
			}
		}
	}
	else {
		//�������з�Χ
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
	//�����޵�
	this->removeComponent(this->getPhysicsBody());
	//��������
	this->scheduleOnce(CC_SCHEDULE_SELECTOR(Player::rangerSkillOver), 1);
}
void Player::rangerSkillOver(float dt) {
	setSpeed(7);
	my_sprite->setOpacity(255);


	//���������������
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
		//��������
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
	//������֡���ʱ����
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

//����ƶ�
void Player::moving() {
	if (!playerID) {  //���δѡ���޷��ƶ�
		return;
	}

	auto up = EventKeyboard::KeyCode::KEY_W;
	auto left = EventKeyboard::KeyCode::KEY_A;
	auto down = EventKeyboard::KeyCode::KEY_S;
	auto right = EventKeyboard::KeyCode::KEY_D;

	float X = 0, Y = 0, px, py, width, height;

	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	px = this->getPosition().x;
	py = this->getPosition().y;

	//������ź�ĳ���
	width = (this->showSprite())->getBoundingBox().size.width;
	height = (this->showSprite())->getBoundingBox().size.height;

	//��ȡ�������������ߵĵ�
	Vec2 tiledPos_up1 = tileCoordForPosition(Vec2(px - width / 2 + 10, py + height / 2));
	Vec2 tiledPos_up2 = tileCoordForPosition(Vec2(px + width / 2 - 10, py + height / 2));
	Vec2 tiledPos_down1 = tileCoordForPosition(Vec2(px - width / 2 + 10, py - height / 2));
	Vec2 tiledPos_down2 = tileCoordForPosition(Vec2(px + width / 2 - 10, py - height / 2));
	Vec2 tiledPos_left1 = tileCoordForPosition(Vec2(px - width / 2, py - height / 2 + 10));
	Vec2 tiledPos_left2 = tileCoordForPosition(Vec2(px - width / 2, py + height / 2 - 10));
	Vec2 tiledPos_right1 = tileCoordForPosition(Vec2(px + width / 2, py - height / 2 + 10));
	Vec2 tiledPos_right2 = tileCoordForPosition(Vec2(px + width / 2, py + height / 2 - 10));

	//�����ϰ���
	setBarrierLayer();

	//ͨ�������ȡTiledMap�ϸ��ӵ�Ψһ��ʶ
	int tiledGid_up1 = barrier->getTileGIDAt(tiledPos_up1);
	int tiledGid_up2 = barrier->getTileGIDAt(tiledPos_up2);
	int tiledGid_down1 = barrier->getTileGIDAt(tiledPos_down1);
	int tiledGid_down2 = barrier->getTileGIDAt(tiledPos_down2);
	int tiledGid_left1 = barrier->getTileGIDAt(tiledPos_left1);
	int tiledGid_left2 = barrier->getTileGIDAt(tiledPos_left2);
	int tiledGid_right1 = barrier->getTileGIDAt(tiledPos_right1);
	int tiledGid_right2 = barrier->getTileGIDAt(tiledPos_right2);

	//���������£��Ҿ���ı߲����ϰ�����
	if (keyMap[up] && !tiledGid_up1 && !tiledGid_up2) {
		Y += Speed;
	}
	if (keyMap[left] && !tiledGid_left1 && !tiledGid_left2) {
		//�����Χ��û���ˣ������ƶ�
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

	//����ͬ��
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

