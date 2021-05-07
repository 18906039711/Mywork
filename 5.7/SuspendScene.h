#pragma once
#ifndef __SUSPEND_SCENE_H__
#define __SUSPEND_SCENE_H__

#include "cocos2d.h"
#include"ChooseScene.h"

USING_NS_CC;

class SuspendScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void continueCallback(cocos2d::Ref* pSender);
    void returnMainCallback(cocos2d::Ref* pSender);
    void setCallback(cocos2d::Ref* pSender);
    void musicCallback(cocos2d::Ref* pSender);

    static Scene* scene(RenderTexture* sqr);

    // implement the "static create()" method manually
    CREATE_FUNC(SuspendScene);
};

#endif // __SUSPEND_SCENE_H__
