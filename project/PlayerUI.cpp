#include"PlayerUI.h"

bool PlayerUI::init()
{
	return true;
}

void  PlayerUI::getPlayerMaxAttribute() {
	if (player_num == 1) {
		MaxHP = UserDefault::getInstance()->getIntegerForKey("RangerHP", 6);
		MaxDefendce = UserDefault::getInstance()->getIntegerForKey("RangerDefendce", 3);
		MaxMP = UserDefault::getInstance()->getIntegerForKey("RangerMP", 180);
	}
	else if (player_num == 2) {
		MaxHP = UserDefault::getInstance()->getIntegerForKey("sorcererHP", 3);
		MaxDefendce = UserDefault::getInstance()->getIntegerForKey("sorcererDefendce", 5);
		MaxMP = UserDefault::getInstance()->getIntegerForKey("sorcererMP", 240);
	}
}

void PlayerUI::setUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//底框
	BottomFrameUI->setAnchorPoint(Vec2(0, 1));
	BottomFrameUI->setPosition(0, visibleSize.height + BottomFrameUI->getContentSize().height / 2);
	BottomFrameUI->setScale(visibleSize.height / BottomFrameUI->getContentSize().height / 8);
	this->addChild(BottomFrameUI);

	//UI条
	BottomFrameUI->addChild(PlayerHPUI);
	BottomFrameUI->addChild(PlayerDefendceUI);
	BottomFrameUI->addChild(PlayerMPUI);

	PlayerHPUI->setPosition(Vec2(BottomFrameUI->getContentSize().width / 5 * 3 - 27, BottomFrameUI->getContentSize().height / 4 * 3 + 22));
	PlayerDefendceUI->setPosition(Vec2(BottomFrameUI->getContentSize().width / 5 * 3 - 27, BottomFrameUI->getContentSize().height / 2 + 12));
	PlayerMPUI->setPosition(Vec2(BottomFrameUI->getContentSize().width / 5 * 3 - 27, BottomFrameUI->getContentSize().height / 4 + 4));

	BottomFrameUI->runAction(MoveTo::create(0.5, Vec2(0, visibleSize.height)));
}

void PlayerUI::updateUI(Player* player) {
	HP = player->HP;
	MP = player->MP;
	Defendce = player->Defendce;

	PlayerHPUI->setPercent(static_cast<float>(HP) / MaxHP * 100);
	PlayerDefendceUI->setPercent(static_cast<float>(Defendce) / MaxDefendce * 100);
	PlayerMPUI->setPercent(static_cast<float>(MP) / MaxMP * 100);

	//UI条数字显示
	std::string HPstr = std::to_string(HP) + " / " + std::to_string(MaxHP);
	Label* HPLabel = Label::createWithTTF(HPstr, "fonts/arial.ttf", 80);
	HPLabel->setPosition(Vec2(BottomFrameUI->getContentSize().width / 5 * 3 - 27, BottomFrameUI->getContentSize().height / 4 * 3 + 22));

	std::string Defendcestr = std::to_string(Defendce) + " / " + std::to_string(MaxDefendce);
	Label* DefendceLabel = Label::createWithTTF(Defendcestr, "fonts/arial.ttf", 80);
	DefendceLabel->setPosition(Vec2(BottomFrameUI->getContentSize().width / 5 * 3 - 27, BottomFrameUI->getContentSize().height / 2 + 12));

	std::string MPstr = std::to_string(MP) + " / " + std::to_string(MaxMP);
	Label* MPLabel = Label::createWithTTF(MPstr, "fonts/arial.ttf", 80);
	MPLabel->setPosition(Vec2(BottomFrameUI->getContentSize().width / 5 * 3 - 27, BottomFrameUI->getContentSize().height / 4 + 4));

	BottomFrameUI->addChild(HPLabel);
	BottomFrameUI->addChild(DefendceLabel);
	BottomFrameUI->addChild(MPLabel);
}