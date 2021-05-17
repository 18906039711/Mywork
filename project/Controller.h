#pragma once
#ifndef _ControllerListener_H_
#define _ControllerListener_H_
#include"cocos2d.h"
USING_NS_CC;

//控制器
class ControllerListener :public Node {
public:
	//设置监听对象
	void setControllerListener(ControllerListener* controllerListener);

protected:
	ControllerListener* my_controllerListener;
};


#endif