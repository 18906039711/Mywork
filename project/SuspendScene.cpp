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

    //获取屏幕显示大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float x = visibleSize.width / 2;
    float y = visibleSize.height / 2;

    //suspend界面
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

    //continue按钮
    auto continueItem = MenuItemImage::create(
        "suspendScene/continue.png",
        "suspendScene/continue.png",
        CC_CALLBACK_1(SuspendScene::continueCallback, this));
    //home按钮
    auto homeItem = MenuItemImage::create(
        "suspendScene/home.png",
        "suspendScene/home.png",
        CC_CALLBACK_1(SuspendScene::returnMainCallback, this));
    //set按钮
    auto setItem = MenuItemImage::create(
        "suspendScene/setNormal.png",
        "suspendScene/setSelected.png",
        CC_CALLBACK_1(SuspendScene::setCallback, this));
    
    
    auto menu = Menu::create(continueItem, homeItem, setItem, NULL);
    //menu按钮设为界面子节点
    suspendSprite->addChild(menu, 1);
    //以左下角为menu左边原点
    menu->setPosition(Vec2::ZERO);
    //将menu设在中间
    menu->setPosition(suspendSprite->getContentSize().width / 2, continueItem->getContentSize().height / 2 + 50);
    //continue按钮为中心
    continueItem->setPosition(0, 0);
    //home按钮在左边
    homeItem->setPosition(-continueItem->getContentSize().width / 2 - homeItem->getContentSize().width / 2 - 10, 0);
    //set按钮在右边
    setItem->setPosition(continueItem->getContentSize().width / 2 + setItem->getContentSize().width / 2 + 10, 0);

    setBackGround();
    return true;
}

void SuspendScene::setBackGround() {
    //获取屏幕显示大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //设置背景
    Background = Sprite::create("suspendScene/setBackground.png");
    float Scaling = visibleSize.width / Background->getContentSize().width / 3;
    Background->setScale(Scaling);
    Background->setPosition(visibleSize.width / 2, -Background->getContentSize().height * Scaling / 2);
    this->addChild(Background);

    //设置音乐按钮
    auto setSpriteNormal = Sprite::create("suspendScene/musicNormal.png");
    auto setSpriteSelected = Sprite::create("suspendScene/musicSelected.png");
    auto setItemNormal = MenuItemSprite::create(setSpriteNormal, setSpriteNormal);
    auto setItemSelected = MenuItemSprite::create(setSpriteSelected, setSpriteSelected);
    auto musicItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SuspendScene::musicCallback, this),
        setItemNormal, setItemSelected, NULL);
    musicItem->setPosition(musicItem->getContentSize().width / 3 * 2 + 10, 0);

    //创建菜单
    auto menu = Menu::create(musicItem, NULL);
    Background->addChild(menu);
    menu->setPosition(0, -Background->getContentSize().height * Scaling / 2 + musicItem->getContentSize().height * 2 / 3);
    
    //设置音量滑动条
    musicSlider->loadBarTexture("suspendScene/musicSlider_back.png"); // what the slider looks like
    musicSlider->loadSlidBallTextures("suspendScene/musicSliderNode_Normal.png",
        "suspendScene/musicSliderNode_Press.png", "suspendScene/musicSliderNode_Normal.png");
    musicSlider->loadProgressBarTexture("suspendScene/musicSlider_PressBar.png");

    musicSlider->setScale(Background->getContentSize().width / musicSlider->getContentSize().width / 2);
    musicSlider->setPosition(Vec2(Background->getContentSize().width / 5 * 3 - 40, musicItem->getContentSize().height / 2 + 20));
    musicItem->addChild(musicSlider);

    volumePercent = UserDefault::getInstance()->getIntegerForKey("volumePercent", 100);
    //初始为100
    musicSlider->setPercent(volumePercent);

    //显示音量百分比
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
    //增加部分：使用Game界面中截图的sqr纹理图片创建Sprite

  //得到窗口的大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* backSpr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
    backSpr->setPosition(visibleSize.width / 2, visibleSize.height / 2); //放置位置,这个相对于中心位置。
    backSpr->setFlippedY(true);            //翻转，因为UI坐标和OpenGL坐标不同
    backSpr->setColor(Color3B::GRAY); //图片颜色变灰色
    scene->addChild(backSpr);
    return scene;
}

void SuspendScene::musicCallback(cocos2d::Ref* pSender) {

}