#include"ChooseScene.h"


int player_num = 0;

Scene* ChooseScene::createScene()
{
	return ChooseScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainScene.cpp\n");
}

bool ChooseScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	//设置地图
	this->addChild(map);
	
	choosePlayer();
	setButton();
	setMusic();
	

	return true;
}
void ChooseScene::choosePlayer() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//设置精灵

	player_rangerwb->setScale(0.2f);
	player_rangerwb->setPosition(Vec2(visibleSize.width / 4 * 3, visibleSize.height / 3 + origin.y));
	this->addChild(player_rangerwb, 1);

	player_sorcererwb->setScale(0.2f);
	player_sorcererwb->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 3 + origin.y));
	this->addChild(player_sorcererwb, 1);

	//鼠标点击事件监听
	auto listenerMouse = EventListenerMouse::create();
	listenerMouse->onMouseUp = [=](Event* event) {
		EventMouse* e = (EventMouse*)event;//转换为EventMouse才能读取
		Vec2 spriteLocatione = e->getLocationInView();
		Rect rect_ranger = player_rangerwb->getBoundingBox();
		Rect rect_sorcerer = player_sorcererwb->getBoundingBox();
		if (rect_ranger.containsPoint(spriteLocatione) && player_num == 0) {//如果点击点在图片内
			player_rangerwb->setVisible(false);
			player_sorcererwb->setVisible(false);
			player_num = 1;
			addPlayer();
			this->scheduleUpdate();
		}
		else if (rect_sorcerer.containsPoint(spriteLocatione) && player_num == 0) {//如果点击点在图片内
			player_rangerwb->setVisible(false);
			player_sorcererwb->setVisible(false);
			player_num = 2;
			addPlayer();
			this->scheduleUpdate();
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);
}

void ChooseScene::addPlayer() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//设置玩家
	Player* player = Player::create();
	player = Player::setPlayer();
	this->addChild(player, 0);

	// 创建控制器
	PlayerMoveController* moveController = PlayerMoveController::create();
	moveController->setSpeed(8);
	moveController->getMap(map);
	map->addChild(moveController);

	//对player进行控制
	moveController->bindPlayer(player);

	//增加playerUI
	PlayerUI* playerUI = PlayerUI::create();
	playerUI->getPlayerMaxAttribute();
	playerUI->setUI();
	playerUI->updateUI(player);
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
	/*float x = origin.x + visibleSize.width - suspendItem->getContentSize().width / 2;
	float y = origin.y + visibleSize.height - suspendItem->getContentSize().height / 2;*/
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
	backGroundMusic = AudioEngine::play2d("music/ElecrystalSoundTeam.mp3", true);
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
	RenderTexture* screen = RenderTexture::create(winSize.width, winSize.height);

	//屏幕截图
	screen->begin();            //开始抓屏
	this->visit();              //遍历当前场景Scene的节点信息，画入screen中
	screen->end();              //结束抓屏

	Director::getInstance()->pushScene(SuspendScene::scene(screen));
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
//	auto sprite_right = this->getChildByTag(player_num);
//	auto eddy = this->getChildByTag(100);
//	auto playScene = PlayScene1::create();
//	if (eddy->getBoundingBox().containsPoint(sprite_right->getPosition())) {
//		Director::getInstance()->pushScene(TransitionRotoZoom::create(1.f, playScene));
//	}
//}
//
//void ChooseScene::MusicCallback(cocos2d::Ref* pSender) 
//{
//
//
//}
//
//
//

