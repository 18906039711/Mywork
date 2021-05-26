#include"CoinUI.h"

bool CoinUI::init()
{
	this->scheduleUpdate();
	this->setUI();
	return true;
}

void CoinUI::intializeCoin() {
	UserDefault::getInstance()->setIntegerForKey("CoinNumber", 0);
}

void CoinUI::setUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	CoinBackGround->setScale(visibleSize.width / CoinBackGround->getContentSize().width / 10);
	CoinBackGround->setPosition(visibleSize.width / 5 * 4, visibleSize.height - CoinBackGround->getContentSize().height / 2);
	CoinBackGround->setOpacity(200);
	this->addChild(CoinBackGround);

	coinNum = UserDefault::getInstance()->getIntegerForKey("CoinNumber", 0);
	//数字显示
	std::string Coinstr = std::to_string(coinNum);
	Label* CoinLabel = Label::createWithTTF(Coinstr, "fonts/arial.ttf", 80);
	CoinLabel->setPosition(CoinBackGround->getContentSize().width / 4 * 3, CoinBackGround->getContentSize().height / 2);
	CoinBackGround->addChild(CoinLabel, 1, 1);
}

void CoinUI::update(float delta) {
	if (coinNum != UserDefault::getInstance()->getIntegerForKey("CoinNumber")) {
		coinNum = UserDefault::getInstance()->getIntegerForKey("CoinNumber");
		//数字显示
		std::string Coinstr = std::to_string(coinNum);
		Label* CoinLabel = Label::createWithTTF(Coinstr, "fonts/arial.ttf", 80);
		CoinBackGround->removeChildByTag(1);
		CoinLabel->setPosition(CoinBackGround->getContentSize().width / 4 * 3, CoinBackGround->getContentSize().height / 2);
		CoinBackGround->addChild(CoinLabel,1,1);
	}
}