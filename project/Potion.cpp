#include"Potion.h"

bool Potion::init()
{
	this->scheduleUpdate();
	//设置Tag
	this->setTag(ObjectTag_Potion);

	std::string potionID = "Potion/" + std::to_string(ID) + ".png";
	Sprite* potionSprite = Sprite::create(potionID);
	potionSprite->setScale(static_cast<float>(0.6));
	this->bindSprite(potionSprite);
	setInformation();

	getPotion->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(getPotion, this);

	return true;
}

Potion* Potion::create(int m_ID) {
	auto potion = new Potion();
	potion->ID = m_ID;

	if (potion && potion->init()) {
		potion->autorelease();
		return potion;
	}
	CC_SAFE_DELETE(potion);
	return nullptr;
}
void Potion::update(float delta) {
	//获得图片的Rect,Rect设置的点在左下角
	Vec2 potionPoint = Vec2(this->getPosition().x - my_sprite->getBoundingBox().size.width / 2,
		this->getPosition().y - my_sprite->getBoundingBox().size.height / 2);
	Rect potionRect = Rect(potionPoint, this->my_sprite->getBoundingBox().size);
	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByTag(ObjectTag_Player));
	if (player == nullptr) {
		return;
	}
	auto playerSize = player->showSprite()->getBoundingBox().size;
	auto playerRect = Rect(Vec2(player->getPosition().x - player->showSprite()->getBoundingBox().size.width / 2,
		player->getPosition().y - player->showSprite()->getBoundingBox().size.height / 2), playerSize);
	if (potionRect.intersectsRect(playerRect)) {
		this->showInfomation();
		getPotion->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event)
		{
			if (code == EventKeyboard::KeyCode::KEY_J && player != nullptr) {
				player->getPotion(this);
			}
		};
	}
	else {
		this->removeInfomation();
		//依然有监听，但监听无内容
		getPotion->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {};
	}
}

void Potion::showInfomation() {
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

void Potion::removeInfomation() {
	Sprite* potionInformation = dynamic_cast<Sprite*>(this->getChildByTag(ObjectTag_Information));
	if (potionInformation != nullptr) {
		potionInformation->runAction(RemoveSelf::create());
	}
}

void Potion::setInformation() {
	char HPStr[50];
	sprintf(HPStr, "%d_HP", ID);
	HPRecoveryVolume = UserDefault::getInstance()->getIntegerForKey(HPStr);
	char MPStr[50];
	sprintf(MPStr, "%d_MP", ID);
	MPRecoveryVolume = UserDefault::getInstance()->getIntegerForKey(MPStr);
	char nameStr[50];
	sprintf(nameStr, "%d_name", ID);
	name = UserDefault::getInstance()->getStringForKey(nameStr);
}

void Potion::putIntoMap(TMXTiledMap* map) {
	my_map = map;
	map->addChild(this, map->getLayer("player")->getLocalZOrder() + 1);
}

void Potion::recover(Player* player) {
	player->changeHP(HPRecoveryVolume);
	player->changeMP(MPRecoveryVolume);
}
