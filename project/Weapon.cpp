#include"Weapon.h"

bool Weapon::init()
{
	this->scheduleUpdate();
	//设置Tag
	this->setTag(ObjectTag_Weapon);

	std::string weaponID = "Weapon/" + std::to_string(ID) + "/Weapon.png";
	Sprite* weaponSprite = Sprite::create(weaponID);
	weaponSprite->setScale(static_cast<float>(0.7));
	this->bindSprite(weaponSprite);
	setInformation();
	
	//枪口火焰
	std::string fireStr = "Weapon/" + std::to_string(ID) + "/Fire.png";
	fire = Sprite::create(fireStr);
	if (fire!=nullptr){
		fire->setPosition(my_sprite->getBoundingBox().size.width / 2 + fire->getBoundingBox().size.width / 2, 0);
		this->addChild(fire);
		fire->setOpacity(0);
	}
	

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
void Weapon::update(float delta) {
	//如果在角色手上
	if (this->getParent()->getParent()->getTag()== ObjectTag_Player) {

	}
	//如果在地图上上
	else {
		//获得武器图片的Rect
		Vec2 weaponPoint = Vec2(this->getPosition().x - my_sprite->getBoundingBox().size.width / 2,
			this->getPosition().y - my_sprite->getBoundingBox().size.height / 2);
		Rect weapon = Rect(weaponPoint, this->my_sprite->getBoundingBox().size);
		Player* player = dynamic_cast<Player*>(this->getParent()->getChildByTag(ObjectTag_Player));
		auto playerSize = player->my_sprite->getBoundingBox().size;
		auto playerRect = Rect(Vec2(player->getPosition().x - player->my_sprite->getBoundingBox().size.width / 2,
			player->getPosition().y - player->my_sprite->getBoundingBox().size.height / 2), playerSize);
		if (weapon.intersectsRect(playerRect)) {
			this->showInfomation();
			player->getWeapon(this);
		}
		else {
			this->removeInfomation();
		}
	}
}

void Weapon::fireSwitch(bool mark = false) {
	if (mark) {
		this->schedule(CC_SCHEDULE_SELECTOR(Weapon::attack), static_cast<float>(0.1));
	}
	else {
		this->unschedule(CC_SCHEDULE_SELECTOR(Weapon::attack));
	}
}

void Weapon::showInfomation() {
	//如果已经存在直接返回
	if (this->getParent()->getParent()->getChildByTag(ObjectTag_weaponInformation) != nullptr) {
		return;
	}
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

	Sprite* arrow = Sprite::create("Weapon/arrow.png");
	arrow->setScale(static_cast<float>(0.3));
	arrow->setPosition(0, this->showSprite()->getContentSize().height / 2 + 30);
	this->addChild(arrow, 0, ObjectTag_WeaponArrow);
	auto name = Label::createWithTTF(this->name, "fonts/arial.ttf", 120);
	name->enableOutline(Color4B::WHITE, 2);
	name->setPosition(arrow->getContentSize().width / 2, arrow->getContentSize().height * 2);
	arrow->addChild(name);
}

void Weapon::removeInfomation() {
	Sprite* weaponInformation = dynamic_cast<Sprite*>(this->getParent()->getParent()->getChildByTag(ObjectTag_weaponInformation));
	if (weaponInformation != nullptr) {
		weaponInformation->runAction(Sequence::create(MoveBy::create(static_cast<float>(0.2), Vec2(0, -weaponInformation->getBoundingBox().size.height))
			, RemoveSelf::create(), NULL));
	}
	if (this->getChildByTag(ObjectTag_WeaponArrow) != nullptr) {
		this->removeChildByTag(ObjectTag_WeaponArrow);
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

void Weapon::putIntoMap(Vec2 point) {
	my_map->addChild(this,5);
	this->setPosition(point);
}

void Weapon::attack(float dt) {
	//计算攻击间隔
	static float count = firingRate;

	count += 0.1;
	if (count >= firingRate) {
		attackMark1 = true;
	}
	//获取角色信息
	Player* player = dynamic_cast<Player*>(this->my_map->getChildByTag(ObjectTag_Player));

	/*满足按J键(attackMark), 武器不在发射冷却时间(attackMark1), MP大于蓝耗(player->MP >= MPConsumption)
	才能发射子弹*/
	if (attackMark && attackMark1 && (player->MP >= MPConsumption)) {
		//耗蓝
		player->changeMP(-MPConsumption);

		//获取玩家的坐标（相对于地图）
		Vec2 point = player->getPosition();
		
		//射击武器
		if (ID < SwordID) {
			//计算枪口位置
			if (player->showSprite()->getScaleX() > 0) {
				point.x += player->showSprite()->getBoundingBox().size.width * 3 / 10;
				point.x += (my_sprite->getBoundingBox().size.width / 2 + fire->getBoundingBox().size.width / 2) * cos(this->getRotation() / 180 * M_PI);
			}
			else {
				point.x -= player->showSprite()->getBoundingBox().size.width * 3 / 10;
				point.x -= (my_sprite->getBoundingBox().size.width / 2 + fire->getBoundingBox().size.width / 2) * cos(this->getRotation() / 180 * M_PI);
			}
			point.y -= player->showSprite()->getBoundingBox().size.height / 4;
			point.y -= (my_sprite->getBoundingBox().size.width / 2 + fire->getBoundingBox().size.width / 2) * sin(this->getRotation() / 180 * M_PI);

			//发射子弹
			//散弹多发
			if (ID == ShootGunID) {
				for (int i = 0; i < 4; i++) {
					auto bullet = Bullet::create(ID);
					bullet->my_map = this->my_map;
					bullet->putIntoMap(point, this->getRotation() + rand_0_1() * 20 - 10);
				}
			}
			else {
				auto bullet = Bullet::create(ID);
				bullet->my_map = this->my_map;
				bullet->putIntoMap(point, this->getRotation() + rand_0_1() * 0);
			}


			//喷射火焰及后坐动画
			this->runAction(Sequence::create(MoveBy::create(static_cast<float>(0.1), Vec2(-50, 0)),
				MoveBy::create(static_cast<float>(0.1), Vec2(50, 0)), NULL));
			fire->runAction(Sequence::create(FadeIn::create(static_cast<float>(0.01)), DelayTime::create(static_cast<float>(0.08)),
				FadeOut::create(static_cast<float>(0.01)), NULL));
		}
		//近战武器
		else {
			//计算枪口位置
			if (player->showSprite()->getScaleX() > 0) {
				point.x += player->showSprite()->getBoundingBox().size.width * 3 / 10;
				point.x += my_sprite->getBoundingBox().size.width / 2* cos(this->getRotation() / 180 * M_PI);
			}
			else {
				point.x -= player->showSprite()->getBoundingBox().size.width * 3 / 10;
				point.x -= my_sprite->getBoundingBox().size.width / 2 * cos(this->getRotation() / 180 * M_PI);
			}
			point.y -= player->showSprite()->getBoundingBox().size.height / 4;
			point.y -= my_sprite->getBoundingBox().size.width / 2 * sin(this->getRotation() / 180 * M_PI);

			my_sprite->runAction(Sequence::create(RotateBy::create(static_cast<float>(0.01), -110),
				RotateBy::create(static_cast<float>(0.15), 220), RotateBy::create(static_cast<float>(0.2), -110), NULL));

			auto bullet = Bullet::create(ID);
			bullet->my_map = this->my_map;
			bullet->putIntoMap(point, this->getRotation() + rand_0_1() * 0);

		}
		
		
		
		//攻击间隔
		attackMark1 = false;
		count = 0;
	}

}





