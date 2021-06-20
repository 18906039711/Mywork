#include"TreasureChest.h"

bool TreasureChest::init()
{
	//设置Tag
	this->setTag(ObjectTag_TreasureChest);

	if (ID == 1) {
		this->bindSprite(Sprite::create("TreasureChest/closed1.png"));
	}
	else if (ID == 2) {
		this->bindSprite(Sprite::create("TreasureChest/closed2.png"));
	}

	auto body = PhysicsBody::createBox(my_sprite->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	this->addComponent(body);

	/* 碰撞检测 */
	EventListenerPhysicsContact* ChestContactListener = EventListenerPhysicsContact::create();
	ChestContactListener->onContactBegin = CC_CALLBACK_1(TreasureChest::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ChestContactListener, this);

	std::srand((unsigned)time(0));
	int null = static_cast<int>(rand_0_1());

	appearAction();

	return true;
}


TreasureChest* TreasureChest::create(int m_ID) {
	auto Chest = new TreasureChest();

	//在碰撞检测中加入带物理刚体的对象无法正确设置其位置
	//且每个对象分别判定，，所以不能放在init里
	Chest->schedule(CC_SCHEDULE_SELECTOR(TreasureChest::ifChestOpened), static_cast<float>(0.1));

	Chest->ID = m_ID;
	if (Chest && Chest->init()) {
		Chest->autorelease();
		return Chest;
	}
	CC_SAFE_DELETE(Chest);
	return nullptr;
}

bool TreasureChest::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	//防止节点为nullptr闪退
	if (nodeA == nullptr || nodeB == nullptr) {
		CCLOG("node=nullptr");
		return true;
	}

	Node* ChestNode = NULL;    /* 宝箱对象 */
	Node* PlayerNode = NULL;

	//判断两个碰撞对象
	if (nodeA->getTag() == ObjectTag_TreasureChest){
		ChestNode = nodeA;
		PlayerNode = nodeB;
	}
	else{
		ChestNode = nodeB;
		PlayerNode = nodeA;
	}

	//绑定碰撞的玩家和宝箱
	Player*player = dynamic_cast<Player*>(PlayerNode);
	TreasureChest* Chest = dynamic_cast<TreasureChest*>(ChestNode);
	
	
	//玩家碰到宝箱时才打开
	if (PlayerNode->getTag() == ObjectTag_Player && ChestNode->getTag() == ObjectTag_TreasureChest) {
		if (Chest->ID == 1) {
			Chest->bindSprite(Sprite::create("TreasureChest/opened1.png"));
		}
		else if (Chest->ID == 2) {
			Chest->bindSprite(Sprite::create("TreasureChest/opened2.png"));
			
			//随机生成1-3个coin，以及3-5个MpFactor
			int CoinNum = static_cast<int>(rand_0_1() * 3) + 1;
			int MpFactorNum = static_cast<int>(rand_0_1() * 3) + 3;
			for (int i = 0; i < CoinNum; i++) {
				Coin* coin = Coin::create();
				Chest->addChild(coin);
			}
			for (int i = 0; i < MpFactorNum; i++) {
				Sprite* MpFactor = Sprite::create("Coin/MPFactor.png");
				MpFactor->setScale(static_cast<float>(0.3));
				MpFactor->runAction(Sequence::create(MoveTo::create(static_cast<float>(0.2), Vec2(rand_0_1() * 400 - 200, rand_0_1() * 400 - 200)),
					DelayTime::create(static_cast<float>(1)), FadeOut::create(static_cast<float>(0.5)), NULL));
				Chest->addChild(MpFactor);
			}
			player->changeMP(5 * MpFactorNum);
		}
		Chest->ifOpened = true;
		Chest->removeAllComponents();
	}
	return true;
}

void TreasureChest::ifChestOpened(float dt) {
	if (this->ifOpened) {
		if (this->ID == 1) {
			//随机生成武器
			int WeaponID = static_cast<int>(rand_0_1() * 1000) % 5 + AK47ID;
			Weapon* weapon = Weapon::create(WeaponID);
			weapon->runAction(MoveBy::create(static_cast<float>(0.3), Vec2(0, weapon->showSprite()->getContentSize().height)));
			weapon->fireSwitch(false);
			weapon->my_map = dynamic_cast<TMXTiledMap*>(this->getParent());
			weapon->putIntoMap(this->getPosition());
		}
		if (this->ID == 2) {
			this->runAction(Sequence::create(DelayTime::create(static_cast<float>(2)), RemoveSelf::create(), NULL));
		}
		this->unschedule(CC_SCHEDULE_SELECTOR(TreasureChest::ifChestOpened));
	}
}

void TreasureChest::appearAction() {

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