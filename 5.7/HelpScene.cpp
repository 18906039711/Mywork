#include"HelpScene.h"

Scene* HelpScene::createScene()
{
	return HelpScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainScene.cpp\n");
}


bool HelpScene::init() {
	if (!Scene::init()) {
		return false;
	}

    //获取屏幕显示大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //返回按钮
    auto returnItem = MenuItemImage::create(
        "button/cancel.png",
        "button/cancel.png",
        CC_CALLBACK_1(HelpScene::returnMainCallback, this));

    if (returnItem == nullptr ||
        returnItem->getContentSize().width <= 0 ||
        returnItem->getContentSize().height <= 0)
    {
        problemLoading("'button/cancel.png' and 'button/cancel.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - returnItem->getContentSize().width / 2;
        float y = origin.y + visibleSize.height - returnItem->getContentSize().height / 2;
        returnItem->setPosition(Vec2(x, y));
    } auto menu = Menu::create(returnItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto help = Sprite::create("help.png");
    if (help == nullptr)
    {
        problemLoading("'help.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        help->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y));
        this->addChild(help, 0);
    }
    return true;
}

void HelpScene::returnMainCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->popScene();
}