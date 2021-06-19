#include "BossScene.h"

extern int playerID;

Scene* BossScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = BossScene::create();
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainScene.cpp\n");
}

bool BossScene::init()
{
	if (!Layer::init()) {
		return false;
	}
	this->scheduleUpdate();

	setMap();
	setButton();
	setMusic();
	displayCoinNum();
	//addPlayer();
	createBoss();
	showBossHP();

	return true;
}

void BossScene::setMap() {
	//���õ�ͼ
	this->addChild(map);

	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	map->setPosition(visibleSize.width / 2 - mapWidth / 2, visibleSize.height / 2 - mapHeight / 2);

	map->getLayer("barrier")->setVisible(false);
	map->getLayer("enemyBarrier")->setVisible(false);
}

void BossScene::setMusic() {

	// set the background music and continuously play it.
	backGroundMusic1 = AudioEngine::play2d("music/Boss.mp3", true);
	//������ID����UserDefault�ļ���
	UserDefault::getInstance()->setIntegerForKey("backGroundMusicID", backGroundMusic1);

	//��ȡ֮ǰ������
	int volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
	AudioEngine::setVolume(backGroundMusic1, volumePercent / 100.f);
}

void BossScene::suspendCallback(cocos2d::Ref* pSender) {
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

void BossScene::setButton() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//pause��ť
	auto suspendItem = MenuItemImage::create(
		"button/suspend.png",
		"button/suspend.png",
		CC_CALLBACK_1(BossScene::suspendCallback, this));
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

void BossScene::displayCoinNum() {
	CoinUI* coinPanel = CoinUI::create();
	this->addChild(coinPanel);
}

void BossScene::addPlayer(float delta) {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//�������
	player = Player::create();
	player->putIntoMap(map);
	player->setPosition(mapWidth / 2 - visibleSize.width / 4, mapHeight / 2);

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

void BossScene::createBoss() {
	boss->putIntoMap(map, ObjectTag_Enemy);
	boss->setPosition(mapWidth / 2, mapHeight / 2);
	bossAppearance();
}

void BossScene::bossAppearance() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	std::string coverID = "Enemy/" + std::to_string(GoblinPriest) + "/cover.png";
	Sprite* cover = Sprite::create(coverID);
	cover->setScale(visibleSize.width / cover->getContentSize().width);
	cover->setPosition(visibleSize.width * 2, visibleSize.height / 2);
	cover->runAction(Sequence::create(DelayTime::create(static_cast<float>(0.5)),
		MoveBy::create(static_cast<float>(0.5), Vec2(-visibleSize.width * 3 / 2, 0)),
		DelayTime::create(static_cast<float>(1)), MoveBy::create(static_cast<float>(0.5), Vec2(-visibleSize.width / 20, 0)),
		MoveBy::create(static_cast<float>(0.5), Vec2(visibleSize.width * 3 / 2, 0)), NULL));
	this->addChild(cover);
	this->scheduleOnce(CC_SCHEDULE_SELECTOR(BossScene::addPlayer), 3);
}

void BossScene::showBossHP() {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	BossHP->setPercent(100);
	this->addChild(BossHP);
	BossHP->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - BossHP->getBoundingBox().size.height));
}

void BossScene::update(float delta) {
	//auto exit = map->getChildByTag(ObjectTag_Exit);
	if (player != nullptr) {
		if (!(player->aliveMark)) {
			gameOver();
		}
	}
	//����С���Ƿ�ȫ��
	for (int i = 0; i < 1; i++) {
		if (i != 4) {
			auto enemyLayer = dynamic_cast<EnemyLayer*>(map->getChildByTag(ObjectTag_EnemyLayer - i));
			if (enemyLayer != nullptr) {
				break;
			}

		}
		if (i == 0) {
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("createBoss");
			_eventDispatcher->removeCustomEventListeners("createBoss");
		}
	}

	//����bossѪ��
	char HPStr[50];
	sprintf(HPStr, "%d_HP", boss->ID);
	int maxHP = UserDefault::getInstance()->getIntegerForKey(HPStr);
	BossHP->setPercent(static_cast<float>(boss->getHP()) / maxHP * 100);
	CCLOG("%d/%d", boss->getHP(), maxHP);
}

void BossScene::gameOver() {
	if (this->getChildByName("gameOverLabel") != nullptr) {
		return;
	}
	auto tombstone = Sprite::create("character/died.png");
	tombstone->setPosition(map->getChildByTag(ObjectTag_Player)->getPosition());
	tombstone->setScale(static_cast<float>(0.7));
	map->addChild(tombstone, map->getLayer("player")->getLocalZOrder());

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto gameOverLabel = Label::createWithTTF("Game  Over", "fonts/arial.ttf", 200);
	gameOverLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(gameOverLabel, 0, "gameOverLabel");

	this->scheduleOnce(CC_SCHEDULE_SELECTOR(BossScene::returnToChoose), static_cast<float>(1));
}

void BossScene::returnToChoose(float dt) {
	AudioEngine::stopAll();
	Director::getInstance()->replaceScene(TransitionFade::create(static_cast<float>(2), ChooseScene::createScene()));
	playerID = 0;
}
