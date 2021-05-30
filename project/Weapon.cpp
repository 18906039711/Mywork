#include"Weapon.h"

bool Weapon::init()
{
	//����Tag
	this->setTag(ObjectTag_Weapon);

	std::string weaponID = "Weapon/" + std::to_string(ID) + ".png";
	this->bindSprite(Sprite::create(weaponID));
	
	auto body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	this->addComponent(body);
	
	/* ��ײ��� */
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

	//����ײ����Һͱ���
	Player* player = dynamic_cast<Player*>(nodeA);
	Weapon* weapon = dynamic_cast<Weapon*>(nodeB);

	//��ֹ�ڵ�Ϊnullptr����
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//�����������ʱ
	if (nodeA->getTag() == ObjectTag_Player && nodeB->getTag() == ObjectTag_Weapon) {
		weapon->showInfomation();
	}
	return true;
}

bool Weapon::onContactPreSolve(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	//����ײ����Һͱ���
	Player* player = dynamic_cast<Player*>(nodeA);
	Weapon* weapon = dynamic_cast<Weapon*>(nodeB);

	//��ֹ�ڵ�Ϊnullptr����
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//����������ص�ʱ����ʰȡ
	if (nodeA->getTag() == ObjectTag_Player && nodeB->getTag() == ObjectTag_Weapon) {
		player->getWeapon(weapon);
	}
	return true;
}

bool Weapon::onContactSeparate(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	//����ײ����Һͱ���
	Player* player = dynamic_cast<Player*>(nodeA);
	Weapon* weapon = dynamic_cast<Weapon*>(nodeB);

	//��ֹ�ڵ�Ϊnullptr����
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//����뿪������
	if (nodeA->getTag() == ObjectTag_Player && nodeB->getTag() == ObjectTag_Weapon) {
		weapon->removeInfomation();
	}
	return true;
}

void Weapon::showInfomation() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//��Ϣ��
	Sprite* weaponInformation = Sprite::create("weapon/weaponInformation.png");
	//�����map�У���������getparent
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

