#include"ChooseScene.h"

int playerID = 0;

Scene* ChooseScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0,0));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = ChooseScene::create();
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainScene.cpp\n");
}

bool ChooseScene::init()
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
	choosePlayer();
	setEnemy();
	setDoor();

	return true;
}

void ChooseScene::setMap() {
	//设置地图
	this->addChild(map);

	//获取单个瓦片的尺寸(pixel)
	auto tiledSize = map->getTileSize();
	map->setPosition(-tiledSize.width * 2, -tiledSize.height * 2);
	map->getLayer("barrier")->setVisible(false);
	map->getLayer("enemyBarrier")->setVisible(false);
}

void ChooseScene::choosePlayer() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//设置精灵

	player_rangerwb->setScale(0.2f);
	player_rangerwb->setPosition(Vec2(visibleSize.width / 4 * 3, visibleSize.height / 3));
	this->addChild(player_rangerwb, 1);

	player_sorcererwb->setScale(0.2f);
	player_sorcererwb->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 3));
	this->addChild(player_sorcererwb, 1);

	//鼠标点击事件监听
	auto listenerMouse = EventListenerMouse::create();
	listenerMouse->onMouseUp = [=](Event* event) {
		EventMouse* e = (EventMouse*)event;//转换为EventMouse才能读取
		Vec2 spriteLocatione = e->getLocationInView();
		Rect rect_ranger = player_rangerwb->getBoundingBox();
		Rect rect_sorcerer = player_sorcererwb->getBoundingBox();
		if (rect_ranger.containsPoint(spriteLocatione) && playerID == 0) {//如果点击点在图片内
			player_rangerwb->removeFromParent();
			player_sorcererwb->removeFromParent();
			playerID = rangerID;
			addPlayer();
			_eventDispatcher->removeEventListener(listenerMouse);
		}
		else if (rect_sorcerer.containsPoint(spriteLocatione) && playerID == 0) {//如果点击点在图片内
			player_rangerwb->removeFromParent();
			player_sorcererwb->removeFromParent();
			playerID = sorcererID;
			addPlayer();
			_eventDispatcher->removeEventListener(listenerMouse);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);
}

void ChooseScene::addPlayer() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//设置玩家
	player = Player::create();
	player->putIntoMap(map);
	//设置初始面板属性
	player->setPlayerAttribute();
	//初始化
	player->initializePlayer();

	if (playerID == rangerID) {
		player->setPosition(visibleSize.width / 4 * 3 + 64, visibleSize.height / 3 + 64);
	}
	else if (playerID == sorcererID) {
		player->setPosition(visibleSize.width / 4 + 64, visibleSize.height / 3 + 64);
	}

	//增加playerUI
	PlayerUI* playerUI = PlayerUI::create();
	playerUI->bindPlayer(player);
	this->addChild(playerUI);
	
}

void ChooseScene::setButton() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//pause按钮
	auto suspendItem = MenuItemImage::create(
		"button/suspend.png",
		"button/suspend.png",
		CC_CALLBACK_1(ChooseScene::suspendCallback, this));
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

void ChooseScene::setMusic() {
	// set the background music and continuously play it.
	backGroundMusic = AudioEngine::play2d("music/Home.mp3", true);
	//将音乐ID传入UserDefault文件中
	UserDefault::getInstance()->setIntegerForKey("backGroundMusicID", backGroundMusic);

	//读取之前的音量
	int volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
	AudioEngine::setVolume(backGroundMusic, volumePercent / 100.f);
}

void ChooseScene::suspendCallback(cocos2d::Ref* pSender) {
	//获取屏幕尺寸大小
	Size winSize = Director::getInstance()->getWinSize();

	//创建RenderTexture，纹理图片大小为窗口大小winSize
	RenderTexture* screen = RenderTexture::create(static_cast<int>(winSize.width), static_cast<int>(winSize.height));

	//屏幕截图
	screen->begin();            //开始抓屏
	this->visit();              //遍历当前场景Scene的节点信息，画入screen中
	screen->end();              //结束抓屏

	Director::getInstance()->pushScene(SuspendScene::scene(screen));
}

void ChooseScene::setTreasureChest() {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	TreasureChest* chest1 = TreasureChest::create(1);
	chest1->setPosition(mapWidth / 5 * 2, mapHeight / 10 * 7);
	map->addChild(chest1, map->getLayer("player")->getLocalZOrder() - 1);

	TreasureChest* chest2 = TreasureChest::create(2);
	chest2->setPosition(mapWidth / 5 * 3, mapHeight / 10 * 7);
	map->addChild(chest2, map->getLayer("player")->getLocalZOrder() - 1);

}

void ChooseScene::displayCoinNum() {
	CoinUI* coinPanel= CoinUI::create();
	coinPanel->intializeCoin();
	this->addChild(coinPanel);
}

void ChooseScene::setEnemy() {
	Enemy* dummy = Enemy::create(Dummy);
	dummy->setPosition(mapWidth / 5 * 4, mapHeight / 5 * 3);
	dummy->putIntoMap(map);
}

void ChooseScene::setDoor() {
	//设置通完下一场景的门
	Sprite* door = Sprite::create("door.png");
	door->setContentSize(Size(32 * 8, 32));
	door->setPosition(static_cast<float>(mapWidth / 2), static_cast<float>(mapHeight * 0.85));
	door->setVisible(false);
	map->addChild(door, 1, ObjectTag_Exit);
}

void ChooseScene::update(float dt) {
	auto exit = map->getChildByTag(ObjectTag_Exit);
	if (player != nullptr) {
		if (exit->getBoundingBox().containsPoint(player->getPosition())) {
			AudioEngine::stopAll();
			Director::getInstance()->pushScene(TransitionFade::create(static_cast<float>(0.5), PlayScene1::createScene()));
		}
		if (!(player->aliveMark)) {
			gameOver();
		}
	}
}

void ChooseScene::gameOver() {
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

	this->scheduleOnce(CC_SCHEDULE_SELECTOR(ChooseScene::returnToChoose), static_cast<float>(1));
}

void ChooseScene::returnToChoose(float dt) {
	AudioEngine::stopAll();
	Director::getInstance()->replaceScene(TransitionFade::create(static_cast<float>(2), ChooseScene::createScene()));
	playerID = 0;
}


//
//void ChooseScene::makeEddy() 
//{
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
//	//PolygonInfo eddypinfo = AutoPolygon::generatePolygon("eddy.png");
//	Sprite* eddy = Sprite::create("eddy.png");
//	eddy->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.2 + origin.y));
//	eddy->setScale(0.05f);
//	Action* eddy_amplify = ScaleTo::create(1.f, 0.3f);
//	eddy->runAction(eddy_amplify);	
//	Action* eddy_revolve = RotateBy::create(10000.f, 10000.f * 10);
//	eddy->runAction(eddy_revolve);
//
//	this->addChild(eddy, 1, 100);
//	return ;
//}
//
//void ChooseScene::enterEddy() {
//	auto sprite_right = this->getChildByTag(playerID);
//	auto eddy = this->getChildByTag(100);
//	auto playScene = PlayScene1::create();
//	if (eddy->getBoundingBox().containsPoint(sprite_right->getPosition())) {
//		Director::getInstance()->pushScene(TransitionRotoZoom::create(1.f, playScene));
//	}
//}

