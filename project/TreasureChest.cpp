﻿#include"TreasureChest.h"

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
	EventListenerPhysicsContact* contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(TreasureChest::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);



	return true;
}


TreasureChest* TreasureChest::create(int m_ID) {
	auto Chest = new TreasureChest();
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

	//Node* ChestNode = NULL;    /* 宝箱对象 */
	//Node* other = NULL;

	////判断两个碰撞对象
	//if (nodeA->getTag() == ObjectTag_TreasureChest)
	//{
	//	ChestNode = nodeA;
	//	other = nodeB;
	//}
	//else if (nodeB->getTag() == ObjectTag_TreasureChest)
	//{
	//	ChestNode = nodeB;
	//	other = nodeA;
	//}

	//绑定碰撞的玩家和宝箱
	Player*player = dynamic_cast<Player*>(nodeA);
	TreasureChest* Chest = dynamic_cast<TreasureChest*>(nodeB);
	
	//防止节点为nullptr闪退
	if (nodeA == nullptr || nodeB == nullptr) {
		return true;
	}

	//玩家碰到宝箱时才打开
	if (nodeA->getTag() == ObjectTag_Player && nodeB->getTag() == ObjectTag_TreasureChest) {
		if (Chest->ID == 1) {
			Chest->bindSprite(Sprite::create("TreasureChest/opened1.png"));
		}
		else if (Chest->ID == 2) {
			Chest->bindSprite(Sprite::create("TreasureChest/opened2.png"));
			
			//随机生成1-3个coin，以及3-5个MpFactor
			std::srand((unsigned)time(0));
			int null = rand_0_1();
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
					DelayTime::create(static_cast<float>(1)), FadeOut::create(static_cast<float>(2)), NULL));
				Chest->addChild(MpFactor);
				player->changeMP(5);
			}
		}
		Chest->ifOpened = true;
		Chest->removeAllComponents();
	}
	return true;
}

