#include "PlayScene1.h"

extern int player_num;

Scene* PlayScene1::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = PlayScene1::create();
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainScene.cpp\n");
}

bool PlayScene1::init()
{
	if (!Layer::init()) {
		return false;
	}
	this->scheduleUpdate();

	setMap();
	setButton();
	setMusic();
	setTreasureChest();
	displayCoinNum();
	addPlayer();
	removeFence();

	auto enemyLayer = EnemyLayer::create();
	enemyLayer->putIntoMap(map);
	enemyLayer->setPosition(mapWidth / 148 * 22, mapHeight / 2);
	return true;
}

void PlayScene1::update(float delta) {

}

void PlayScene1::setMap() {
	//���õ�ͼ
	this->addChild(map);

	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	map->setPosition(visibleSize.width / 2 - mapWidth / 2, visibleSize.height / 2 - mapHeight / 2);
	map->getLayer("fenceFloor")->setPosition(0, -32);
	map->getLayer("fence1")->setPosition(0, -32);
	map->getLayer("fence2")->setPosition(0, 32);

	//map->getLayer("barrier")->setVisible(false);
	//map->getLayer("enemyBarrier")->setVisible(false);

}

void PlayScene1::setMusic() {

	// set the background music and continuously play it.
    backGroundMusic1 = AudioEngine::play2d("music/Forest.mp3", true);
	//������ID����UserDefault�ļ���
	UserDefault::getInstance()->setIntegerForKey("backGroundMusicID", backGroundMusic1);

	//��ȡ֮ǰ������
	int volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
	AudioEngine::setVolume(backGroundMusic1, volumePercent / 100.f);
}

void PlayScene1::suspendCallback(cocos2d::Ref* pSender) {
	//��ȡ��Ļ�ߴ��С
	Size winSize = Director::getInstance()->getWinSize();

	//����RenderTexture������ͼƬ��СΪ���ڴ�СwinSize
	RenderTexture* screen = RenderTexture::create(winSize.width, winSize.height);

	//��Ļ��ͼ
	screen->begin();            //��ʼץ��
	this->visit();              //������ǰ����Scene�Ľڵ���Ϣ������screen��
	screen->end();              //����ץ��

	Director::getInstance()->pushScene(SuspendScene::scene(screen));
}

void PlayScene1::setButton() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//pause��ť
	auto suspendItem = MenuItemImage::create(
		"button/suspend.png",
		"button/suspend.png",
		CC_CALLBACK_1(PlayScene1::suspendCallback, this));
	float x = origin.x + visibleSize.width - suspendItem->getContentSize().width / 2;
	float y = origin.y + visibleSize.height - suspendItem->getContentSize().height / 2;

	if (suspendItem == nullptr ||
		suspendItem->getContentSize().width <= 0 ||
		suspendItem->getContentSize().height <= 0)
	{
		problemLoading("'button/suspend.png' and 'button/suspend.png'");
	}
	else
	{
		suspendItem->setPosition(Vec2(x, y));
	}
	auto menu = Menu::create(suspendItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void PlayScene1::displayCoinNum() {
	CoinUI* coinPanel = CoinUI::create();
	this->addChild(coinPanel);
}

void PlayScene1::addPlayer() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//�������
	player = Player::create();
	player->putIntoMap(map);
	player->setPosition(mapWidth / 2, mapHeight / 2);

	//�ӽǸ���
	player->playerFollowingMark = true;

	//�̳��������
	player->setPlayerAttribute();
	player->getWeapon();


	//����playerUI
	PlayerUI* playerUI = PlayerUI::create();
	playerUI->bindPlayer(player);
	this->addChild(playerUI);

}

void PlayScene1::setTreasureChest() {
	TreasureChest* chest1 = TreasureChest::create(1);
	chest1->setPosition(mapWidth / 2+256, mapHeight / 2);
	map->addChild(chest1, map->getLayer("player")->getLocalZOrder() - 1);

	TreasureChest* chest2 = TreasureChest::create(2);
	chest2->setPosition(mapWidth / 2-256, mapHeight / 2);
	map->addChild(chest2, map->getLayer("player")->getLocalZOrder() - 1);

}

/*mark = true,set the fence. 
  mark = false,remvove the fence.*/
void ChangeFence(TMXTiledMap* map, int x, bool mark = false) {
	if (mark) {
		//Vertical
		for (float i = 9; i <= 64; i += 26) {
			for (float j = 0; j < 4; j++) {
				map->getLayer("barrier")->setTileGID(1,Vec2(x, i + j));
			}
		}
		//Horizontal
		for (float i = 9; i <= 64; i += 26) {
			for (float j = 0; j < 4; j++) {
				map->getLayer("barrier")->setTileGID(1,Vec2(i + j, x));
			}
		}
	}
	else {
		//Vertical
		for (float i = 9; i <= 64; i += 26) {
			for (float j = 0; j < 4; j++) {
				map->getLayer("barrier")->removeTileAt(Vec2(x, i + j));
			}
		}
		//Horizontal
		for (float i = 9; i <= 64; i += 26) {
			for (float j = 0; j < 4; j++) {
				map->getLayer("barrier")->removeTileAt(Vec2(i + j, x));
			}
		}
	}
}


void PlayScene1::setFence() {
	ChangeFence(map, 20, true);
	ChangeFence(map, 27, true);
	ChangeFence(map, 46, true);
	ChangeFence(map, 53, true);
	map->getLayer("fence1")->setVisible(true);
	map->getLayer("fence2")->setVisible(true);
}

void PlayScene1::removeFence() {
	ChangeFence(map, 20, false);
	ChangeFence(map, 27, false);
	ChangeFence(map, 46, false);
	ChangeFence(map, 53, false);
	map->getLayer("fence1")->setVisible(false);
	map->getLayer("fence2")->setVisible(false);
}

