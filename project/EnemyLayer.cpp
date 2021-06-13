#include "EnemyLayer.h"

bool EnemyLayer::init()
{
	this->scheduleUpdate();

	//����Tag
	this->setTag(ObjectTag_EnemyLayer);

	/*this->bindSprite(Sprite::create("door.png"));
	my_sprite->setContentSize(Size(1000, 1000));*/

	return true;
}

void EnemyLayer::update(float delta) {
	if (player == nullptr) {
		return;
	}

	Vec2 Point = Vec2(this->getPosition().x - borderLength / 2,
		this->getPosition().y - borderLength / 2);
	Rect rect = Rect(Point, Size(borderLength, borderLength));
	auto playerSize = player->showSprite()->getBoundingBox().size;
	auto playerRect = Rect(Vec2(player->getPosition().x - playerSize.width / 2, player->getPosition().y - playerSize.height / 2), playerSize);

	//��������غ���û���ɵ���
	if (rect.intersectsRect(playerRect)&&!enemyNum) {
		dynamic_cast<PlayScene1*>(my_map->getParent())->setFence();
		createEnemies();
	}
	//������ڵ���
	if (enemyNum) {
		if (enemyVec.empty()) {
			dynamic_cast<PlayScene1*>(my_map->getParent())->removeFence();
			this->unscheduleUpdate();
			return;
		}
		for (auto& enemy : enemyVec) {
			//�����������,�Ƴ��õ���
			if (!enemy->aliveMark) {
				enemyVec.eraseObject(enemy);
				break;
			}
		}
		
	}
}

void EnemyLayer::putIntoMap(TMXTiledMap* map) {
	my_map = map;
	my_map->addChild(this, map->getLayer("player")->getLocalZOrder());
	if (my_map->getChildByTag(ObjectTag_Player) != nullptr) {
		player = dynamic_cast<Player*>(my_map->getChildByTag(ObjectTag_Player));
	}
}

void EnemyLayer::createEnemies() {
	enemyNum = rand_0_1() * 5 + 4;
	for (int i = 0; i < enemyNum; i++) {
		Enemy* dummy = Enemy::create(Dummy);
		enemyVec.pushBack(dummy);
	}
	for (int i = 0; i < enemyNum; i++) {
		//��ÿ�����˶��岻ͬ��tag
		enemyVec.at(i)->putIntoMap(my_map, ObjectTag_Enemy + i);
		enemyVec.at(i)->setPosition(this->getPosition().x + rand_0_1() * borderLength - borderLength / 2,
			this->getPosition().y + rand_0_1() * borderLength - borderLength / 2);
	}
	
}