#include"SuspendScene.h"

Scene* SuspendScene::createScene()
{
    return SuspendScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainScene.cpp\n");
}

bool SuspendScene::init() {
    if (!Scene::init()) {
        return false;
    }

    //��ȡ��Ļ��ʾ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float x = visibleSize.width / 2;
    float y = visibleSize.height / 2;

    //suspend����
    auto suspendSprite = Sprite::create("suspendScene/suspendSprite.png");
    suspendSprite->setPosition(x, y);
    suspendSprite->setScale(visibleSize.height / suspendSprite->getContentSize().height / 2);
    this->addChild(suspendSprite);

    Sprite* PlayerSculpture = Sprite::create();
    if (playerID == 1) {
        PlayerSculpture = Sprite::create("suspendScene/rangerSculpture.png");
        
    }
    else if (playerID == 2) {
        PlayerSculpture = Sprite::create("suspendScene/sorcererSculpture.png");
        
    }
    PlayerSculpture->setPosition(PlayerSculpture->getContentSize().width / 2 + 52,
        suspendSprite->getContentSize().height - PlayerSculpture->getContentSize().height / 2 - 80);
    suspendSprite->addChild(PlayerSculpture);

    //continue��ť
    auto continueItem = MenuItemImage::create(
        "suspendScene/continue.png",
        "suspendScene/continue.png",
        CC_CALLBACK_1(SuspendScene::continueCallback, this));
    //home��ť
    auto homeItem = MenuItemImage::create(
        "suspendScene/home.png",
        "suspendScene/home.png",
        CC_CALLBACK_1(SuspendScene::returnMainCallback, this));
    //set��ť
    auto setItem = MenuItemImage::create(
        "suspendScene/setNormal.png",
        "suspendScene/setSelected.png",
        CC_CALLBACK_1(SuspendScene::setCallback, this));
    
    
    auto menu = Menu::create(continueItem, homeItem, setItem, NULL);
    //menu��ť��Ϊ�����ӽڵ�
    suspendSprite->addChild(menu, 1);
    //�����½�Ϊmenu���ԭ��
    menu->setPosition(Vec2::ZERO);
    //��menu�����м�
    menu->setPosition(suspendSprite->getContentSize().width / 2, continueItem->getContentSize().height / 2 + 50);
    //continue��ťΪ����
    continueItem->setPosition(0, 0);
    //home��ť�����
    homeItem->setPosition(-continueItem->getContentSize().width / 2 - homeItem->getContentSize().width / 2 - 10, 0);
    //set��ť���ұ�
    setItem->setPosition(continueItem->getContentSize().width / 2 + setItem->getContentSize().width / 2 + 10, 0);

    setBackGround();
    return true;
}

void SuspendScene::setBackGround() {
    //��ȡ��Ļ��ʾ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //���ñ���
    Background = Sprite::create("suspendScene/setBackground.png");
    float Scaling = visibleSize.width / Background->getContentSize().width / 3;
    Background->setScale(Scaling);
    Background->setPosition(visibleSize.width / 2, -Background->getContentSize().height * Scaling / 2);
    this->addChild(Background);

    //�������ְ�ť
    auto setSpriteNormal = Sprite::create("suspendScene/musicNormal.png");
    auto setSpriteSelected = Sprite::create("suspendScene/musicSelected.png");
    auto setItemNormal = MenuItemSprite::create(setSpriteNormal, setSpriteNormal);
    auto setItemSelected = MenuItemSprite::create(setSpriteSelected, setSpriteSelected);
    auto musicItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SuspendScene::musicCallback, this),
        setItemNormal, setItemSelected, NULL);
    musicItem->setPosition(musicItem->getContentSize().width / 3 * 2 + 10, 0);

    //�����˵�
    auto menu = Menu::create(musicItem, NULL);
    Background->addChild(menu);
    menu->setPosition(0, -Background->getContentSize().height * Scaling / 2 + musicItem->getContentSize().height * 2 / 3);
    
    //��������������
    musicSlider->loadBarTexture("suspendScene/musicSlider_back.png"); // what the slider looks like
    musicSlider->loadSlidBallTextures("suspendScene/musicSliderNode_Normal.png",
        "suspendScene/musicSliderNode_Press.png", "suspendScene/musicSliderNode_Normal.png");
    musicSlider->loadProgressBarTexture("suspendScene/musicSlider_PressBar.png");

    musicSlider->setScale(Background->getContentSize().width / musicSlider->getContentSize().width / 2);
    musicSlider->setPosition(Vec2(Background->getContentSize().width / 5 * 3 - 40, musicItem->getContentSize().height / 2 + 20));
    musicItem->addChild(musicSlider);

    volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
    //��ʼΪ100
    musicSlider->setPercent(volumePercent);

    //��ʾ�����ٷֱ�
    std::string s = std::to_string(volumePercent);
    Label* num = Label::createWithTTF(s, "fonts/arial.ttf", 70);
    Background->addChild(num, 0, 0);
    num->setPosition(Vec2(Background->getContentSize().width / 5 * 3 + 20, Background->getContentSize().height / 4 * 3));
    musicSlider->addEventListener(CC_CALLBACK_2(SuspendScene::sliderCallback, this));

}


void SuspendScene::sliderCallback(cocos2d::Ref* pSender, ui::Slider::EventType type) {
    if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Background->removeChildByTag(0);
        volumePercent = dynamic_cast<ui::Slider*> (pSender)->getPercent();
        std::string s = std::to_string(volumePercent);
        Label* num = Label::createWithTTF(s, "fonts/arial.ttf", 70);
        Background->addChild(num, 0, 0);
        num->setPosition(Vec2(Background->getContentSize().width / 5 * 3 + 20, Background->getContentSize().height / 4 * 3));

        UserDefault::getInstance()->setIntegerForKey("volumePercent", volumePercent);
        auto backGroundMusic = UserDefault::getInstance()->getIntegerForKey("backGroundMusicID");
        AudioEngine::setVolume(backGroundMusic, volumePercent / static_cast<float>(100));
    }
}

void SuspendScene::setCallback(cocos2d::Ref* pSender) {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    if (Background->getPosition().y > 0) {
        Background->runAction(MoveBy::create(0.5f, Vec2(0, -Background->getContentSize().height *
            visibleSize.width / Background->getContentSize().width / 3)));
    }
    else {
        Background->runAction(MoveBy::create(0.5f, Vec2(0, Background->getContentSize().height *
            visibleSize.width / Background->getContentSize().width / 3)));
    }
}

void SuspendScene::continueCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->popScene();
}

void SuspendScene::returnMainCallback(cocos2d::Ref* pSender) {
    extern int playerID;
    playerID = 0;
    AudioEngine::stopAll();
    Director::getInstance()->popToRootScene();
}

Scene* SuspendScene::scene(RenderTexture* sqr) {
    Scene* scene =Scene::create();
    SuspendScene* layer = SuspendScene::create();
    scene->addChild(layer, 1);
    //���Ӳ��֣�ʹ��Game�����н�ͼ��sqr����ͼƬ����Sprite

  //�õ����ڵĴ�С
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* backSpr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
    backSpr->setPosition(visibleSize.width / 2, visibleSize.height / 2); //����λ��,������������λ�á�
    backSpr->setFlippedY(true);            //��ת����ΪUI�����OpenGL���겻ͬ
    backSpr->setColor(Color3B::GRAY); //ͼƬ��ɫ���ɫ
    scene->addChild(backSpr);
    return scene;
}

void SuspendScene::musicCallback(cocos2d::Ref* pSender) {

}