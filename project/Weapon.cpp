#include"Weapon.h"

bool Weapon::init()
{
	//设置Tag
	this->setTag(ObjectTag_Weapon);

	std::string weaponID = "Weapon/" + std::to_string(ID) + ".png";
	this->bindSprite(Sprite::create(weaponID));
	
	auto body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	this->addComponent(body);
	
	/* 碰撞检测 */
	EventListenerPhysicsContact* contactListenerWeapon = EventListenerPhysicsContact::create();
	contactListenerWeapon->onContactBegin = CC_CALLBACK_1(Weapon::onContactBegin, this);
	contactListenerWeapon->onContactPreSolve = CC_CALLBACK_1(Weapon::onContactPreSolve, this);
	contactListenerWeapon->onContactSeparate = CC_CALLBACK_1(Weapon::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenerWeapon, this);


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
}

void Weapon::removeInfomation() {
	Node* weaponInformation = this->getParent()->getParent()->getChildByTag(ObjectTag_weaponInformation);
	weaponInformation->runAction(Sequence::create(MoveBy::create(static_cast<float>(0.2), Vec2(0, -weaponInformation->getBoundingBox().size.height))
	, RemoveSelf::create(), NULL));
	
}

