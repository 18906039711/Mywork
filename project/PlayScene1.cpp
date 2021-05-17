#include"PlayScene1.h"
#include "ui/CocosGUI.h"

extern int player_num;

Scene* PlayScene1::createScene()
{
	return PlayScene1::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainScene.cpp\n");
}

bool PlayScene1::init()
{
	if (!Scene::init()) {
		return false;
	}
	this->scheduleUpdate();
	setPlayer();
	setButton();
	
	return true;
}

void PlayScene1::update(float delta) {
	// Register an update function that checks to see if the CTRL key is pressed
	// and if it is displays how long, otherwise tell the user to press it
	Node::update(delta);
	EventListenerKeyboard* playerMove = EventListenerKeyboard::create();
	playerMove->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event)
	{
		keyMap[code] = true;
	};
	playerMove->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
		keyMap[code] = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(playerMove, this);

	auto up = EventKeyboard::KeyCode::KEY_W;
	auto left = EventKeyboard::KeyCode::KEY_A;
	auto down = EventKeyboard::KeyCode::KEY_S;
	auto right = EventKeyboard::KeyCode::KEY_D;

	float X = 0, Y = 0;
	static int mark = 0;


	if (keyMap[up]) {
		Y = 7;
	}
	if (keyMap[left]) {
		mark = 1;
		X = -7;
	}
	if (keyMap[down]) {
		Y = -7;
	}
	if (keyMap[right]) {
		X = 7;
		mark = 0;
	}
	auto sprite_left = this->getChildByTag(2);
	auto sprite_right = this->getChildByTag(1);

	if (mark) {
		sprite_left->setVisible(true);
		sprite_right->setVisible(false);
	}
	else {
		sprite_left->setVisible(false);
		sprite_right->setVisible(true);
	}
	sprite_left->runAction(MoveBy::create(0.2f, Vec2(X, Y)));
	sprite_right->runAction(MoveBy::create(0.2f, Vec2(X, Y)));
}

void PlayScene1::suspendCallback(cocos2d::Ref* pSender) {
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

void PlayScene1::MusicCallback(cocos2d::Ref* pSender)
{

}

void PlayScene1::setPlayer() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	if (player_num == 1) {
		player_right = Sprite::create("character/ranger_right.png");
		player_left = Sprite::create("character/ranger_left.png");
		player_right->setScale(visibleSize.height / player_right->getContentSize().height / 10);
		player_left->setScale(visibleSize.height / player_right->getContentSize().height / 10);
	}
	else if (player_num == 2) {
		player_right = Sprite::create("character/sorcerer_right.png");
		player_left = Sprite::create("character/sorcerer_left.png");
		player_right->setScale(visibleSize.height / player_right->getContentSize().height / 9);
		player_left->setScale(visibleSize.height / player_right->getContentSize().height / 9);
	}

	player_right->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	
	player_left->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	

	
	this->addChild(player_right, 1, 1);
	this->addChild(player_left, 1, 2);
	player_left->setVisible(false);
	player_right->setVisible(true);
}

void PlayScene1::setButton() {
	//获取屏幕显示大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	

	//pause按钮
	auto suspendItem = MenuItemImage::create(
		"button/suspend.png",
		"button/suspend.png",
		CC_CALLBACK_1(PlayScene1::suspendCallback, this));
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

	//音乐按钮
	auto openSpr = Sprite::create("button/openMusic.png");
	auto closeSpr = Sprite::create("button/closeMusic.png");
	auto openItem = MenuItemSprite::create(openSpr, openSpr);
	auto closeItem = MenuItemSprite::create(closeSpr, closeSpr);
	auto musicItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(PlayScene1::MusicCallback, this), openItem, closeItem, NULL);
	if (musicItem == nullptr ||
		musicItem->getContentSize().width <= 0 ||
		musicItem->getContentSize().height <= 0)
	{
		problemLoading("'button/openMusic.png' and 'button/closeMusic.png'");
	}
	else
	{
		musicItem->setPosition(Vec2(x - musicItem->getContentSize().width * 3 / 2, y));
	}
	auto menu = Menu::create(suspendItem, musicItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

