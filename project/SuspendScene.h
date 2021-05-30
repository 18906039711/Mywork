#pragma once
#ifndef __SUSPEND_SCENE_H__
#define __SUSPEND_SCENE_H__

#include "cocos2d.h"
#include"ChooseScene.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include <string>
#include "AudioEngine.h"

extern int playerID;

USING_NS_CC;

class SuspendScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void setBackGround();

    // a selector callback
    void continueCallback(cocos2d::Ref* pSender);
    void returnMainCallback(cocos2d::Ref* pSender);
    void setCallback(cocos2d::Ref* pSender);
    void sliderCallback(cocos2d::Ref* pSender, ui::Slider::EventType type);
    void musicCallback(cocos2d::Ref* pSender);

    static Scene* scene(RenderTexture* sqr);

    // implement the "static create()" method manually
    CREATE_FUNC(SuspendScene);

private:
    Sprite* Background;

    ui::Slider* musicSlider = ui::Slider::create();



    //“Ù¡ø≥ı ºŒ™100
    int volumePercent = 100;
};

#endif // __SUSPEND_SCENE_H__
