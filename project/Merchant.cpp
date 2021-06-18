#include"Merchant.h"

bool Merchant::init()
{
	this->scheduleUpdate();
	//设置Tag
	this->setTag(ObjectTag_Merchant);

	std::string MerchantID = "Merchant/" + std::to_string(ID) + ".png";
	Sprite* MerchantSprite = Sprite::create(MerchantID);
	this->bindSprite(MerchantSprite);
	setInformation();

	buyThing->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buyThing, this);

	return true;
}

Merchant* Merchant::create(int m_ID) {
	auto merchant = new Merchant();
	merchant->ID = m_ID;

	if (merchant && merchant->init()) {
		merchant->autorelease();
		return merchant;
	}
	CC_SAFE_DELETE(merchant);
	return nullptr;
}
void Merchant::update(float delta) {
	//获得图片的Rect,Rect设置的点在左下角
	Vec2 merchantPoint = Vec2(this->getPosition().x - my_sprite->getBoundingBox().size.width / 2,
		this->getPosition().y - my_sprite->getBoundingBox().size.height / 2);
	Rect merchantRect = Rect(merchantPoint, this->my_sprite->getBoundingBox().size);
	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByTag(ObjectTag_Player));
	if (player == nullptr) {
		return;
	}
	auto playerSize = player->showSprite()->getBoundingBox().size;
	auto playerRect = Rect(Vec2(player->getPosition().x - player->showSprite()->getBoundingBox().size.width / 2,
		player->getPosition().y - player->showSprite()->getBoundingBox().size.height / 2), playerSize);

	if (merchantRect.intersectsRect(playerRect)) {
		this->showInfomation();
		buyThing->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event)
		{
			if (code == EventKeyboard::KeyCode::KEY_J && player != nullptr) {
				if (!priceMark) {
					showPrice();
					priceMark = true;
				}
				else{
					deal();
					priceMark = false;
				}
			}
		};
	}
	else {
		priceMark = false;
		this->removeInfomation();
		buyThing->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {};
	}
}

void Merchant::setInformation() {
	char priceStr[50];
	sprintf(priceStr, "%d_price", ID);
	price = UserDefault::getInstance()->getIntegerForKey(priceStr);
	char nameStr[50];
	sprintf(nameStr, "%d_name", ID);
	name = UserDefault::getInstance()->getStringForKey(nameStr);
}

void Merchant::showInfomation() {
	//如果已经存在直接返回
	if (this->getChildByTag(ObjectTag_Information) != nullptr) {
		return;
	}
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* arrow = Sprite::create("potion/arrow.png");
	arrow->setScale(static_cast<float>(0.3));
	arrow->setPosition(0, this->showSprite()->getContentSize().height / 2 + 30);

	this->addChild(arrow, 0, ObjectTag_Information);
	auto name = Label::createWithTTF(this->name, "fonts/arial.ttf", 120);
	name->enableOutline(Color4B::WHITE, 2);
	name->setPosition(arrow->getContentSize().width / 2, arrow->getContentSize().height * 2);
	arrow->addChild(name);
}

void Merchant::removeInfomation() {
	Sprite* potionInformation = dynamic_cast<Sprite*>(this->getChildByTag(ObjectTag_Information));
	if (potionInformation != nullptr) {
		potionInformation->runAction(RemoveSelf::create());
	}
}

void Merchant::showPrice() {
	this->getChildByTag(ObjectTag_Information)->removeAllChildren();
	auto price = Label::createWithTTF(std::to_string(this->price), "fonts/arial.ttf", 120);
	price->setTextColor(Color4B::YELLOW);
	price->enableOutline(Color4B::YELLOW, 2);
	price->setPosition(this->getChildByTag(ObjectTag_Information)->getContentSize().width / 2,
		this->getChildByTag(ObjectTag_Information)->getContentSize().height * 2);
	this->getChildByTag(ObjectTag_Information)->addChild(price);
}

void Merchant::givePotion() {
	Potion* potion=Potion::create(static_cast<int>(rand_0_1() * 1000) % 2 + ObjectTag_HPPotion);
	potion->setPosition(this->getPosition().x, this->getPosition().y - this->my_sprite->getContentSize().height / 3);
	potion->runAction(MoveBy::create(static_cast<float>(0.5), Vec2(0, -potion->showSprite()->getContentSize().height)));
	potion->putIntoMap(my_map);
}

void Merchant::deal() {
	//钱够
	if (UserDefault::getInstance()->getIntegerForKey("CoinNumber", 0) >= price) {
		givePotion();
		int coinNumber = UserDefault::getInstance()->getIntegerForKey("CoinNumber") - price;
		UserDefault::getInstance()->setIntegerForKey("CoinNumber", coinNumber);

		this->getChildByTag(ObjectTag_Information)->removeAllChildren();
		auto talk = Label::createWithTTF("Good Luck!", "fonts/arial.ttf", 120);
		talk->enableOutline(Color4B::WHITE, 2);
		talk->setPosition(this->getChildByTag(ObjectTag_Information)->getContentSize().width / 2,
			this->getChildByTag(ObjectTag_Information)->getContentSize().height * 2);
		this->getChildByTag(ObjectTag_Information)->addChild(talk);
	}
	//钱不够
	else {
		this->getChildByTag(ObjectTag_Information)->removeAllChildren();
		auto talk = Label::createWithTTF("You coins are not enough", "fonts/arial.ttf", 120);
		talk->enableOutline(Color4B::WHITE, 2);
		talk->setPosition(this->getChildByTag(ObjectTag_Information)->getContentSize().width / 2,
			this->getChildByTag(ObjectTag_Information)->getContentSize().height * 2);
		this->getChildByTag(ObjectTag_Information)->addChild(talk);
	}
}

void Merchant::putIntoMap(TMXTiledMap* map) {
	my_map = map;
	map->addChild(this, map->getLayer("player")->getLocalZOrder() - 1);
}

