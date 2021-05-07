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
	//���õ�ͼ
	this->addChild(map);
	choosePlayer();
	setButton();
	

	return true;
}
void ChooseScene::addPlayer() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//�������
	Player* player = Player::create();
	if (player_num == 1) {
		player = Player::setRanger();
		//��������
		player->setPosition(Vec2(visibleSize.width / 4 * 3, visibleSize.height / 3 + origin.y));
	}
	else if(player_num == 2){
		player = Player::setSorcerer();
		player->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 3 + origin.y));
	}
	map->addChild(player);

	// ����������
	PlayerMoveController* moving = PlayerMoveController::create();
	moving->setSpeed(7);
	this->addChild(moving);
	player->setController(moving);
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
void ChooseScene::choosePlayer() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//���þ���
	
	player_rangerwb->setScale(visibleSize.height / player_rangerwb->getContentSize().height / 10);
	player_rangerwb->setPosition(Vec2(visibleSize.width / 4 * 3, visibleSize.height / 3 + origin.y));
	this->addChild(player_rangerwb, 1);

	player_sorcererwb->setScale(visibleSize.height / player_sorcererwb->getContentSize().height / 9);
	player_sorcererwb->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 3 + origin.y));
	this->addChild(player_sorcererwb, 1);

	//������¼�����
	auto listenerMouse = EventListenerMouse::create();
	listenerMouse->onMouseUp = [=](Event* event) {
		EventMouse* e = (EventMouse*)event;//ת��ΪEventMouse���ܶ�ȡ
		Vec2 spriteLocatione = e->getLocationInView();
		Rect rect_ranger = player_rangerwb->getBoundingBox();
		Rect rect_sorcerer = player_sorcererwb->getBoundingBox();
		if (rect_ranger.containsPoint(spriteLocatione) && player_num == 0) {//����������ͼƬ��
			player_rangerwb->setVisible(false);
			player_num = 1;
			addPlayer();
			this->scheduleUpdate();
		}
		else if (rect_sorcerer.containsPoint(spriteLocatione) && player_num == 0) {//����������ͼƬ��
			player_sorcererwb->setVisible(false);
			player_num = 2;
			addPlayer();
			this->scheduleUpdate();
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);
}

void ChooseScene::setButton() {
	//��ȡ��Ļ��ʾ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//pause��ť
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
void ChooseScene::suspendCallback(cocos2d::Ref* pSender) {
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

