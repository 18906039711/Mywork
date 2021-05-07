#include"player.h"

bool Player::init()
{
	barrier = map->getLayer("piano");
	return true;
}
//��ѡ�񴴽����ֲ�ͬ��ɫ
Player* Player::setRanger() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite* playerSprite = Sprite::create("character/ranger_right.png"); 
	playerSprite->setScale(visibleSize.height / playerSprite->getContentSize().height / 10);
	Player* player = Player::create();
	player->bindSprite(playerSprite);
	return player;
}
Player* Player::setSorcerer() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite* playerSprite = Sprite::create("character/sorcerer_right.png");
	playerSprite->setScale(visibleSize.height / playerSprite->getContentSize().height / 9);
	Player* player = Player::create();
	player->bindSprite(playerSprite);
	return player;
}



