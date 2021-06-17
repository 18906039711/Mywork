#include"Enemy.h"

bool Enemy::init()
{

	this->scheduleUpdate();

	//����Tag
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
	if (ID == Dummy) { //�޷��ƶ��ĵ��˾�������� 
		return;
	}

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
	Vec2 tiledPos_up = tileCoordForPosition(Vec2(px, py + height / 2));
	Vec2 tiledPos_down = tileCoordForPosition(Vec2(px, py - height / 2));
	Vec2 tiledPos_left = tileCoordForPosition(Vec2(px - width / 2, py));
	Vec2 tiledPos_right = tileCoordForPosition(Vec2(px + width / 2, py));

	//�����ϰ���
	setBarrierLayer();

	//ͨ�������ȡTiledMap�ϸ��ӵ�Ψһ��ʶ
	int tiledGid_up = barrier->getTileGIDAt(tiledPos_up);
	int tiledGid_down = barrier->getTileGIDAt(tiledPos_down);
	int tiledGid_left = barrier->getTileGIDAt(tiledPos_left);
	int tiledGid_right = barrier->getTileGIDAt(tiledPos_right);


	//���������£��Ҿ���ı߲����ϰ�����
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

	//ת���ɵ�λ����
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
	//����ģʽ�£�����Ƶ�ʱ��
	if (hardMark) {
		attack();
	}
}

void Enemy::towardsPlayerMove(float delta) {
	if (ID == Dummy) { //�޷��ƶ��ĵ��˾�������� 
		return;
	}

	float X = 0, Y = 0, px, py, width, height;

	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Player* player = dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player));
	if (player == nullptr) {
		return;
	}
	px = player->getPosition().x;
	py = player->getPosition().y;

	//������ź�ĳ���
	width = (this->showSprite())->getBoundingBox().size.width;
	height = (this->showSprite())->getBoundingBox().size.height;

	//��ȡ�������������ߵĵ�
	Vec2 tiledPos_up = tileCoordForPosition(Vec2(px, py + height / 2));
	Vec2 tiledPos_down = tileCoordForPosition(Vec2(px, py - height / 2));
	Vec2 tiledPos_left = tileCoordForPosition(Vec2(px - width / 2, py));
	Vec2 tiledPos_right = tileCoordForPosition(Vec2(px + width / 2, py));

	//�����ϰ���
	setBarrierLayer();

	//ͨ�������ȡTiledMap�ϸ��ӵ�Ψһ��ʶ
	int tiledGid_up = barrier->getTileGIDAt(tiledPos_up);
	int tiledGid_down = barrier->getTileGIDAt(tiledPos_down);
	int tiledGid_left = barrier->getTileGIDAt(tiledPos_left);
	int tiledGid_right = barrier->getTileGIDAt(tiledPos_right);


	//���������£��Ҿ���ı߲����ϰ�����
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

	//�������
	float distance = pow(this->getPosition().x - px, 2) + pow(this->getPosition().y - py, 2);
	Vec2 destination;

	//�����ɫ���ڹ�����Χ������ƶ�
	if (distance > pow(700, 2)) {
		//ת���ɵ�λ����
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
		//���ƶ�ʱ����
	    //����ģʽ�£�����Ƶ�ʱ��
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
	//��������֡
	auto animation = Animation::create();

	if (ID != Dummy) {
		for (int i = 1; i <= 5; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "appearAction/appear%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	//������֡���ʱ����
	animation->setDelayPerUnit(static_cast<float>(0.1));
	//����ѭ��1��
	animation->setLoops(1);
	appearSprite->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), NULL));

	//��������
	my_sprite->setOpacity(0);
	this->addChild(appearSprite);
	my_sprite->runAction(FadeIn::create(static_cast<float>(0.5)));
}

void Enemy::standAction() {
	//��������֡
	auto animation = Animation::create();

	if (ID != Dummy) {
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "Enemy/%d/stand%d.png", ID, i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}

	//������֡���ʱ����
	animation->setDelayPerUnit(static_cast<float>(0.3));
	//����ѭ����-1����ѭ��
	animation->setLoops(-1);
	//�����һ֡������ָ�����һ֡
	animation->setRestoreOriginalFrame(true);

	my_sprite->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.5)), Animate::create(animation), NULL));
}

void Enemy::attackAction() {
	my_sprite->stopAllActions();
	//��������֡
	auto animation = Animation::create();

	if (ID != Dummy) {
		for (int i = 1; i <= 4; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "Enemy/%d/attack%d.png", ID, i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}

	//������֡���ʱ����
	animation->setDelayPerUnit(static_cast<float>(0.1));
	animation->setLoops(1);

	my_sprite->runAction(Animate::create(animation));
}

void Enemy::attack() {
	attackAction();
	standAction();

	Player* player = dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player));
	//���û�����,������
	if (player == nullptr) {
		return;
	}

	Vec2 enemyPosition = this->getPosition();
	Vec2 playerPosition = player->getPosition();

	float rotation = atan((enemyPosition.y - playerPosition.y) / (enemyPosition.x - playerPosition.x)) * 180 / M_PI;

	Vec2 point = this->getPosition();

	//���������ӵ�
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
	//Զ��
	if (ID < piggyEnemy) {
		if (my_sprite->getScaleX() > 0) {
			bullet->putIntoMap(point, rotation);
		}
		else {
			bullet->putIntoMap(point, rotation + 180);
		}
	}
	//��ս������Ч��ֱ�Ӽ���ͼ���ϣ����ڹ���tag����ͬ�����ý�����ڵ�ͼ��
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
		//Ŀ����Ϊ�˻�ȡ���������ص�Ƕ��޹�
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
		//����ͼ��
		std::string deadStr = "Enemy/" + std::to_string(ID) + "/dead.png";
		auto deadSprite = Sprite::create(deadStr);
		if (deadSprite != nullptr) {
			deadSprite->setPosition(this->getPosition());
			my_map->addChild(deadSprite, my_map->getLayer("player")->getLocalZOrder() - 2);
			if (my_sprite->getScaleX() < 0) {
				deadSprite->setScaleX(-1);
			}
		}

		//�������0-1��coin���Լ�0-1��MpFactor
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