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
    return true;
}

void SuspendScene::continueCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->popScene();
}

void SuspendScene::returnMainCallback(cocos2d::Ref* pSender) {
    extern int player_num;
    player_num = 0;
    Director::getInstance()->popToRootScene();
}

void SuspendScene::setCallback(cocos2d::Ref* pSender) {
    //��ȡ��Ļ��ʾ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //���ñ���
    auto setBackground = Sprite::create("suspendScene/setBackground.png");
    setBackground->setScale(visibleSize.width / setBackground->getContentSize().width / 3);
    setBackground->setPosition(visibleSize.width / 2, setBackground->getContentSize().height *
        visibleSize.width / setBackground->getContentSize().width / 3 / 2);
    this->addChild(setBackground);
    //�������ְ�ť
    auto setSpriteNormal = Sprite::create("suspendScene/musicNormal.png");
    auto setSpriteSelected = Sprite::create("suspendScene/musicSelected.png");
    auto setItemNormal = MenuItemSprite::create(setSpriteNormal, setSpriteNormal);
    auto setItemSelected = MenuItemSprite::create(setSpriteSelected, setSpriteSelected);
    auto musicItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SuspendScene::musicCallback, this),
        setItemNormal, setItemSelected, NULL);
    musicItem->setPosition(musicItem->getContentSize().width / 3 * 2, 0);
    //�����˵�
    auto menu = Menu::create(musicItem, NULL);
    setBackground->addChild(menu);
    menu->setPosition(0, musicItem->getContentSize().height / 2 + 10);
    
}

Scene* SuspendScene::scene(RenderTexture* sqr) {
    Scene* scene =Scene::create();
    SuspendScene* layer = SuspendScene::create();
    scene->addChild(layer, 1);
    //���Ӳ��֣�ʹ��Game�����н�ͼ��sqr����ͼƬ����Sprite

  //�õ����ڵĴ�С
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
    back_spr->setPosition(visibleSize.width / 2, visibleSize.height / 2); //����λ��,������������λ�á�
    back_spr->setFlippedY(true);            //��ת����ΪUI�����OpenGL���겻ͬ
    back_spr->setColor(Color3B::GRAY); //ͼƬ��ɫ���ɫ
    scene->addChild(back_spr);
    return scene;
}

void SuspendScene::musicCallback(cocos2d::Ref* pSender) {

}
