#include"player.h"

bool Player::init()
{
	return true;
}

//可选择创建两种不同角色
Player* Player::setPlayer() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (player_num == 1) {
		Sprite* playerSprite = Sprite::create("character/ranger_right.png");
			playerSprite->setScale(0.2f);
			Player* player = Player::create();
			player->bindSprite(playerSprite);
			player->StandAction();
			player->setPosition(Vec2(visibleSize.width / 4 * 3, visibleSize.height / 3 + origin.y));
			player->setPlayerAttribute();
			return player;
	}
	else if(player_num == 2) {
		Sprite* playerSprite = Sprite::create("character/sorcerer_right.png");
		playerSprite->setScale(0.2f);
		Player* player = Player::create();
		player->bindSprite(playerSprite);
		player->StandAction();
		player->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 3 + origin.y));
		player->setPlayerAttribute();
		return player;
	}
}

void Player::StandAction() {
	//创建序列帧
	auto animation = Animation::create();

	if (player_num == 1) {
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/ranger%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	else if (player_num == 2) {
		for (int i = 1; i <= 2; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/sorcerer%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	//设置两帧间的时间间隔
	animation->setDelayPerUnit(0.3f);
	//设置循环，-1无限循环
	animation->setLoops(-1);
	//在最后一帧播放完恢复到第一帧
	animation->setRestoreOriginalFrame(true);

	my_sprite->runAction(Animate::create(animation));
}

void Player::RunningAction() {
	auto animation = Animation::create();
	if (player_num == 1) {
		for (int i = 1; i <= 4; i++) {
			char nameSize[100] = { 0 };
			sprintf(nameSize, "character/ranger/run%d.png", i);
			animation->addSpriteFrameWithFile(nameSize);
		}
	}
	animation->setDelayPerUnit(0.3f);
	animation->setLoops(-1);
	animation->setRestoreOriginalFrame(true);
	my_sprite->runAction(Animate::create(animation));
}

void Player::stopPlayerAllActions() {
	my_sprite->stopAllActions();
}

void Player::setPlayerAttribute() {
	if (player_num == 1) {
		HP = UserDefault::getInstance()->getIntegerForKey("RangerHP", 6);
		Defendce = UserDefault::getInstance()->getIntegerForKey("RangerDefendce", 3);
		MP = UserDefault::getInstance()->getIntegerForKey("RangerMP", 180);
	}
	else if (player_num == 2) {
		HP = UserDefault::getInstance()->getIntegerForKey("sorcererHP", 3);
		Defendce = UserDefault::getInstance()->getIntegerForKey("sorcererDefendce", 5);
		MP = UserDefault::getInstance()->getIntegerForKey("sorcererMP", 240);
	}
}
