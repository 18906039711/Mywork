#include"Coin.h"

bool Coin::init()
{
	//设置Tag
	this->setTag(ObjectTag_Coin);

	this->bindSprite(Sprite::create("Coin/GoldCoin.png"));
	this->setScale(static_cast<float>(0.4));
	my_sprite->runAction(Sequence::create(MoveTo::create(static_cast<float>(0.2), Vec2(rand_0_1() * 300 - 150, rand_0_1() * 300 - 150)),
		DelayTime::create(static_cast<float>(1)), FadeOut::create(static_cast<float>(2)), NULL));

	int coinNum = UserDefault::getInstance()->getIntegerForKey("CoinNumber", 0);
	UserDefault::getInstance()->setIntegerForKey("CoinNumber", coinNum + 1);

	auto soundEffect = AudioEngine::play2d("Music/eatCoin.mp3", false);
	//读取之前的音量
	int volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
	AudioEngine::setVolume(soundEffect, volumePercent / 100.f);
	
	return true;
}

Coin* Coin::create()
{
	Coin* coin = new Coin();
	if (coin && coin->init())
	{
		coin->autorelease();
		return coin;
	}
	CC_SAFE_DELETE(coin);
	return nullptr;
}

