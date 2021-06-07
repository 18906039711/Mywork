#include"Weapon.h"

bool Weapon::init()
{
	//设置Tag
	this->setTag(ObjectTag_Weapon);

	std::string weaponID = "Weapon/" + std::to_string(ID) + ".png";
	Sprite* weaponSprite = Sprite::create(weaponID);
	weaponSprite->setScale(static_cast<float>(0.7));
	this->bindSprite(weaponSprite);
	setInformation();

	auto body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	this->addComponent(body);


	return true;
}

Weapon* Weapon::create(int m_ID) {
	auto weapon = new Weapon();
	weapon->ID = m_ID;

	if (weapon && weapon->init()) {
		weapon->autorelease();
		return weapon;
	}
	CC_SAFE_DELETE(weapon);
	return nullptr;
}

void Weapon::fireSwitch(bool mark = false) {
	if (mark) {
		this->schedule(CC_SCHEDULE_SELECTOR(Weapon::attack), static_cast<float>(0.1));
		_eventDispatcher->removeEventListener(contactListenerWeapon);
	}
	else {
		this->unschedule(CC_SCHEDULE_SELECTOR(Weapon::attack));
		contactListenerWeapon->onContactBegin = CC_CALLBACK_1(Weapon::onContactBegin, this);
		contactListenerWeapon->onContactPreSolve = CC_CALLBACK_1(Weapon::onContactPreSolve, this);
		contactListenerWeapon->onContactSeparate = CC_CALLBACK_1(Weapon::onContactSeparate, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenerWeapon, this);
	}
}

bool Weapon::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	//绑定碰撞的玩家和宝箱
	Player* player = dynamic_cast<Player*>(nodeA);
	Weapon* weapon = dynamic_cast<Weapon*>(nodeB);

	//防止节点为nullptr闪退
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//玩家碰到武器时
	if (nodeA->getTag() == ObjectTag_Player && nodeB->getTag() == ObjectTag_Weapon) {
		weapon->showInfomation();
		Sprite* arrow = Sprite::create("Weapon/arrow.png");
		arrow->setScale(static_cast<float>(0.3));
		arrow->setPosition(0, weapon->showSprite()->getContentSize().height / 2);
		weapon->addChild(arrow, 0, ObjectTag_WeaponArrow);
		auto name = Label::createWithTTF(weapon->name, "fonts/arial.ttf", 120);
		name->enableOutline(Color4B::WHITE, 2);
		name->setPosition(arrow->getContentSize().width / 2, arrow->getContentSize().height * 2);
		arrow->addChild(name);
	}
	return true;
}

bool Weapon::onContactPreSolve(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	//绑定碰撞的玩家和宝箱
	Player* player = dynamic_cast<Player*>(nodeA);
	Weapon* weapon = dynamic_cast<Weapon*>(nodeB);

	//防止节点为nullptr闪退
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//玩家与武器重叠时可以拾取
	if (nodeA->getTag() == ObjectTag_Player && nodeB->getTag() == ObjectTag_Weapon) {
		player->getWeapon(weapon);
	}
	return true;
}

bool Weapon::onContactSeparate(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	//绑定碰撞的玩家和宝箱
	Player* player = dynamic_cast<Player*>(nodeA);
	Weapon* weapon = dynamic_cast<Weapon*>(nodeB);

	//防止节点为nullptr闪退
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//玩家离开武器后
	if (nodeA->getTag() == ObjectTag_Player && nodeB->getTag() == ObjectTag_Weapon) {
		weapon->removeInfomation();
		weapon->removeChildByTag(ObjectTag_WeaponArrow);
	}
	return true;
}

void Weapon::showInfomation() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//信息板
	Sprite* weaponInformation = Sprite::create("weapon/weaponInformation.png");
	//添加在map中，运用两次getparent
	this->getParent()->getParent()->addChild(weaponInformation, 10, ObjectTag_weaponInformation);;
	weaponInformation->setScale(visibleSize.width / weaponInformation->getContentSize().width / 4);
	weaponInformation->setPosition(visibleSize.width / 2, -weaponInformation->getBoundingBox().size.height / 2);
	weaponInformation->runAction(MoveBy::create(static_cast<float>(0.2), Vec2(0, weaponInformation->getBoundingBox().size.height)));

	//数字显示
	std::string damagestr = std::to_string(damage);
	Label* damageLabel = Label::createWithTTF(damagestr, "fonts/arial.ttf", 80);
	weaponInformation->addChild(damageLabel);
	damageLabel->setPosition(Vec2(weaponInformation->getContentSize().width / 4, weaponInformation->getContentSize().height / 2));

	std::string MPConsumptionstr = std::to_string(MPConsumption);
	Label* MPConsumptionLabel = Label::createWithTTF(MPConsumptionstr, "fonts/arial.ttf", 80);
	weaponInformation->addChild(MPConsumptionLabel);
	MPConsumptionLabel->setPosition(Vec2(weaponInformation->getContentSize().width / 2+20, weaponInformation->getContentSize().height / 2));

	char temp[10];
	sprintf(temp, "%.2f", firingRate);
	std::string firingRatestr = temp;
	Label* firingRateLabel = Label::createWithTTF(firingRatestr, "fonts/arial.ttf", 80);
	weaponInformation->addChild(firingRateLabel);
	firingRateLabel->setPosition(Vec2(weaponInformation->getContentSize().width / 6 * 5+10, weaponInformation->getContentSize().height / 2));
}

void Weapon::removeInfomation() {
	Sprite* weaponInformation = dynamic_cast<Sprite*>(this->getParent()->getParent()->getChildByTag(ObjectTag_weaponInformation));
	if (weaponInformation != nullptr) {
		weaponInformation->runAction(Sequence::create(MoveBy::create(static_cast<float>(0.2), Vec2(0, -weaponInformation->getBoundingBox().size.height))
			, RemoveSelf::create(), NULL));
	}
}

void Weapon::setInformation() {
	char damageStr[50];
	sprintf(damageStr, "%d_damage", ID);
	damage = UserDefault::getInstance()->getIntegerForKey(damageStr);
	char MPConsumptionStr[50];
	sprintf(MPConsumptionStr, "%d_MPConsumption", ID);
	MPConsumption = UserDefault::getInstance()->getIntegerForKey(MPConsumptionStr);
	char firingRateStr[50];
	sprintf(firingRateStr, "%d_firingRate", ID);
	firingRate = UserDefault::getInstance()->getFloatForKey(firingRateStr);
	char nameStr[50];
	sprintf(nameStr, "%d_name", ID);
	name = UserDefault::getInstance()->getStringForKey(nameStr);
}

void Weapon::attack(float dt) {
	static float count = 0;
	if (attackMark&& attackMark1) {
		//获取枪中心的坐标（相对于地图）
		Player* player = dynamic_cast<Player*>(this->getParent()->getParent());
		Vec2 point = player->getPosition();
		
		//计算枪口位置
		if (player->showSprite()->getScaleX() > 0) {
			point.x += player->showSprite()->getBoundingBox().size.width * 3 / 10;
			point.x += my_sprite->getBoundingBox().size.width / 2 * cos(this->getRotation() / 180 * M_PI);
		}
		else {
			point.x -= player->showSprite()->getBoundingBox().size.width * 3 / 10;
			point.x -= my_sprite->getBoundingBox().size.width / 2 * cos(this->getRotation() / 180 * M_PI);
		}

		point.y -= player->showSprite()->getBoundingBox().size.height / 4;
		point.y -= my_sprite->getBoundingBox().size.width / 2 * sin(this->getRotation() / 180 * M_PI);

		auto bullet = Bullet::create(ID);
		bullet->my_map = this->my_map;
		bullet->putIntoMap(point, this->getRotation());

		//攻击间隔
		attackMark1 = false;
		count = 0;
	}

	count += 0.1;
	if (count >= firingRate) {
		attackMark1 = true;
	}

}





