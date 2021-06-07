#include"PlayScene1.h"

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
	//this->scheduleUpdate();
	//���õ�ͼ
	this->addChild(map);

	setButton();
	setMusic();
	setTreasureChest();
	displayCoinNum();
	addPlayer();
	
	return true;
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
	player->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	//�̳��������
	player->setPlayerAttribute();
	player->getWeapon();


	//����playerUI
	PlayerUI* playerUI = PlayerUI::create();
	playerUI->bindPlayer(player);
	this->addChild(playerUI);

}

void PlayScene1::setTreasureChest() {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	TreasureChest* chest1 = TreasureChest::create(1);
	chest1->setPosition(visibleSize.width / 5 * 2, visibleSize.height / 4 * 3);
	map->addChild(chest1, 5, ObjectTag_TreasureChest);

	TreasureChest* chest2 = TreasureChest::create(2);
	chest2->setPosition(visibleSize.width / 5 * 3, visibleSize.height / 4 * 3);
	map->addChild(chest2, 5, ObjectTag_TreasureChest);

}